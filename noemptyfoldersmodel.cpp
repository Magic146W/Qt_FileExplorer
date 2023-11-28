#include "noemptyfoldersmodel.h"
#include <QDirIterator>

NoEmptyFoldersModel::NoEmptyFoldersModel(QObject *parent)
    : QFileSystemModel(parent) {}

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
