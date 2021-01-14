#ifndef COORDINATE_H
#define COORDINATE_H

namespace vtcore
{
namespace render
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

} // namespace render
}   // namespace vtcore


#endif // COORDINATE_H
