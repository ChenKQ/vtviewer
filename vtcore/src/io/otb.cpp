#include "vtcore/io/otb.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

namespace vtcore
{
namespace io
{

bool OtbCapture::open(const std::string &path)
{
    // load image and copy path to path
    image = cv::imread(path, cv::IMREAD_UNCHANGED);
    if(image.empty())
        return false;

    // list all the files and sort
    QFileInfo finfo {QString::fromStdString(path)};
    QString parentDir = finfo.path();
    QString suffix = finfo.completeSuffix();

    QStringList nameFilters;
    nameFilters << "*." + suffix;
    QDir dir(parentDir);
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    imageFiles.clear();
    imageFiles.reserve(static_cast<size_t>(files.size()));
    for(int i=0; i<files.size(); ++i)
    {
        QString f = parentDir + "/" + files[i];
        imageFiles.push_back(f.toStdString());
    }
    // find current index
    currentIndex = files.indexOf(finfo.fileName());
}

bool OtbCapture::isOpen() const
{
    return !image.empty();
}

cv::Mat OtbCapture::previous()
{
    if(currentIndex>0)
    {
        --currentIndex;
    }
    return current();

}

cv::Mat OtbCapture::next()
{
    if(currentIndex<static_cast<int>(imageFiles.size()-1))
    {
        ++currentIndex;
    }

    return current();
}

cv::Mat OtbCapture::current()
{
    std::string filePath = imageFiles[static_cast<size_t>(currentIndex)];
    image = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    return image;
}

void OtbCapture::reset()
{
    currentIndex = 0;
    if(!imageFiles.empty())
    {
        current();
    }
}

size_t OtbCapture::frameCount() const
{
    return imageFiles.size();
}

size_t OtbCapture::currentFrameIndex() const
{
    return static_cast<size_t>(currentIndex);
}

void OtbCapture::setFrameIndex(size_t index)
{
    currentIndex = static_cast<int>(index);
    current();
}

std::vector<tracking::BBox> OtbAnnotationFinder::GetBoxes(size_t index)
{
    if(bboxes.empty())
        return {};
    return bboxes[index];
}

bool OtbAnnotationFinder::Open(const std::string &path)
{
    bboxes.clear();
    //
    QFileInfo finfo {QString::fromStdString(path)};
    QString dir = finfo.path();

    QDir dirdir(dir);
    dirdir.cdUp();
    QString parentDir = dirdir.path();

    QString gtFile = parentDir + "/groundtruth_rect.txt";
    QFile dataFile(gtFile);

    if(dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream strm(&dataFile);
        QString line;
        while(!strm.atEnd())
        {
            line = strm.readLine();
            line.remove('\n');
            QStringList numberStrs = line.split(QRegExp("[,\t]"));
            int x = numberStrs[0].toInt();
            int y = numberStrs[1].toInt();
            int w = numberStrs[2].toInt();
            int h = numberStrs[3].toInt();
            tracking::BBox box {"", 1.0, x, x+w, y, y+h};
            bboxes.push_back(std::vector<tracking::BBox>{box});
        }
    }
}

}   // namespace io
}   // namespace vtcore
