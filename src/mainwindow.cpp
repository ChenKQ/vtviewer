#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initStyle();

    capture.open("/home/chenkq/Desktop/test.mp4");
    initializeSlider();
    pm.setImageProcessor([obj=this](cv::Mat& img)
    {
        obj->ui->canvas->updateBuffer(img);
        obj->updateSlider(obj->capture, *obj->ui->horizontalSlider);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    });
}

MainWindow::~MainWindow()
{
    pm.stop(capture);
    delete ui;
}

void MainWindow::initStyle()
{
    //加载样式表
    QString qss;
    QStringList list;
    QFile file(":/qss/psblack.css");
    QFile defaultstylefile(":/qss/default.css");

        if (file.open(QFile::ReadOnly)) {

            //用QTextStream读取样式文件不用区分文件编码 带bom也行

            QTextStream in(&file);
            //in.setCodec("utf-8");
            while (!in.atEnd()) {
                QString line;
                in >> line;
                list << line;
            }
            qss = list.join("\n");
            file.close();
        }

    if (defaultstylefile.open(QFile::ReadOnly))
    {
        //用QTextStream读取样式文件不用区分文件编码 带bom也行
        QTextStream in(&defaultstylefile);
        //in.setCodec("utf-8");
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }

        defaultstylefile.close();
    }

    qss = list.join("\n");
    QString paletteColor = qss.mid(20, 7);
    qApp->setPalette(QPalette(QColor(paletteColor)));
    qApp->setStyleSheet(qss);
}

void MainWindow::initializeSlider()
{
    size_t length = capture.frameCount();
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(static_cast<int>(length));
    ui->horizontalSlider->setValue(0);
}

void MainWindow::updateSlider(const vtviewer::IVideoCapture &cap, QSlider &slider)
{
    int captureIndex = static_cast<int>(cap.currentFrameIndex());
    slider.setValue(captureIndex);
}

void MainWindow::on_actionPlay_triggered()
{
    pm.play(capture, true);

}

void MainWindow::on_actionStop_triggered()
{
    pm.stop(capture);
    initializeSlider();
    cv::Mat img;
    ui->canvas->updateBuffer(img);
}

void MainWindow::on_actionPause_triggered()
{
    pm.play(capture, false);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionFit_triggered()
{
    ui->canvas->fitDisplay();
}

void MainWindow::on_actionOriginal_triggered()
{
    ui->canvas->originalDisplay();
}

void MainWindow::on_actionZoomIn_triggered()
{
    ui->canvas->zoomIn();
}

void MainWindow::on_actionZoomOut_triggered()
{
    ui->canvas->zoomOut();
}

void MainWindow::on_actionPrevious_triggered()
{
    cv::Mat img = capture.previous();
    updateSlider(capture, *ui->horizontalSlider);
    ui->canvas->updateBuffer(img);
}

void MainWindow::on_actionNext_triggered()
{
    cv::Mat img = capture.next();
    updateSlider(capture, *ui->horizontalSlider);
    ui->canvas->updateBuffer(img);
}


void MainWindow::on_horizontalSlider_sliderPressed()
{
    pm.play(capture, false);
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    pm.play(capture, true);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    capture.setFrameIndex(static_cast<size_t>(position));
    cv::Mat img = capture.next();
    ui->canvas->updateBuffer(img);
}
