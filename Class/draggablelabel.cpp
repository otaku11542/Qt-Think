#include "draggablelabel.h"

DraggableLabel::DraggableLabel(QWidget *parent): QLabel(parent)
{
    setMouseTracking(true); // 启用鼠标跟踪
}

void DraggableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标按下的位置
        dragStartPos = event->pos();
        dragging = true;
    }
}

void DraggableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging) {
        // 计算新的尺寸
        QPoint delta = event->pos() - dragStartPos;
        QSize newSize = pixmap.size() + QSize(delta.x(), delta.y());
        // 限制最小尺寸
        newSize = newSize.expandedTo(QSize(10, 10));
        // 更新 QLabel 的尺寸
        setFixedSize(newSize);
        // 缩放并设置新的 QPixmap
        setPixmap(pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void DraggableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 停止拖动
        dragging = false;
    }
}

void DraggableLabel::wheelEvent(QWheelEvent *event) {
    // 获取鼠标位置相对于 QLabel 的位置
    QPoint pos = event->pos();
    // 计算缩放因子，这里使用更敏感的值，例如 1.1
    qreal zoomFactor = (event->angleDelta().y() > 0) ? 1.1 : 1 / 1.1;
    // 计算图片新的尺寸
    QSize newSize = pixmap.size() * zoomFactor;
    // 限制最小尺寸
    newSize = newSize.expandedTo(QSize(10, 10));

    // 计算缩放后鼠标应保持的相对位置
    QPoint delta = pos - rect().center();
    QPoint newDelta = delta * zoomFactor;

    // 更新 QLabel 的尺寸
    setFixedSize(newSize);
    // 缩放并设置新的 QPixmap
    setPixmap(pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 保持鼠标相对于图片的位置不变
    QPoint newCenter = rect().center() + newDelta;
    QPoint newTopLeft = newCenter - QPoint(rect().width() / 2, rect().height() / 2);

    // 移动 QLabel，使鼠标保持在图片上的相对位置不变
    move(newTopLeft);
}
