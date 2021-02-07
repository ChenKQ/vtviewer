#include "vtcore/render/glvideowidget.h"
#include "vtcore/render/view.h"
#include "vtcore/render/coordinate.h"

#include <QMouseEvent>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace vtcore
{
namespace render
{

GlVideoWidget::GlVideoWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setCursor(Qt::CursorShape::DragMoveCursor);
    m_annotationPainter.setColor(0.0f, 1.0f, 0.0f);
    m_detectionPainter.setColor(1.0f, 0.0f, 0.0f);
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

    if(!img.empty() && img.channels()==1)
    {
        cv::cvtColor(img, imgBuffer, cv::COLOR_GRAY2BGR);
    }
    else
    {
        img.copyTo(imgBuffer);
    }

    this->update();
}

void GlVideoWidget::updateAnnotations(const std::vector<tracking::BBox> &boxes)
{
    annotations = boxes;
    this->update();
}

void GlVideoWidget::updateDetections(const std::vector<tracking::BBox> &boxes)
{
    detections = boxes;
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
    m_annotationPainter.initialize();
    m_detectionPainter.initialize();

    vtcore::render::SizeInt winSize {this->width(), this->height()};
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
    vtcore::render::SizeInt winSize {this->width(), this->height()};
    vtcore::render::SizeInt imgSize = winSize;
    if(!imgBuffer.empty())
    {
        imgSize = {imgBuffer.cols, imgBuffer.rows};
    }
    m_view.updateGLBorder(winSize, imgSize);

    m_render.render(imgBuffer, m_view);
    for(tracking::BBox& box : annotations)
    {
        CoordinateF topLeftGL = CoordinateTransform::Qt2OpenGL(CoordinateInt{box.minx, box.miny}, imgSize);
        CoordinateF bottomRightGL = CoordinateTransform::Qt2OpenGL(CoordinateInt{box.maxx, box.maxy}, imgSize);
        m_annotationPainter.updateRectBound(topLeftGL.y, bottomRightGL.y, topLeftGL.x, bottomRightGL.x);
        m_annotationPainter.render(m_view);
    }
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
        vtcore::render::OffsetInt offsetInQt = {currentPos.x() - m_loc_mouse_press.x(), -currentPos.y() + m_loc_mouse_press.y()};
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

}   // namespace render
}   // namespace vtcore

