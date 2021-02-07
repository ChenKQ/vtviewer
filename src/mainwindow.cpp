#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QSlider>
#include <QFileDialog>

#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initStyle();
}

MainWindow::~MainWindow()
{
    if(m_pCapture)
    {
        pm.stop(*m_pCapture);
    }
    delete ui;
}

void MainWindow::initStyle()
{
    //加载样式表
    QString qss;
    QStringList list;
    QFile file(":/qss/psblack.css");
    QFile defaultstylefile(":/qss/default.css");

    if (file.open(QFile::ReadOnly))
    {
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
    size_t length = m_pCapture->frameCount();
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(static_cast<int>(length)-1);
    ui->horizontalSlider->setValue(0);
}

void MainWindow::updateSlider(const vtcore::io::IVideoCapture &cap, QSlider &slider)
{
    int captureIndex = static_cast<int>(cap.currentFrameIndex());
    slider.setValue(captureIndex);
}

void MainWindow::openDirectory(std::string captureType)
{
    QFileDialog fileDlg;
    fileDlg.setWindowTitle(QStringLiteral("choose a video file..."));
    fileDlg.setDirectory("./");
    fileDlg.setNameFilter(tr("File(*.*)"));
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    fileDlg.setViewMode(QFileDialog::Detail);

    if(fileDlg.exec())
    {
        QString fileName = fileDlg.selectedFiles()[0];
        pm.stop(*m_pCapture);
        m_pCapture = vtcore::io::IVideoCapture::CreateInstance(captureType);
        m_pCapture->reset();
        m_pCapture->open(fileName.toStdString());
        initializeSlider();

        m_pFinder = vtcore::io::IAnnotationFinder::CreateInstance(captureType);
        m_pFinder->Open(fileName.toStdString());

        std::vector<vtcore::tracking::BBox> boxes =
                m_pFinder->GetBoxes(m_pCapture->currentFrameIndex());
        ui->canvas->updateAnnotations(boxes);

        pm.setImageProcessor([obj=this](cv::Mat& img)
        {
            obj->ui->canvas->updateBuffer(img);
            obj->updateSlider(*obj->m_pCapture, *obj->ui->horizontalSlider);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            std::vector<vtcore::tracking::BBox> boxes =
                    obj->m_pFinder->GetBoxes(obj->m_pCapture->currentFrameIndex());
            obj->ui->canvas->updateAnnotations(boxes);
        });
    }
}

void MainWindow::on_actionPlay_triggered()
{
    if(m_pCapture)
    {
        pm.play(*m_pCapture, true);
    }
}

void MainWindow::on_actionStop_triggered()
{
    if(m_pCapture)
    {
        pm.stop(*m_pCapture);
        initializeSlider();
    }
    cv::Mat img;
    ui->canvas->updateBuffer(img);
}

void MainWindow::on_actionPause_triggered()
{
    if(m_pCapture)
    {
        pm.play(*m_pCapture, false);
    }
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
    if(m_pCapture)
    {
        cv::Mat img = m_pCapture->previous();
        updateSlider(*m_pCapture, *ui->horizontalSlider);
        ui->canvas->updateBuffer(img);

        std::vector<vtcore::tracking::BBox> boxes =
                m_pFinder->GetBoxes(m_pCapture->currentFrameIndex());
        ui->canvas->updateAnnotations(boxes);
    }
}

void MainWindow::on_actionNext_triggered()
{
    if(m_pCapture)
    {
        cv::Mat img = m_pCapture->next();
        updateSlider(*m_pCapture, *ui->horizontalSlider);
        ui->canvas->updateBuffer(img);

        std::vector<vtcore::tracking::BBox> boxes =
                m_pFinder->GetBoxes(m_pCapture->currentFrameIndex());
        ui->canvas->updateAnnotations(boxes);
    }
}


void MainWindow::on_horizontalSlider_sliderPressed()
{
    if(m_pCapture)
    {
        pm.play(*m_pCapture, false);
    }

}

void MainWindow::on_horizontalSlider_sliderReleased()
{

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    if(m_pCapture)
    {
        m_pCapture->setFrameIndex(static_cast<size_t>(position));
        cv::Mat img = m_pCapture->current();
        ui->canvas->updateBuffer(img);

        std::vector<vtcore::tracking::BBox> boxes =
                m_pFinder->GetBoxes(m_pCapture->currentFrameIndex());
        ui->canvas->updateAnnotations(boxes);
    }
}

void MainWindow::on_actionVideoFile_triggered()
{
    openDirectory("video");
    on_actionNext_triggered();
}

void MainWindow::on_actionOTB_triggered()
{
    openDirectory("otb");
    if(m_pCapture)
    {
        cv::Mat img = m_pCapture->current();
        updateSlider(*m_pCapture, *ui->horizontalSlider);
        ui->canvas->updateBuffer(img);
    }
}
