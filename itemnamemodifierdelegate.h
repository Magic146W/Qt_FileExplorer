#ifndef ITEMNAMEMODIFIERDELEGATE_H
#define ITEMNAMEMODIFIERDELEGATE_H

#include <QStyledItemDelegate>

class ItemNameModifierDelegate : public QStyledItemDelegate
{

public:
    explicit ItemNameModifierDelegate(QObject* parent = nullptr);
    void setCustomSize(const QSize& size);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;


private:
    QSize customSize;
};

#endif // ITEMNAMEMODIFIERDELEGATE_H
