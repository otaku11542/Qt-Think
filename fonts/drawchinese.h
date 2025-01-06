#ifndef DRAWCHINESE_H
#define DRAWCHINESE_H

#include <QObject>
#include <QDebug>
#include <iostream>
#include <QTextCodec>
#include <math.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class DrawChinese : public QObject
{
    Q_OBJECT
public:
    explicit DrawChinese(QObject *parent = nullptr);
    void paint_chinese(cv::Mat& image, cv::Point start, unsigned long offset, cv::Scalar color);
    void paint_ascii(cv::Mat& image, cv::Point start, unsigned long offset, cv::Scalar color);
    void put_text_to_image(cv::Mat &image, QString text, cv::Scalar color);

private:
    unsigned char * qstringToHex(QString hex);

signals:

};

#endif // DRAWCHINESE_H

