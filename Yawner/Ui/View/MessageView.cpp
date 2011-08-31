#include "MessageView.h"
#include <QStyleOption>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QScrollBar>
#include "Yawner.h"

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            MessageView::MessageView(QWidget *parent) :
                QGraphicsView(parent), _viewMode(Feed), _currentThreadId(0), _messagesItems()
            {
                setScene(new QGraphicsScene(this));
                setInteractive(true);
                setMouseTracking(true);
            }

            void MessageView::init()
            {
                QMap<int, YammerNS::Message*> messageMap = Yawner::getInstance()->getMessageManager()->getMessageIndex();
                QMapIterator<int, YammerNS::Message*> it(messageMap);
                while (it.hasNext()) {
                    it.next();
                    YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *item =
                            new YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem(this);
                    connect(
                        item,
                        SIGNAL(hoverEntered(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*)),
                        this,
                        SLOT(messageItemHoverEntered(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*))
                    );
                    connect(
                        item,
                        SIGNAL(hoverLeft(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*)),
                        this,
                        SLOT(messageItemHoverLeft(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*))
                    );
                    connect(
                        item,
                        SIGNAL(clicked(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*)),
                        this,
                        SLOT(messageItemClicked(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*))
                    );
                    item->setMessage(it.value());
                    item->setPos(scene()->sceneRect().topLeft());
                    item->setWidth(scene()->sceneRect().width());
                    item->setOpacity(0);
                    scene()->addItem(item);
                    _messagesItems.append(item);
                }

                qSort(
                    _messagesItems.begin(),
                    _messagesItems.end(),
                    YawnerNS::UiNS::ViewNS::MessageViewNS::message_item_is_newer_than
                );

                layoutItems(&_messagesItems);
            }

            void MessageView::layoutItems(QList<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> *items)
            {
                QListIterator<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> it(*items);
                qreal top;
                qreal bottom;
                int i = 0;
                while (it.hasNext()) {
                    QPointF dest = scene()->sceneRect().topLeft();
                    if (it.hasPrevious()) {
                        YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *prev = it.peekPrevious();
                        dest = QPointF(
                            prev->destination().x(),
                            prev->destination().y() + prev->height()
                        );
                        top = std::min(top, prev->destination().y());
                    }
                    else {
                        bottom = top = dest.y();
                    }

                    YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *item = it.next();
                    item->setZValue(i++);
                    bottom = std::max(bottom, dest.y() + item->height());
                    item->setDestination(dest);

                    QPropertyAnimation *anim = new QPropertyAnimation(item, "pos");
                    anim->setDuration(500);
                    anim->setStartValue(item->pos());
                    anim->setEndValue(item->destination());
                    anim->setEasingCurve(QEasingCurve::OutExpo);
                    anim->start(QAbstractAnimation::DeleteWhenStopped);

                    QPropertyAnimation *anim2 = new QPropertyAnimation(item, "opacity");
                    anim2->setDuration(1000);
                    anim2->setStartValue(item->opacity());
                    anim2->setEndValue(1);
                    anim2->setEasingCurve(QEasingCurve::OutExpo);
                    anim2->start(QAbstractAnimation::DeleteWhenStopped);
                }
                setSceneRect(sceneRect().left(), top, sceneRect().width(), bottom - top);
            }

            void MessageView::messageItemHoverEntered(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem* item)
            {
                if (_viewMode == Feed) {
                    _viewMode = ThreadPreview;
                    QListIterator<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> it(_messagesItems);
                    while (it.hasNext()) {
                        if (it.peekNext()->message()->getThreadStarterId() == item->message()->getThreadStarterId()) {
                            it.peekNext()->animateGrayness(0);
                            it.next()->animateOpacity(1);
                        }
                        else {
                            it.peekNext()->animateGrayness(0.9);
                            it.next()->animateOpacity(0.9);
                        }
                    }
                }
            }

            void MessageView::messageItemHoverLeft(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem* item)
            {
                if (_viewMode == ThreadPreview) {
                    _viewMode = Feed;
                    QListIterator<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> it(_messagesItems);
                    while (it.hasNext()) {
                        it.peekNext()->animateGrayness(0);
                        it.next()->animateOpacity(1);
                    }
                }
            }

            void MessageView::messageItemClicked(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *item)
            {
                if (_viewMode != Thread || _currentThreadId != item->message()->getThreadStarterId()) {
                    _currentThreadId = item->message()->getThreadStarterId();
                    _viewMode = Thread;
                    QList<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> threadList;
                    QListIterator<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> all(_messagesItems);
                    all.toBack();
                    while (all.hasPrevious()) {
                        if (all.peekPrevious()->message()->getThreadStarterId() == _currentThreadId) {
                            threadList.append(all.previous());
                        }
                        else {
                            all.previous()->animateOpacity(0);
                        }
                    }
                    qSort(
                        threadList.begin(),
                        threadList.end(),
                        YawnerNS::UiNS::ViewNS::MessageViewNS::message_item_is_older_than
                    );
                    layoutItems(&threadList);
                }
                else {
                    _viewMode = Feed;
                    layoutItems(&_messagesItems);
                }
            }

            void MessageView::setWidth(int width)
            {
                setSceneRect(sceneRect().x(), sceneRect().y(), width, sceneRect().height());
                QListIterator<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> it(_messagesItems);
                while (it.hasNext()) {
                    it.next()->setWidth(width - std::min(4, verticalScrollBar()->width()));
                }
                layoutItems(&_messagesItems);
            }
        }
    }
}
