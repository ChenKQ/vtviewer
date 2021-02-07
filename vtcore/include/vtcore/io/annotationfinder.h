#ifndef ANNOTATIONFINDER_H
#define ANNOTATIONFINDER_H

#include "vtcore/tracking/bbox.h"

#include <string>
#include <vector>
#include <memory>

namespace vtcore
{
namespace io
{

class IAnnotationFinder
{
public:
    virtual bool Open(const std::string& path) = 0;
    virtual std::vector<tracking::BBox> GetBoxes(size_t index) = 0;
    virtual ~IAnnotationFinder() = default;

    static std::unique_ptr<IAnnotationFinder> CreateInstance(const std::string& captureType);
};

}   // namespace tracking
}   // namespace vtcore

#endif // ANNOTATIONFINDER_H
