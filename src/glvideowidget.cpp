#include "glvideowidget.h"
#include "utils.h"

#include <QMouseEvent>

GlVideoWidget::GlVideoWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setCursor(Qt::CursorShape::DragMoveCursor);
}

GlVideoWidget::~GlVideoWidget()
{

}

void GlVideoWidget::setPixelFormat(int pixelFormat)
{
    m_pixel_format = static_cast<brick::media::PixelFormat>(pixelFormat);
    m_pRender = ImageRender::CreateInstance(m_pixel_format);
}

void GlVideoWidget::updateBuffer(const unsigned char* const* data, const int* linesize,
                                      int width, int height, int pixelFormat)
{
    assert(m_pixel_format==pixelFormat);
    assert(m_pRender);

    if(imgBuffer.empty() || m_width!= width || m_height!=height)
    {
        m_width = width;
        m_height = height;
        m_pixel_format = static_cast<brick::media::PixelFormat>(pixelFormat);
        imgBuffer = brick::media::Image(width, height, m_pixel_format);
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

void GlVideoWidget::fitDisplay()
{
    m_view.fitDisplay({width(),height()}, {imgBuffer.getWidth(), imgBuffer.getHeight()});
    this->update();
}

void GlVideoWidget::originalDisplay()
{
    m_view.originalDisplay({width(),height()}, {imgBuffer.getWidth(), imgBuffer.getHeight()});
    this->update();
}

void GlVideoWidget::zoomIn()
{
    m_view.zoomIn();
    this->update();
}

void GlVideoWidget::zoomOut()
{
    m_view.zoomOut();
    this->update();
}

void GlVideoWidget::initializeGL()
{
    m_pRender->initialize();

    vtviewer::SizeInt winSize {this->width(), this->height()};
    m_view.fitDisplay(winSize, winSize);
}

void GlVideoWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
//    glViewport(0, 0, w, h);
}

void GlVideoWidget::paintGL()
{
    vtviewer::SizeInt winSize {this->width(), this->height()};
    vtviewer::SizeInt imgSize = winSize;
    if(!imgBuffer.empty())
    {
        imgSize = {imgBuffer.getWidth(), imgBuffer.getHeight()};
    }
    m_view.updateGLBorder(winSize, imgSize);

    m_pRender->render(imgBuffer.getDataPtr(), m_width, m_height, m_view);
}

void GlVideoWidget::mousePressEvent(QMouseEvent *event)
{
    m_loc_mouse_press = event->pos();
}

void GlVideoWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    if(event->DragMove)
    {
        vtviewer::OffsetInt offsetInQt = {currentPos.x() - m_loc_mouse_press.x(), -currentPos.y() + m_loc_mouse_press.y()};
        m_view.drag(offsetInQt, {width(), height()});
        m_loc_mouse_press = event->pos();
        this->update();
    }
}

void GlVideoWidget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() >0 )
    {
        zoomIn();
    }
    else if(event->delta() < 0)
    {
        zoomOut();
    }
}

