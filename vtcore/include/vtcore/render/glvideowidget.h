#ifndef ANTICORE_GLVIDEOWIDGET_H
#define ANTICORE_GLVIDEOWIDGET_H

#include "vtcore/render/glvideorender.h"
#include "vtcore/render/view.h"
#include "vtcore/tracking/bbox.h"
#include "vtcore/render/marks.h"

#include <QOpenGLWidget>
#include <QPoint>

#include <opencv2/core.hpp>

#include <vector>

namespace vtcore
{
namespace  render
{

class GlVideoWidget : public QOpenGLWidget
{
public:
    GlVideoWidget(QWidget* parent = nullptr);
    ~GlVideoWidget() override;

    void updateBuffer(const cv::Mat& img);
    void updateAnnotations(const std::vector<tracking::BBox>& boxes);
    void updateDetections(const std::vector<tracking::BBox>& boxes);

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
    vtcore::render::GLImageRender m_render;
    cv::Mat imgBuffer;

    RectMark m_annotationPainter;
    RectMark m_detectionPainter;
    std::vector<tracking::BBox> annotations;
    std::vector<tracking::BBox> detections;

    vtcore::render::View m_view;
    QPoint m_loc_mouse_press;
};

}   // namespace render
}   // namespace vtcore

#endif // ANTICORE_GLVIDEOWIDGET_H
