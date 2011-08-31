#ifndef YAWNER_UI_VIEW_MESSAGEVIEW_H
#define YAWNER_UI_VIEW_MESSAGEVIEW_H

#include <QGraphicsView>
#include "Yawner/Ui/View/MessageView/MessageItem.h"

namespace YawnerNS {
    namespace UiNS {
        namespace ViewNS {

            class MessageView : public QGraphicsView
            {
                Q_OBJECT
            private:
                enum ViewMode { Feed, Thread, ThreadPreview };

                ViewMode _viewMode;

                int _currentThreadId;

                QList<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> _messagesItems;

            public:
                explicit MessageView(QWidget *parent = 0);

                void init();

                void layoutItems(QList<YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem*> *items);

                void setWidth(int width);

            public slots:
                void messageItemHoverEntered(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem* item);
                void messageItemHoverLeft(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem* item);
                void messageItemClicked(YawnerNS::UiNS::ViewNS::MessageViewNS::MessageItem* item);

            };

        }
    }
}

#endif // YAWNER_UI_VIEW_MESSAGEVIEW_H
