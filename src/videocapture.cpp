#include "videocapture.h"

namespace vtviewer
{

bool vtviewer::VideoCapture::open(const std::string &path)
{
    this->path = path;
    return capture.open(path, cv::CAP_FFMPEG);
}

bool VideoCapture::isOpen() const
{
    return capture.isOpened();
}

cv::Mat VideoCapture::previous()
{
    size_t currentIndex = currentFrameIndex();
    capture.set(cv::CAP_PROP_POS_FRAMES, currentIndex-2);
    capture >> image;
    return image;
}

cv::Mat VideoCapture::next()
{
    capture >> image;
    return image;
}

void VideoCapture::reset()
{
    capture.release();
    open(path);
}

size_t VideoCapture::frameCount() const
{
    return static_cast<size_t>(capture.get(cv::CAP_PROP_FRAME_COUNT));
}

size_t VideoCapture::currentFrameIndex() const
{
    return static_cast<size_t>(capture.get(cv::CAP_PROP_POS_FRAMES));
}

void VideoCapture::setFrameIndex(size_t index)
{
    capture.set(cv::CAP_PROP_POS_FRAMES, index);
}

std::unique_ptr<IVideoCapture> IVideoCapture::CreateInstance(const std::string &captureType)
{
    if(captureType == "videoFile")
    {
        VideoCapture* p = new VideoCapture;
        return std::unique_ptr<IVideoCapture>(p);
    }

    return nullptr;
}


}   // namespace vtviewer


