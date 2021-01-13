#include "playmanager.h"

#include <QDebug>

namespace vtviewer
{

PlayManager::~PlayManager()
{
    qDebug() << "deconstructor of playmanager...";
}

void PlayManager::play(IVideoCapture &reader, bool playnow)
{
    atom_pause = !playnow;
    if(atom_stop) // create a new thread
    {
        atom_stop = false;

        t_producer = std::thread([&reader, obj = this]()
        {
            obj->producer_thread(reader);
        });

        t_producer.detach();
    }
    else
    {
        if(!atom_pause) //carry on
        {
            cv_play.notify_one();
        }
    }
}

void PlayManager::stop(IVideoCapture& reader)
{
    atom_stop = true;
    cv_play.notify_all();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    mtx_play_sig.lock();
    mtx_play_sig.unlock();

    Q_UNUSED(reader);
}

void PlayManager::setImageProcessor(const std::function<PlayManager::ImageProcessor> proc)
{
    processor = proc;
}

void PlayManager::producer_thread(IVideoCapture &reader)
{
    int ret = 0;
    std::unique_lock<std::mutex> locker(mtx_play_sig);
    while(true)
    {
        if(atom_stop)
        {
            reader.reset();
            qDebug() << "stop: producer thread exists.";
            return;
        }

        while(atom_pause)
        {
            if(atom_stop)
            {
                reader.reset();
                qDebug() << "stop when pause: producer thread exists";
                return;
            }
            cv_play.wait(locker);
        }
        cv::Mat img = reader.next();
        if(img.empty())
        {
            atom_stop = true;
            qDebug() << "ERROR IN FETCH : " << ret << " and producer thread exists";
            reader.reset();
            return;
        }
        if(processor!=nullptr)
        {
            processor(img);
        }
    }
}

}   // namespace brick
