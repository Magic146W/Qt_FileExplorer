#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QListView>
#include <QToolBar>
#include <QVBoxLayout>

class FileExplorer : public QObject
{
    Q_OBJECT
public:
    explicit FileExplorer(QObject *parent = nullptr);

signals:

};

#endif // FILEEXPLORER_H
