#ifndef ANTICORE_GLVIDEOWIDGET_H
#define ANTICORE_GLVIDEOWIDGET_H

#include "media/ffmpeg.h"
#include "glvideorender.h"
#include "media/image.h"

#include <QOpenGLWidget>

class GlVideoWidget : public QOpenGLWidget
{
public:
    GlVideoWidget(QWidget* parent = nullptr);
    ~GlVideoWidget() override;

    void updateBuffer(const unsigned char* const* data, const int* linesize,
                      int width, int height, int pixelFormat);

    void updateBuffer(const unsigned char* data, int stride,
                      int width, int height, int pixelFormat);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

protected:
    static constexpr brick::media::PixelFormat pix_fmt =
            brick::media::PixelFormat::YUV420P;

    GLVideoRender<pix_fmt> m_render;
    brick::media::Image imgBuffer;

    int m_height = 0;
    int m_width = 0;
    brick::media::PixelFormat m_pixel_format = brick::media::PixelFormat::UNKNOWN;
};


#endif // ANTICORE_GLVIDEOWIDGET_H
