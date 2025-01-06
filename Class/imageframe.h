#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QPainter>
#include <opencv2/opencv.hpp>
class ImageFrame : public QFrame
{
public:
    ImageFrame(const QPixmap &bgPixmap, const QString &str, QWidget *parent = nullptr);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    QPixmap originalPixmap;
    QString strs;
    void showEnlargedImage();

};

#endif // IMAGEFRAME_H
