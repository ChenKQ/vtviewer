#ifndef OTB_H
#define OTB_H

#include "videocapture.h"
#include "annotationfinder.h"
#include "vtcore/tracking/bbox.h"

#include <string>
#include <vector>
#include <opencv2/core.hpp>

namespace vtcore
{
namespace io
{

class OtbCapture: public IVideoCapture
{
public:
    bool open(const std::string& path) override;
    bool isOpen() const override;
    cv::Mat previous() override;
    cv::Mat next() override;
    cv::Mat current() override;
    void reset() override;
    size_t frameCount() const override;
    size_t currentFrameIndex() const override;
    void setFrameIndex(size_t index) override;
    ~OtbCapture() override = default;
private:
    std::string path;
    int currentIndex = -1;
    std::vector<std::string> imageFiles;
    cv::Mat image;
};

class OtbAnnotationFinder: public IAnnotationFinder
{
public:
    bool Open(const std::string& path) override;
    std::vector<tracking::BBox> GetBoxes(size_t index) override;
    ~OtbAnnotationFinder() override = default;

private:
    std::vector<std::vector<tracking::BBox>> bboxes;
    std::string folder;
};

}   // namespace io
}   // namespace vtcore

#endif // OTB_H
