#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace vtviewer
{

class IVideoCapture
{
public:
    virtual bool open(const std::string& path) = 0;
    virtual bool isOpen() = 0;
    virtual cv::Mat previous() = 0;
    virtual cv::Mat next() = 0;
    virtual void reset() = 0;
    virtual size_t frameCount() = 0;
    virtual size_t currentFrameIndex() = 0;
    virtual ~IVideoCapture() = default;
};

class VideoCapture : public IVideoCapture
{
public:
    bool open(const std::string& path) override;
    bool isOpen() override;
    cv::Mat previous() override;
    cv::Mat next() override;
    void reset() override;
    size_t frameCount() override;
    size_t currentFrameIndex() override;
    ~VideoCapture() override = default;
private:
    cv::VideoCapture capture;
    cv::Mat image;
    std::string path;
};

} // namespace vtviewer


#endif // VIDEOCAPTURE_H
