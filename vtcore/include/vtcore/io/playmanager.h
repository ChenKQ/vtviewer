#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include "videocapture.h"

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <opencv2/opencv.hpp>

namespace vtcore
{
namespace io
{

class PlayManager final
{
public:
    using ImageProcessor = void (cv::Mat& img);
//    std::function<>
public:
    PlayManager() = default;
    ~PlayManager();

    void play(IVideoCapture& reader, bool playnow = false);
    void stop(IVideoCapture& reader);
    void setImageProcessor(const std::function<ImageProcessor> proc);

    inline bool stopped() { return atom_stop; }

private:
    std::thread t_producer;

    std::mutex mtx_play_sig;
    std::atomic<bool> atom_stop = {true};
    std::atomic<bool> atom_pause = {true};
    std::condition_variable cv_play;

    std::function<ImageProcessor> processor;

    void producer_thread(IVideoCapture &reader);
};

}   // namespace io
}   //namespac vtcore

#endif // PLAYMANAGER_H
