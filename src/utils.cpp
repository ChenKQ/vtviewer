#include "utils.h"

namespace vtviewer
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

float View::getScaleRate(const SizeInt &winSize, const SizeInt &imgSize)
{
    float scaleRateWidth = static_cast<float>(imgSize.x)/static_cast<float>(winSize.x);
    float scaleRateHeight = static_cast<float>(imgSize.y)/static_cast<float>(winSize.y);

    if(scaleRateWidth<scaleRateHeight)
    {
        return scaleRateHeight;
    }
    else
    {
        return scaleRateWidth;
    }
}

CoordinateF &View::getGLBorder(const SizeInt &winSize, const SizeInt &imgSize)
{
    float scaleRateWidth = static_cast<float>(imgSize.x)/static_cast<float>(winSize.x);
    float scaleRateHeight = static_cast<float>(imgSize.y)/static_cast<float>(winSize.y);

    float scaleRate = 0.0;
    float validWindowHeight = 0.0;
    float validWindowWidth = 0.0;
    if(scaleRateWidth<scaleRateHeight)
    {
        scaleRate = scaleRateHeight;
        validWindowHeight = winSize.y;
        validWindowWidth = imgSize.x/scaleRate;
    }
    else
    {
        scaleRate = scaleRateWidth;
        validWindowHeight = imgSize.y/scaleRate;
        validWindowWidth = winSize.x;
    }
    GLBorder = {validWindowWidth/winSize.x, validWindowHeight/winSize.y};
    return GLBorder;
}

void View::updateGLBorder(const SizeInt &winSize, const SizeInt &imgSize)
{
    getGLBorder(winSize, imgSize);
}

void View::updateOffsetGL(const OffsetInt &offsetInQt, const SizeInt &windowSize)
{
    OffsetGL = OffsetTransform::Qt2OpenGL(offsetInQt, windowSize);
}

void View::fitDisplay(const SizeInt &winSize, const SizeInt &imgSize)
{
    getGLBorder(winSize, imgSize);
    ZoomRate = 1.0;
    OffsetGL = {0.0, 0.0};
}

void View::originalDisplay(const SizeInt &winSize, const SizeInt &imgSize)
{
    float scaleRate = getScaleRate(winSize, imgSize);
    ZoomRate = scaleRate;
}

void View::zoomIn()
{
    ZoomRate *= ZoomSensitivity;
}

void View::zoomOut()
{
    ZoomRate /= ZoomSensitivity;
}

void View::drag(const OffsetInt &offsetInQt, const SizeInt &windowSize)
{
    OffsetGL += OffsetTransform::Qt2OpenGL(offsetInQt, windowSize);
}

template<typename T>
Coordinate<T> &Coordinate<T>::operator+=(const Coordinate<T> &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

template<typename T>
Coordinate<T> operator+(const Coordinate<T> &lhs, const Coordinate<T> &rhs)
{
    return {lhs.x+rhs.x, lhs.y+rhs.y};
}

} // namespace vtviewer


