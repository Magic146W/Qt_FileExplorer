#include "treeviewmanager.h"
#include "treemodelfilters.h"
#include <QTreeView>
#include <QModelIndex>
#include <QHeaderView>
#include <QTimer>

/**
  @file treeviewmanager.h
* @brief Manages the interaction and behavior of the QListView component within the application.
*/

TreeViewManager::TreeViewManager()
{

}

void TreeViewManager::initialize(QTreeView* treeView)
{
    this -> treeView = treeView;
}

TreeViewManager::~TreeViewManager()
{}

TreeViewManager& TreeViewManager::instance()
{
    static TreeViewManager instance;

    if (instance.modelWithTreeModelFilters == nullptr)
    {
        instance.modelWithTreeModelFilters = new TreeModelFilters;
        instance.modelWithTreeModelFilters->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        instance.modelWithTreeModelFilters->setRootPath("");
    }

    return instance;
}

/**
     * @brief Sets the model for the specified QTreeView.
     *
     * @param treeView The QTreeView for which to set the model.
     */
void TreeViewManager::setModelForTreeView(QTreeView *treeView)
{
    treeView->setModel(modelWithTreeModelFilters);
}

/**
 * \brief Updates the tree view with the file system model based on provided path.
 *
 * \param treeView The QTreeView based in MainWindow.
 * \param path The path to initialize file system model.
 */
void TreeViewManager::updateModelForTreeView(const QString &path)
{
    if (modelWithTreeModelFilters == nullptr)
    {
        return;
    }

    treeView->collapseAll();

    QModelIndex index = modelWithTreeModelFilters->index(path);
    if (index.isValid())
    {
        treeView->expand(index);
        treeView->setCurrentIndex(index);
        QTimer::singleShot(200, this, [=]()
                           {
                               treeView->scrollTo(index, QAbstractItemView::PositionAtCenter);
                           });
    }
}

/**
 * \brief Retrieves the path of the selected item in the tree view.
 *
 * \return The path of the selected item in the tree view.
 */
QString TreeViewManager::treeViewSelectedItemPath(const QModelIndex &index)
{
    if (index.isValid())
    {
        QVariant data = index.data(QFileSystemModel::FilePathRole);
        if (data.isValid())
        {
            QString directoryPath = data.toString();
            emit updateViewData(directoryPath);
            return directoryPath;
        }
    }
    return "";
}

/**
     * @brief Handles the expansion of a tree view index by scrolling to it and positioning at the center.
     *
     * @param index The QModelIndex of the item to be expanded.
     */
void TreeViewManager::onTreeViewIndexExpanded(const QModelIndex &index)
{
    treeView->scrollTo(index, QAbstractItemView::PositionAtCenter);
}
