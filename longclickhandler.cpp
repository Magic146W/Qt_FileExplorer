#include "longclickhandler.h"
#include <QDebug>

/**
 * @file longclickhandler.h
 * @brief Defines the LongClickHandler class responsible for handling long clicks in a QListView.
 */

LongClickHandler::LongClickHandler(QListView *listView, QObject *parent) : QObject(parent), listView(listView)
{
    longClickTimer.setSingleShot(true);
    longClickTimer.setInterval(1000);

    connect(&longClickTimer, &QTimer::timeout, this, &LongClickHandler::handleLongClick);
    listView->viewport()->installEventFilter(this);
}

/**
* @brief Handles the timeout of the long click timer and emits the longClicked signal.
*/
void LongClickHandler::handleLongClick()
{
    QModelIndex index = listView->indexAt(listView->viewport()->mapFromGlobal(QCursor::pos()));
    if (index.isValid())
    {
        emit longClicked(index);
    }
}

/**
* @brief Event filter to detect mouse button presses and releases in the QListView's viewport.
* @param obj The watched object.
* @param event The event to be filtered.
* @return True if the event was handled; otherwise, false.
*/
bool LongClickHandler::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == listView->viewport())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                longClickTimer.start();
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            longClickTimer.stop();
        }
    }

    return QObject::eventFilter(obj, event);
}
