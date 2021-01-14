#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <memory>

namespace vtcore
{
namespace io
{

class IVideoCapture
{
public:
    virtual bool open(const std::string& path) = 0;
    virtual bool isOpen() const = 0;
    virtual cv::Mat previous() = 0;
    virtual cv::Mat next() = 0;
    virtual void reset() = 0;
    virtual size_t frameCount() const = 0;
    virtual size_t currentFrameIndex() const = 0;
    virtual void setFrameIndex(size_t index) = 0;
    virtual ~IVideoCapture() = default;

    static std::unique_ptr<IVideoCapture> CreateInstance(const std::string& captureType);

};

class VideoCapture : public IVideoCapture
{
public:
    bool open(const std::string& path) override;
    bool isOpen() const override;
    cv::Mat previous() override;
    cv::Mat next() override;
    void reset() override;
    size_t frameCount() const override;
    size_t currentFrameIndex() const override;
    void setFrameIndex(size_t index) override;
    ~VideoCapture() override = default;
private:
    cv::VideoCapture capture;
    cv::Mat image;
    std::string path;
};
} // namespace io
} // namespace vtcore


#endif // VIDEOCAPTURE_H
