#pragma once

#include <QAbstractScrollArea>

#include "FixtureCanvas.h"

namespace Fixture{
    class PaintWidget : public QAbstractScrollArea{
        Q_OBJECT
        public:
            PaintWidget(QWidget *parent);

        private:
            Fixture::Canvas *_canvas;
    };
}
