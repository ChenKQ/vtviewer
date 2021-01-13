#ifndef ANTICORE_GLVIDEOWIDGET_H
#define ANTICORE_GLVIDEOWIDGET_H

#include "glvideorender.h"
#include "utils.h"

#include <QOpenGLWidget>
#include <QPoint>

#include <opencv2/core.hpp>

class GlVideoWidget : public QOpenGLWidget
{
public:
    GlVideoWidget(QWidget* parent = nullptr);
    ~GlVideoWidget() override;

    void updateBuffer(const cv::Mat& img);

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
    GLImageRender m_render;
    cv::Mat imgBuffer;

    vtviewer::View m_view;
    QPoint m_loc_mouse_press;
};


#endif // ANTICORE_GLVIDEOWIDGET_H
