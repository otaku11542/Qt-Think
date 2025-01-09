#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include <QAbstractSlider>
#include <QScrollBar> // Include QScrollBar header
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsView>
#include <vector>
using namespace std;
class ImageWidget : public QGraphicsView {
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = nullptr);

    void loadPixmap(const QPixmap &pixmap);
public slots:
    void gotoRectangle(vector<vector<int>> box);

protected:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_imageItem;
    QPixmap m_originalPixmap;
    QPoint m_lastMousePos;
};

#endif // IMAGEWIDGET_H
