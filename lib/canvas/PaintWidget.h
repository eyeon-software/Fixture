#pragma once

#include <QAbstractScrollArea>

#include "FixtureCanvas.h"

namespace Fixture{
    class PaintWidget : public QAbstractScrollArea{
        Q_OBJECT
        public:
            PaintWidget(QWidget *parent);
        protected:
            void paintEvent(QPaintEvent *event);

        private:
            Fixture::Canvas *_canvas;
    };
}
