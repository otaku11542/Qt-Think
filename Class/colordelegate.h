#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class ColorDelegate : public QStyledItemDelegate
{
public:
    explicit ColorDelegate(QObject *parent = nullptr);
    //设置表格双色
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // COLORDELEGATE_H
