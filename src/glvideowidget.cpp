#include "glvideowidget.h"

GlVideoWidget::GlVideoWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GlVideoWidget::~GlVideoWidget()
{

}

void GlVideoWidget::updateBuffer(const unsigned char* const* data, const int* linesize,
                                      int width, int height, int pixelFormat)
{
    if(imgBuffer.empty() || m_width!= width || m_height!=height || m_pixel_format!=pixelFormat)
    {
        m_width = width;
        m_height = height;
        m_pixel_format = static_cast<brick::media::PixelFormat>(pixelFormat);
        imgBuffer = brick::media::Image(width, height, pix_fmt);
    }

    imgBuffer.fillBuffer(data, linesize, width, height, pixelFormat, true);
    this->update();
}

void GlVideoWidget::updateBuffer(const unsigned char *data, int stride,
                                 int width, int height, int pixelFormat)
{
    const unsigned char* dataArray[4] {nullptr};
    dataArray[0] = data;

    int linesizes[4] {0};
    linesizes[0] = stride;

    updateBuffer(dataArray, linesizes, width, height, pixelFormat);
}

void GlVideoWidget::initializeGL()
{
    m_render.initialize();
}

void GlVideoWidget::resizeGL(int w, int h)
{
//    glViewport(0, 0, w, h);
}

void GlVideoWidget::paintGL()
{
    m_render.render(imgBuffer.getDataPtr(), m_width, m_height);
}

