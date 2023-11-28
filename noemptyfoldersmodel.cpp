#include "noemptyfoldersmodel.h"
#include <QDirIterator>

/**
 * \brief The NoEmptyFoldersModel class inherits QFileSystemModel and modifies the behavior for checking if folders have children.
 */

NoEmptyFoldersModel::NoEmptyFoldersModel(QObject *parent)
    : QFileSystemModel(parent) {}

/**
 * \brief Checks if the provided parent index in the file system model has children (folders with content).
 *
 * This function returns true if the parent item is not set as never having children and if the directory iterator indicates the presence of items.
 * It's utilized to eliminate unnecessary folder expansion options.
 *
 * \param parent The model index of the parent item to examine.
 * \return True if the parent item contains children (non-empty folders); otherwise, false.
 */
bool NoEmptyFoldersModel::hasChildren(const QModelIndex &parent) const {
    if (parent.flags() &  Qt::ItemNeverHasChildren) {
        return false;
    }

    return QDirIterator(
               filePath(parent),
               filter() | QDir::NoDotAndDotDot,
               QDirIterator::NoIteratorFlags
               ).hasNext();
}
