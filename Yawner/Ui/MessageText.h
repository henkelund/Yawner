#ifndef MESSAGETEXT_H
#define MESSAGETEXT_H

#include <QWidget>

class MessageText : public QWidget
{
    Q_OBJECT
public:
    explicit MessageText(QWidget *parent = 0);

signals:

public slots:
    void paintEvent(QPaintEvent *e);

};

#endif // MESSAGETEXT_H
