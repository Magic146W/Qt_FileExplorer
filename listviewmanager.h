#ifndef LISTVIEWMANAGER_H
#define LISTVIEWMANAGER_H

#include "modifiedfilesystemmodel.h"
#include <QObject>
#include <QListView>

class ListViewManager : public QObject
{
    Q_OBJECT
public:
    static ListViewManager& instance();
    QString listViewSelectedItemPath(const QModelIndex &index);
    void initialize(QListView* listView);

private:
    ListViewManager();  // Private constructor to prevent instantiation
    ~ListViewManager();
    QListView* listView;

    ModifiedFileSystemModel* modifiedFileSystemModel;
    void handleRenaming(const QFileInfo &fileInfo, QLineEdit *lineEdit, const QString &originalFilename);
    void handleCancelEditing(QLineEdit *lineEdit, const QString &originalFilename);

signals:
    void updateViewData(const QString& path);
    void callFileViewerDialog(const QString& path, bool isImage);
    void shouldAcceptDirectories(bool acceptsDirectories);

public slots:
    void onListViewItemLongClicked(const QModelIndex &index);
    void onListViewItemDoubleClicked(const QModelIndex &index);
    void setModelForListView(const QString &path);

};

#endif // LISTVIEWMANAGER_H
