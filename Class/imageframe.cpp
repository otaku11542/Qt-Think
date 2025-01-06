#include "imageframe.h"
#include "imagedialog.h"
//获取照片
ImageFrame::ImageFrame(const QPixmap &bgPixmap, const QString &str, QWidget *parent)
    :QFrame(parent), originalPixmap(bgPixmap), strs(str)
{
    setMouseTracking(true); // 启用鼠标跟踪
}
//获取左键双击
void ImageFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        showEnlargedImage();
    }
}
//展示放大的图片
void ImageFrame::showEnlargedImage()
{
    // Create and show the dialog
    ImageDialog* dialog = new ImageDialog(originalPixmap);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 确保对话框关闭时自动删除
    dialog->show(); // 显示对话框，不需要调用 exec()
}
