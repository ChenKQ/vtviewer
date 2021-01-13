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

void GlVideoWidget::updateBuffer(const cv::Mat& img)
{
    if(imgBuffer.empty() || imgBuffer.cols!= img.cols || imgBuffer.rows!=img.rows)
    {
        imgBuffer = cv::Mat(img.rows, img.cols, CV_8UC3);
    }

    img.copyTo(imgBuffer);
    this->update();
}

void GlVideoWidget::fitDisplay()
{
    m_view.fitDisplay({width(),height()}, {imgBuffer.cols, imgBuffer.rows});
    this->update();
}

void GlVideoWidget::originalDisplay()
{
    m_view.originalDisplay({width(),height()}, {imgBuffer.cols, imgBuffer.rows});
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
    m_render.initialize();

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
        imgSize = {imgBuffer.cols, imgBuffer.rows};
    }
    m_view.updateGLBorder(winSize, imgSize);

    m_render.render(imgBuffer.data, imgBuffer.cols, imgBuffer.rows, m_view);
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

