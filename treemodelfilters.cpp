#include "treemodelfilters.h"
#include <QDirIterator>

/**
 * @file treemodelfilters.h
 * \brief The class inherits QFileSystemModel and modifies the behavior for checking if folders have children.
 */

TreeModelFilters::TreeModelFilters(QObject *parent)
    : QFileSystemModel(parent) {}

/**
 * \brief Checks if the provided parent index in the file system model has children (folders with content).
 *
 * \param parent The model index of the parent item to examine.
 * \return True if the parent item contains children (non-empty folders); otherwise, false.
 */
bool TreeModelFilters::hasChildren(const QModelIndex &parent) const
{
    if (parent.flags() &  Qt::ItemNeverHasChildren)
    {
        return false;
    }

    return QDirIterator(
               filePath(parent),
               filter() | QDir::NoDotAndDotDot,
               QDirIterator::NoIteratorFlags
               ).hasNext();
}
