#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videocapture.h"
#include "playmanager.h"

#include <opencv2/core.hpp>
#include <QMainWindow>
#include <QSlider>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionPlay_triggered();

    void on_actionStop_triggered();

    void on_actionPause_triggered();

    void on_actionExit_triggered();

    void on_actionFit_triggered();

    void on_actionOriginal_triggered();

    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionPrevious_triggered();

    void on_actionNext_triggered();

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

private:
    void initStyle();

    void initializeSlider();
    void updateSlider(const vtviewer::IVideoCapture& cap, QSlider& slider);

private:
    Ui::MainWindow *ui;

    vtviewer::VideoCapture capture;
    vtviewer::PlayManager pm;
};

#endif // MAINWINDOW_H
