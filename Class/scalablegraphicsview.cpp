#include "Class/scalablegraphicsview.h"
#include <QtMath> // Include cmath for qPow
ScalableGraphicsView::ScalableGraphicsView(QWidget *parent)
 : QGraphicsView(parent), scaleFactor(1.0)
{}

void ScalableGraphicsView::wheelEvent(QWheelEvent *event)
{
    // Calculate the zoom factor
    double angle = event->angleDelta().y();
    double factor = qPow(1.0015, angle);
    scaleFactor *= factor;
    // Perform the zoom
    scale(factor, factor);
    // Adjust the scrollbars
    QScrollBar *hBar = horizontalScrollBar();
    QScrollBar *vBar = verticalScrollBar();
    int hValue = (hBar->value() + event->pos().x()) * factor - event->pos().x();
    int vValue = (vBar->value() + event->pos().y()) * factor - event->pos().y();
    hBar->setValue(hValue);
    vBar->setValue(vValue);
}

void ScalableGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标按下的位置
        lastDragPos = event->pos();
        dragging = true;
    }
}

void ScalableGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging) {
        // 计算鼠标移动的偏移量
        QPointF delta = mapToScene(event->pos()) - mapToScene(lastDragPos);
        // 移动图片项
        foreach (QGraphicsItem *item, scene()->items()) {
            item->setPos(item->pos() + delta);
        }
        // 更新上一次拖动位置
        lastDragPos = event->pos();
    }
}


void ScalableGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 停止拖动
        dragging = false;
    }
}
