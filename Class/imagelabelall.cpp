#include "imagelabelall.h"
#include "scalablegraphicsview.h"
#include <QDebug>
ImageLabelAll::ImageLabelAll(const QPixmap &bgPixmap, QWidget *parent)
    : QVBoxLayout(parent), originalPixmap(bgPixmap)
{
    // Create the scroll area
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setGeometry(0, 0, 1050, 450); // Set the width to 1500 pixels

    // Create the graphics view and scene
    view = new ScalableGraphicsView(scrollArea);
    QGraphicsScene *scene = new QGraphicsScene(view);
    view->setScene(scene);
    view->setAlignment(Qt::AlignCenter);

    // Add the pixmap to the scene
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(originalPixmap);
    scene->addItem(pixmapItem);

    // Center the pixmap
    view->centerOn(pixmapItem);

    // Set the view as the widget for the scroll area
    scrollArea->setWidget(view);

    // Add the scroll area to the main layout
    addWidget(scrollArea);
}

void ImageLabelAll::getBox(vector<vector<int>> &box)
{
    QPointF sceneCenter ; // 将视图中心坐标转换为场景坐标
    //
    if (box.size() != 4) {
        // 确保box包含四个点
        return;
    }
    // 计算矩形的中心点
    double centerX = 0.00;
    double centerY = 0.00;
    for (const auto &point : box) {
        centerX += point[0];
        centerY += point[1];
    }
    centerX /= 4;
    centerY /= 4;
    centerX = centerX/originalPixmap.width()*view->width();
    centerY = centerY/originalPixmap.height()*view->height();
    qDebug()<<"center:"<<centerX<<" "<<centerY;
    // 将pixmap的坐标转换成场景坐标
    sceneCenter = view->mapToScene((int)centerX, (int)centerY);
    // 平移视图以使场景中心点位于视图中心
    view->centerOn(sceneCenter);
}
