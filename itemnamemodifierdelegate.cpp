#include "itemnamemodifierdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QListView>

ItemNameModifierDelegate::ItemNameModifierDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    customSize = QSize(80, 100);
}

void ItemNameModifierDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    opt.textElideMode = Qt::ElideMiddle;

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
}

QSize ItemNameModifierDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    if (index.isValid())
    {
        return customSize;
    }

    return QSize(80, 100);
}

void ItemNameModifierDelegate::setCustomSize(const QSize& size)
{
    customSize = size;
}
