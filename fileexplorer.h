#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QListView>
#include <QToolBar>
#include <QVBoxLayout>
#include <QFileSystemModel>

class FileExplorer : public QObject
{
    Q_OBJECT

public:
    static FileExplorer* getInstance(QObject *parent = nullptr);
    void setupFileSystemModel(const QString& selectedDirectory = QString());


private:
    explicit FileExplorer(QObject *parent = nullptr);
    static FileExplorer* instance;
    QFileSystemModel *fileSystemModel;


signals:
    void SelectionDone(QFileSystemModel *model);
    void FileSelected(const QString& filePath);


public slots:
    void OnTreeViewItemClicked(const QModelIndex &index);
    void OnListViewItemClicked(const QModelIndex &index);
};

#endif // FILEEXPLORER_H
