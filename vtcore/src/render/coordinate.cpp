#include "vtcore/render/coordinate.h"

namespace vtcore
{
namespace render
{

CoordinateF CoordinateTransform::Qt2OpenGL(const CoordinateInt &posInQt, const SizeInt& windowSize)
{
    using PosType = CoordinateF::Type;
    PosType x = static_cast<PosType>(2*posInQt.x)/windowSize.x - 1.0f;
    PosType y = static_cast<PosType>(2*posInQt.y)/windowSize.y - 1.0f;
    return {x,y};
}

CoordinateInt CoordinateTransform::OpenGL2Qt(const CoordinateF &posInGL, const SizeInt& windowSize)
{
    using PosType = CoordinateInt::Type;
    PosType x = static_cast<PosType>((posInGL.x+1.0f) * windowSize.x/2);
    PosType y = static_cast<PosType>((posInGL.y+1.0f) * windowSize.y/2);
    return {x,y};
}

OffsetF OffsetTransform::Qt2OpenGL(const OffsetInt &offsetInQt, const SizeInt &windowSize)
{
    using LengthType = typename OffsetF::Type;
    LengthType deltaX = 2.0f*offsetInQt.x/windowSize.x;
    LengthType deltaY = 2.0f*offsetInQt.y/windowSize.y;
    return {deltaX, deltaY};
}

OffsetInt OffsetTransform::OpenGL2Qt(const OffsetF &offsetInGL, const SizeInt &windowSize)
{
    using LengthType = typename OffsetInt::Type;
    LengthType deltaX = static_cast<LengthType>(offsetInGL.x*windowSize.x/2);
    LengthType deltaY = static_cast<LengthType>(offsetInGL.y*windowSize.y/2);
    return {deltaX, deltaY};
}

} // namespace render
} // namespace vtcore


