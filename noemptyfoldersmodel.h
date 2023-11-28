#ifndef NOEMPTYFOLDERSMODEL_H
#define NOEMPTYFOLDERSMODEL_H

#include <QFileSystemModel>

class NoEmptyFoldersModel : public QFileSystemModel {
    Q_OBJECT

public:
    explicit NoEmptyFoldersModel(QObject *parent = nullptr);

    bool hasChildren(const QModelIndex &parent) const override;
};

#endif // NOEMPTYFOLDERSMODEL_H

