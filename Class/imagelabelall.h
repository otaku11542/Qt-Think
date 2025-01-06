#ifndef IMAGELABELALL_H
#define IMAGELABELALL_H

#include <QLabel>
#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <vector>
#include <QScrollArea>
#include <QEnterEvent>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
class ImageLabelAll : public QVBoxLayout
{
public:
    ImageLabelAll(const QPixmap &bgPixmap, QWidget *parent = nullptr);
public slots:
    void getBox(vector<vector<int>> &box);
private:
    QPixmap originalPixmap;
    QScrollArea *scrollArea;
    QGraphicsView *view;
    qreal scaleFactor;
};

#endif // IMAGELABELALL_H
