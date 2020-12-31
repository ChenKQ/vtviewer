#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_url = "/home/chenkq/Desktop/test.mp4";
    ui->setupUi(this);
    initStyle();

    int retcode = reader.init("/home/chenkq/Desktop/test.mp4");
//    int retcode = reader.init("rtmp://58.200.131.2:1935/livetv/hunantv");
    assert(retcode==0);
    reader.installActionsOnFetchedVideoFrame([obj=this](const unsigned char* const* data, const int* linesize,
                                             int width, int height, int pixelFormat)
    {
        obj->ui->canvas->updateBuffer(data, linesize, width, height, pixelFormat);
    });
}

MainWindow::~MainWindow()
{
    pm.stop(reader);
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

void MainWindow::on_actionPlay_triggered()
{

    pm.play(reader, true);
}

void MainWindow::on_actionStop_triggered()
{
    pm.stop(reader);
    reader.reset();
}

void MainWindow::on_actionPause_triggered()
{
    pm.play(reader, false);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
