#ifndef LONGCLICKHANDLER_H
#define LONGCLICKHANDLER_H

#include <QObject>
#include <QListView>
#include <QMouseEvent>
#include <QTimer>

class LongClickHandler : public QObject {
    Q_OBJECT

public:
    explicit LongClickHandler(QListView *listView, QObject *parent = nullptr);

signals:
    void longClicked(const QModelIndex &index);

private slots:
    void handleLongClick();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QListView *listView;
    QTimer longClickTimer;
};

#endif // LONGCLICKHANDLER_H
