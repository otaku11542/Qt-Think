#ifndef SCALABLEGRAPHICSVIEW_H
#define SCALABLEGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsItem>
class ScalableGraphicsView : public QGraphicsView
{
public:
    ScalableGraphicsView(QWidget *parent = nullptr);
    void wheelEvent(QWheelEvent *event) override;
protected:
    void mousePressEvent(QMouseEvent *event) override ;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    qreal scaleFactor;
    QPoint lastDragPos; // 鼠标上一次拖动的位置
    bool dragging = false; // 是否正在拖动
};

#endif // SCALABLEGRAPHICSVIEW_H
