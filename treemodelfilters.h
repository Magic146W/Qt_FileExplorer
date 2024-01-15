#ifndef TREEMODELFILTERS_H
#define TREEMODELFILTERS_H

#include <QFileSystemModel>

class TreeModelFilters : public QFileSystemModel {
    Q_OBJECT

public:
    explicit TreeModelFilters(QObject *parent = nullptr);

    bool hasChildren(const QModelIndex &parent) const override;
};


#endif // TREEMODELFILTERS_H
