#ifndef ANTICORE_GLVIDEOWIDGET_H
#define ANTICORE_GLVIDEOWIDGET_H

#include "media/ffmpeg.h"
#include "glvideorender.h"
#include "media/image.h"
#include "utils.h"

#include <QOpenGLWidget>
#include <QPoint>

class GlVideoWidget : public QOpenGLWidget
{
public:
    GlVideoWidget(QWidget* parent = nullptr);
    ~GlVideoWidget() override;

    // call this method before call initializeGL and paintGL
    void setPixelFormat(int pixelFormat);

    void updateBuffer(const unsigned char* const* data, const int* linesize,
                      int width, int height, int pixelFormat);

    void updateBuffer(const unsigned char* data, int stride,
                      int width, int height, int pixelFormat);

    void fitDisplay();
    void originalDisplay();
    void zoomIn();
    void zoomOut();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::unique_ptr<ImageRender> m_pRender;
    brick::media::Image imgBuffer;

    int m_height = 0;
    int m_width = 0;
    brick::media::PixelFormat m_pixel_format = brick::media::PixelFormat::UNKNOWN;

    vtviewer::View m_view;
    QPoint m_loc_mouse_press;
};


#endif // ANTICORE_GLVIDEOWIDGET_H
