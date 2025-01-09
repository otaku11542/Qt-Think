#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) : QGraphicsView(parent) {
    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    setTransformationAnchor(AnchorViewCenter);
    m_scene = new QGraphicsScene(this);
    m_imageItem = new QGraphicsPixmapItem();
    m_scene->addItem(m_imageItem);
    setScene(m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag); // Initialize drag mode to NoDrag
}

void ImageWidget::loadPixmap(const QPixmap &pixmap) {
    m_imageItem->setPixmap(pixmap);
    m_originalPixmap = pixmap;
    fitInView(m_imageItem, Qt::KeepAspectRatio);
    QRectF rect(m_originalPixmap.width()/2,
                0,
                width(), m_originalPixmap.height());
    fitInView(rect, Qt::KeepAspectRatio);
}

void ImageWidget::gotoRectangle(vector<vector<int>> box) {
    if(box.size()!=4){
        qWarning()<<__FUNCTION__<<__LINE__<<"坐标数量不足";
        return;
    }
    for(int i = 0;i<4;i++){
        if(box[i].size()!=2){
            qWarning()<<__FUNCTION__<<__LINE__<<"坐标信息不足";
            return;
        }
    }
    // 初始化最小和最大的 x 和 y 坐标
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();

    // 遍历坐标点，找到最小和最大的 x 和 y 坐标
    for (int i = 0; i < 4; ++i) {
        if (box[i][0] < minX) minX = box[i][0]; // 更新最小 x
        if (box[i][0] > maxX) maxX = box[i][0]; // 更新最大 x
        if (box[i][1] < minY) minY = box[i][1]; // 更新最小 y
        if (box[i][1] > maxY) maxY = box[i][1]; // 更新最大 y
    }
    int minx = minX>=0?minX:0;
    int miny = minY>=0?minY:0;
    int maxx = maxX<=m_originalPixmap.width()?maxX-minx:m_originalPixmap.width()-minx;
    int maxy = maxY<=m_originalPixmap.height()?maxY-miny:m_originalPixmap.height()-miny;
    QRectF rect(minx, miny, maxx, maxy);
    fitInView(rect, Qt::KeepAspectRatio);
}

void ImageWidget::wheelEvent(QWheelEvent *event) {
    QPoint scrollAmount = event->angleDelta();
    if (scrollAmount.y() > 0) {
        scale(1.1, 1.1);
    } else {
        scale(0.9, 0.9);
    }
}

void ImageWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        // Calculate how much the mouse has moved
        QPointF delta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
        // Scroll the view
        QAbstractSlider *hBar = horizontalScrollBar();
        QAbstractSlider *vBar = verticalScrollBar();
        hBar->setValue(hBar->value() - delta.x());
        vBar->setValue(vBar->value() - delta.y());
        // Update the last mouse position
        m_lastMousePos = event->pos();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setDragMode(QGraphicsView::NoDrag);
    }
}
