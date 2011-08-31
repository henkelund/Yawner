#include "MessageItem.h"
#include <QPainter>
#include <QTextDocument>
#include <QGraphicsSceneMouseEvent>
#include "Yawner.h"

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {
            namespace MessageViewNS {

                bool message_item_is_newer_than(MessageItem* left, MessageItem* right)
                {
                    return left->message()->isNewerThan(right->message());
                }

                bool message_item_is_older_than(MessageItem* left, MessageItem* right)
                {
                    return left->message()->isOlderThan(right->message());
                }

                MessageItem::MessageItem(QGraphicsView *parent, QGraphicsScene *scene) :
                    QObject(parent),
                    QGraphicsItem(0, scene),
                    _message(0),
                    _boundingRect(0, 0, 0, 0),
                    _bubble(0),
                    _text(new QGraphicsTextItem(this, scene)),
                    _opacityAnimation(new QPropertyAnimation(this, "opacity")),
                    _graynessAnimation(new QPropertyAnimation(this, "grayness")),
                    _grayness(0.0)
                {
                    setAcceptHoverEvents(true);
                    setAcceptedMouseButtons(Qt::LeftButton);
                    setFlag(QGraphicsItem::ItemIsSelectable);
                    _text->setTextInteractionFlags(Qt::TextSelectableByMouse);
                    QFont font = _text->font();
                    font.setPixelSize(11);
                    _text->setFont(font);
                    _text->setPos(66, 10);
                    _destination = QPointF(x(), y());
                }

                MessageItem::~MessageItem()
                {
                    if (_bubble != 0) {
                        delete _bubble;
                    }
                    delete _opacityAnimation;
                    delete _graynessAnimation;
                }

                YammerNS::Message* MessageItem::message()
                {
                    return _message;
                }

                void MessageItem::setMessage(YammerNS::Message *message)
                {
                    _message = message;
                    _text->setHtml(_filterText(message->getText()));
                    setSize(width(), height());
                }

                QRectF MessageItem::boundingRect() const
                {
                    return _boundingRect;
                }

                void MessageItem::setSize(qreal width, qreal height)
                {
                    _text->setTextWidth(width - 76);
                    prepareGeometryChange();
                    _boundingRect.setWidth(width);
                    _boundingRect.setHeight(
                        std::max(
                            68, // absolute minimum
                            std::max(
                                (int) height, // requested
                                (int) _text->boundingRect().height() + 20) // text minimum
                            )
                        );
                }

                qreal MessageItem::width()
                {
                    return boundingRect().width();
                }

                void MessageItem::setWidth(qreal width)
                {
                    setSize(width, 0);
                }

                qreal MessageItem::height()
                {
                    return boundingRect().height();
                }

                QPointF MessageItem::destination()
                {
                    return _destination;
                }

                void MessageItem::setDestination(QPointF destination)
                {
                    _destination = destination;
                }

                qreal MessageItem::grayness()
                {
                    return _grayness;
                }

                void MessageItem::setGrayness(qreal grayness)
                {
                    _grayness = grayness;
                    update();
                }

                void MessageItem::animateOpacity(qreal target, int duration, QEasingCurve curve)
                {
                    if (_opacityAnimation->state() != QAbstractAnimation::Stopped) {
                        _opacityAnimation->stop();
                    }
                    _opacityAnimation->setDuration(duration);
                    _opacityAnimation->setEasingCurve(curve);
                    _opacityAnimation->setStartValue(opacity());
                    _opacityAnimation->setEndValue(std::max(std::min(target, 1.0), 0.0));
                    _opacityAnimation->start(QAbstractAnimation::KeepWhenStopped);
                }

                void MessageItem::animateGrayness(qreal target, int duration, QEasingCurve curve)
                {
                    if (_graynessAnimation->state() != QAbstractAnimation::Stopped) {
                        _graynessAnimation->stop();
                    }
                    _graynessAnimation->setDuration(duration);
                    _graynessAnimation->setEasingCurve(curve);
                    _graynessAnimation->setStartValue(grayness());
                    _graynessAnimation->setEndValue(std::max(std::min(target, 1.0), 0.0));
                    _graynessAnimation->start(QAbstractAnimation::KeepWhenStopped);
                }

                QImage* MessageItem::_getBubble()
                {
                    qreal bw = 2; // border width
                    qreal br = 8; // border radius
                    qreal aw = 10; // arrow width
                    int width = _text->boundingRect().width() + 2*bw + aw;
                    int height = std::max(48, (int) _text->boundingRect().height()) + 2*bw;
                    if (_bubble == 0 || (_bubble->width() != width || _bubble->height() != height)) {
                        if (_bubble != 0) {
                            delete _bubble;
                        }
                        _bubble = new QImage(width, height, QImage::Format_ARGB32_Premultiplied);
                        _bubble->fill(0);
                        QPainter painter(_bubble);
                        painter.setRenderHint(QPainter::Antialiasing, true);
                        QPainterPath darkPath;
                        darkPath.moveTo(aw, 10);
                        darkPath.quadTo(aw, 17, 0, 24);
                        darkPath.quadTo(aw, 31, aw, 38);
                        darkPath.lineTo(aw, 10);
                        darkPath.addRoundedRect(aw, 0, width - aw, height, br, br, Qt::AbsoluteSize);
                        QColor darkColor(0x98, 0xAF, 0x11);
                        painter.fillPath(darkPath, QBrush(darkColor));
                        QPainterPath lightPath;
                        qreal awbw = aw + bw;
                        lightPath.moveTo(awbw, 10);
                        lightPath.quadTo(awbw, 18, bw*1.5, 24);
                        lightPath.quadTo(awbw, 30, awbw, 38);
                        lightPath.lineTo(awbw, 10);
                        lightPath.addRoundedRect(
                            awbw, // x-start = arrow width + border width
                            bw,   // y-start = border width
                            width - (aw + 2*bw), // width = full width - (arrow + 2 borders)
                            height - 2*bw, // height = full height - 2 borders
                            br - bw/2, // radius = slightly smaller than dark radius
                            br - bw/2,
                            Qt::AbsoluteSize
                        );
                        QColor lightColor(0xd2ff00);
                        painter.fillPath(lightPath, QBrush(lightColor));
                        QLinearGradient gradient(0, 0, 0, height);
                        gradient.setColorAt(0.0, QColor(255, 255, 255, 128));
                        gradient.setColorAt(0.85, Qt::transparent);
                        gradient.setColorAt(1.0, QColor(0, 0, 0, 48));
                        painter.fillPath(lightPath, QBrush(gradient));
                    }
                    return _bubble;
                }

                QString MessageItem::_filterText(QString rawText)
                {
                    rawText = rawText
                            .replace("&", "&amp;")
                            .replace("<", "&lt;")
                            .replace(">", "&gt;")
                            .replace("\"", "&quot;")
                            .replace("'", "&apos;")
                            .replace("  ", "&nbsp;&nbsp;");

                    QRegExp linkPattern("((https?|ftp):(\\/\\/)+[\\w\\d:#@%/;$()~_?\\+-=\\\\\\.&]*)");
                    QStringList linkMatches;
                    int pos = 0;

                    while ((pos = linkPattern.indexIn(rawText, pos)) != -1) {
                        QString link = linkPattern.cap(1);
                        if (!linkMatches.contains(link)) {
                            linkMatches.append(link);
                        }
                        pos += linkPattern.matchedLength();
                    }

                    QStringListIterator linkIt(linkMatches);
                    while (linkIt.hasNext()) {
                        QString link(linkIt.next());
                        rawText = rawText.replace(
                            link,
                            QString("<a style=\"color: #008b9e; font: 700 10px; text-decoration: none;\" href=\"%1\">%1</a>").arg(link)
                         );
                    }

                    QRegExp userPattern("\\[\\[user:([0-9]+)\\]\\]");
                    QStringList userMatches;
                    pos = 0;

                    while ((pos = userPattern.indexIn(rawText, pos)) != -1) {
                        userMatches.append(userPattern.cap(1));
                        pos += userPattern.matchedLength();
                    }

                    QStringListIterator it(userMatches);
                    while (it.hasNext()) {
                        int uid = it.next().toInt();
                        if (uid > 0) {
                            YammerNS::User *user = Yawner::getInstance()->getUserManager()->getUserById(uid);
                            rawText = rawText.replace(
                                QString("[[user:%1]]").arg(uid),
                                QString("<a style=\"color: #008b9e; font: 700 10px; text-decoration: none;\" href=\"user:%1\">%2</a>")
                                        .arg(QString::number(uid), user->getName())
                             );
                        }
                    }

                    QString userName = message()->getUser()->getName();

                    if (!userName.isEmpty()) {
                        rawText.prepend(QString("<a style=\"color: #FF5800; font: 700 10px; text-decoration: none;\" href=\"user:%1\">%2</a>: ")
                                        .arg(QString::number(message()->getUser()->getId()), userName));
                    }

                    return rawText.replace(QString("\n"), QString("<br />"));
                }

                void MessageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
                {
                    if (_message == 0) {
                        setVisible(false);
                        return;
                    }

                    QPainterPath bg;
                    bg.addRoundedRect(0, 0, width(), height(), 16, 16, Qt::AbsoluteSize);
                    painter->setRenderHint(QPainter::Antialiasing, true);
                    painter->fillPath(bg, QColor(0x008b9e));
                    painter->setRenderHint(QPainter::Antialiasing, false);

                    // construct a transparent image
                    QImage image(boundingRect().size().toSize(),
                                 QImage::Format_ARGB32_Premultiplied);
                    image.fill(0);

                    // construct a dedicated offline painter for this image
                    QPainter imagePainter(&image);
                    imagePainter.translate(-boundingRect().topLeft());

                    imagePainter.drawImage(
                        _text->pos().x() - 12, // aw + bw
                        _text->pos().y() - 2, // bw
                        (*_getBubble())
                    );

                    QPixmap avatar = _message->getUser()->getSmallImage();
                    QBrush brush(avatar);
                    QPixmap roundedAvatar(avatar.width(), avatar.height());
                    roundedAvatar.fill(Qt::transparent);
                    QPainter aPainter;
                    aPainter.begin(&roundedAvatar);
                    QPainterPath aRoundRect;
                    aRoundRect.addRoundedRect(0, 0, roundedAvatar.width(), roundedAvatar.height(), 15, 15, Qt::AbsoluteSize);
                    aPainter.setRenderHint(QPainter::Antialiasing, true);
                    aPainter.fillPath(aRoundRect, brush);
                    aPainter.setRenderHint(QPainter::Antialiasing, false);
                    aPainter.end();

                    imagePainter.drawPixmap(2, 2, roundedAvatar);

                    QLinearGradient gradient(0, 0, 0, height());
                    gradient.setColorAt(0.0, QColor(0, 0, 0, 32));
                    gradient.setColorAt(0.20, Qt::transparent);
                    gradient.setColorAt(0.80, Qt::transparent);
                    gradient.setColorAt(1.0, QColor(255, 255, 255, 24));

                    QPainterPath roundRect;
                    roundRect.addRoundedRect(2, 2, width() - 4, height() - 4, 15, 15, Qt::AbsoluteSize);

                    imagePainter.setRenderHint(QPainter::Antialiasing, true);
                    imagePainter.fillPath(roundRect, gradient);
                    imagePainter.setRenderHint(QPainter::Antialiasing, false);

                    imagePainter.end();

                    qreal grayness = this->grayness();
                    if (grayness > 0.01) {
                        int x, y, gray;
                        QRgb color;
                        qreal invGrayness = 1 - grayness;

                        for (int y = 0; y < image.height(); ++y) {
                            for (int x = 0; x < image.width(); ++x) {
                                color = image.pixel(x, y);
                                gray = qGray(color);
                                image.setPixel(
                                    x, y,
                                    qRgba(
                                        (invGrayness * qRed(color)) + (grayness * gray),
                                        (invGrayness * qGreen(color)) + (grayness * gray),
                                        (invGrayness * qBlue(color)) + (grayness * gray),
                                        qAlpha(color)
                                        )
                                    );
                            }
                        }
                    }

                    painter->drawImage(boundingRect(), image);
                }

                void MessageItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
                {
                    emit hoverEntered(this);
                }

                void MessageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
                {
                    emit hoverLeft(this);
                }

                void MessageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
                {
                    //qDebug("tj");
                    QGraphicsItem::mousePressEvent(event);
                }

                void MessageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
                {
                    if ((event->pos() - event->buttonDownPos(Qt::LeftButton)).manhattanLength() < 3) {
                        emit clicked(this);
                    }
                    else {
                        //qDebug("drag");
                    }
                    QGraphicsItem::mouseReleaseEvent(event);
                }
            }
        }
    }
}
