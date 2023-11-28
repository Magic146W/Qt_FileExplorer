#include "itemnamemodifierdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QListView>
/**
 * \brief The ItemNameModifierDelegate class customizes the appearance of items in a view, particularly the text display.
 */

/**
 * \brief Constructs an ItemNameModifierDelegate object with the specified parent and sets the default custom size.
 *
 * \param parent The parent QObject.
 */
ItemNameModifierDelegate::ItemNameModifierDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    customSize = QSize(80, 100);
}

/**
 * \brief Paints the item in the view using customized options.
 *
 * Modifies the style options for painting and then draws the item view.
 *
 * \param painter The QPainter object used for painting.
 * \param option The style options for the item view.
 * \param index The model index of the item.
 */
void ItemNameModifierDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    opt.textElideMode = Qt::ElideMiddle;

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
}

/**
 * \brief Returns the size hint for the item.
 *
 * Returns the custom size for the item view if the index is valid; otherwise, returns the default size.
 *
 * \param option The style options for the item view.
 * \param index The model index of the item.
 * \return The size hint for the item.
 */
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

/**
 * \brief Sets a custom size for the item view.
 *
 * \param size The custom size to be set.
 */
void ItemNameModifierDelegate::setCustomSize(const QSize& size)
{
    customSize = size;
}
