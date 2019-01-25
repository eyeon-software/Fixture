#pragma once

#include <QOpenGLWidget>
#include <QPaintEvent>

namespace Fixture{
    class PaintWidget;
    class Canvas : public QOpenGLWidget{
        Q_OBJECT
        public:
            Canvas(QWidget *widget);
        protected:
            void paintEvent(QPaintEvent *event);
        private:
            friend class Fixture::PaintWidget;
    };
}
