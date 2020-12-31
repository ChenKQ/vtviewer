#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "media/ffmpegreader.h"
#include "media/playmanager.h"

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

private:
    void initStyle();

private:
    Ui::MainWindow *ui;

    std::string m_url;
    brick::media::FFMpegReader reader;
    brick::media::PlayManager pm;
};

#endif // MAINWINDOW_H
