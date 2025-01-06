#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QEnterEvent>
class ImageLabel : public QLabel
{
public:
    ImageLabel(const QPixmap &bgPixmap, QWidget *parent = nullptr);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    QPixmap originalPixmap;
    void showEnlargedImage();
};

#endif // IMAGELABEL_H
