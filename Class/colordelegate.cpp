#include "colordelegate.h"

ColorDelegate::ColorDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}
//设置表格双色
void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 设置背景颜色，两种颜色循环
    QColor color = (index.row() % 2) ? QColor(202, 220, 204) : QColor(240, 240, 240); // 使用整数表示颜色
    painter->fillRect(option.rect, color);
    // 绘制其他内容
    QStyledItemDelegate::paint(painter, option, index);
}
