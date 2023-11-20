#ifndef TOOLTIPMANAGER_H
#define TOOLTIPMANAGER_H

#include <QObject>

class TooltipManager : public QObject
{
    Q_OBJECT
public:
    explicit TooltipManager(QObject *parent = nullptr);

signals:

};

#endif // TOOLTIPMANAGER_H
