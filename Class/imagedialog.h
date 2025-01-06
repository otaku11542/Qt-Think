#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H
#include <QLabel>
#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
namespace Ui {
class ImageDialog;
}

class ImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDialog(const QPixmap &pixmap, QWidget *parent = nullptr) ;
    ~ImageDialog();
private:
    QGraphicsView *view;
    qreal scaleFactor;
private:
    Ui::ImageDialog *ui;
};

#endif // IMAGEDIALOG_H
