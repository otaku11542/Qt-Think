#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QtMath>
class DraggableLabel : public QLabel
{
public:
    DraggableLabel(QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override ;
    void mouseMoveEvent(QMouseEvent *event) override ;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QPixmap pixmap; // 存储 QPixmap
    QPoint dragStartPos; // 鼠标按下的位置
    bool dragging = false; // 是否正在拖动
};

#endif // DRAGGABLELABEL_H
