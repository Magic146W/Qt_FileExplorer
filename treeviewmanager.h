#ifndef TREEVIEWMANAGER_H
#define TREEVIEWMANAGER_H

#include "treemodelfilters.h"
#include <QObject>
#include <QTreeView>

class TreeViewManager : public QObject
{
    Q_OBJECT

public:
    static TreeViewManager& instance();
    void setModelForTreeView(QTreeView *treeView);
    void initialize(QTreeView* treeView);

    TreeViewManager();
    ~TreeViewManager();
    QTreeView *treeView;

    TreeModelFilters *modelWithTreeModelFilters;
    bool hasChildren(const QModelIndex &parent) const;

signals:
    void updateViewData(const QString &path);

public slots:
    void updateModelForTreeView(const QString &path);
    QString treeViewSelectedItemPath(const QModelIndex &index);
    void onTreeViewIndexExpanded(const QModelIndex &index);
};

#endif // TREEVIEWMANAGER_H
