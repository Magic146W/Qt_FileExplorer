#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QObject>

class FileExplorer : public QObject
{
    Q_OBJECT
public:
    explicit FileExplorer(QObject *parent = nullptr);

signals:

};

#endif // FILEEXPLORER_H
