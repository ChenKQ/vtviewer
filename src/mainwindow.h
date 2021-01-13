#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videocapture.h"
#include "playmanager.h"

#include <opencv2/core.hpp>
#include <QMainWindow>


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

private:
    void initStyle();

private:
    Ui::MainWindow *ui;

    vtviewer::VideoCapture capture;
    vtviewer::PlayManager pm;
};

#endif // MAINWINDOW_H
