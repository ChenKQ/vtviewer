#ifndef VIEW_H
#define VIEW_H

#include "vtcore/render/coordinate.h"

namespace vtcore
{
namespace render
{

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

}
}

#endif // VIEW_H
