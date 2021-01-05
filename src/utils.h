#ifndef UTILS_H
#define UTILS_H

namespace vtviewer
{

template<typename T> struct Coordinate
{
    using Type = T;
    T x;
    T y;

    Coordinate &operator+=(const Coordinate& rhs);
};

template <typename T> Coordinate<T> operator+(const Coordinate<T>& lhs, const Coordinate<T>& rhs);

using CoordinateInt = Coordinate<int>;
using CoordinateF = Coordinate<float>;

template<typename T> using Size = Coordinate<T>;
using SizeInt = Size<int>;
using SizeF = Size<float>;

using OffsetInt = Size<int>;
using OffsetF = Size<float>;

struct CoordinateTransform
{
    static CoordinateF Qt2OpenGL(const CoordinateInt& posInQt, const SizeInt& windowSize);
    static CoordinateInt OpenGL2Qt(const CoordinateF& posInGL, const SizeInt& windowSize);
};

struct OffsetTransform
{
    static OffsetF Qt2OpenGL(const OffsetInt& offsetInQt, const SizeInt& windowSize);
    static OffsetInt OpenGL2Qt(const OffsetF& offsetInGL, const SizeInt& windowSize);
};

struct View
{
    CoordinateF GLBorder;
    OffsetF OffsetGL;
    float ZoomRate;
    float ZoomSensitivity = 1.1f;

    void updateGLBorder(const SizeInt& winSize, const SizeInt& imgSize);
    void updateOffsetGL(const OffsetInt& offsetInQt, const SizeInt& windowSize);

    void fitDisplay(const SizeInt& winSize, const SizeInt& imgSize);
    void originalDisplay(const SizeInt& winSize, const SizeInt& imgSize);
    void zoomIn();
    void zoomOut();
    void drag(const OffsetInt& offsetInQt, const SizeInt& windowSize);

private:
    static float getScaleRate(const SizeInt& winSize, const SizeInt& imgSize);
    CoordinateF& getGLBorder(const SizeInt& winSize, const SizeInt& imgSize);
};

}   // namespace vtviewer


#endif // UTILS_H
