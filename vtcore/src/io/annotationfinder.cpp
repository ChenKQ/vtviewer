#include "vtcore/io/annotationfinder.h"

#include "vtcore/io/otb.h"

namespace vtcore
{
namespace io
{

std::unique_ptr<IAnnotationFinder> IAnnotationFinder::CreateInstance(const std::string &captureType)
{
    if(captureType=="otb")
    {
        OtbAnnotationFinder* p = new OtbAnnotationFinder;
        return std::unique_ptr<IAnnotationFinder>(p);
    }
    return nullptr;
}

}   // namespace io
}   // namespace vtcore
