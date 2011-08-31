#ifndef YAWNER_UI_VIEW_MESSAGEVIEW_MESSAGEITEM_H
#define YAWNER_UI_VIEW_MESSAGEVIEW_MESSAGEITEM_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QPixmap>
#include "Yammer/Message.h"

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {
            namespace MessageViewNS {

                class MessageItem : public QObject, public QGraphicsItem
                {
                    Q_OBJECT
                    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
                    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
                    Q_PROPERTY(qreal grayness READ grayness WRITE setGrayness)

                private:
                    YammerNS::Message *_message;

                    QRectF _boundingRect;

                    QImage *_bubble;

                    QGraphicsTextItem *_text;

                    QPropertyAnimation *_opacityAnimation;

                    QPropertyAnimation *_graynessAnimation;

                    QPointF _destination;

                    qreal _grayness;

                    QImage* _getBubble();

                    QString _filterText(QString rawText);

                public:
                    explicit MessageItem(QGraphicsView *parent = 0, QGraphicsScene *scene = 0);

                    virtual ~MessageItem();

                    YammerNS::Message* message();

                    void setMessage(YammerNS::Message *message);

                    QRectF boundingRect() const;

                    void setSize(qreal width, qreal height);

                    qreal width();

                    void setWidth(qreal width);

                    qreal height();

                    QPointF destination();

                    void setDestination(QPointF destination);

                    qreal grayness();

                    void setGrayness(qreal grayness);

                    void animateOpacity(qreal target, int duration = 100, QEasingCurve curve = QEasingCurve::Linear);

                    void animateGrayness(qreal target, int duration = 100, QEasingCurve curve = QEasingCurve::Linear);

                    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

                protected:
                    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
                    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
                    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
                    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

                signals:
                    void hoverEntered(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *item);
                    void hoverLeft(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *item);
                    void clicked(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem *item);

                public slots:

                };

                bool message_item_is_newer_than(MessageItem* left, MessageItem* right);
                bool message_item_is_older_than(MessageItem* left, MessageItem* right);
            }
        }
    }
}

#endif // YAWNER_UI_VIEW_MESSAGEVIEW_MESSAGEITEM_H
