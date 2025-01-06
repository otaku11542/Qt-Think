#include "imagelabel.h"
#include "Class/imagedialog.h"
ImageLabel::ImageLabel(const QPixmap &bgPixmap, QWidget *parent) :
    QLabel(parent), originalPixmap(bgPixmap)
{
    setMouseTracking(true); // 启用鼠标跟踪
    setScaledContents(true); // 让 QLabel 自动缩放内容以适应大小
    setPixmap(originalPixmap);
}

void ImageLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        showEnlargedImage();
    }
}

void ImageLabel::showEnlargedImage()
{
    // Create and show the dialog
    ImageDialog* dialog = new ImageDialog(originalPixmap);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 确保对话框关闭时自动删除
    dialog->show(); // 显示对话框，不需要调用 exec()
}
