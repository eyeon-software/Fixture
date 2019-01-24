#pragma once

#include <QOpenGLWidget>

namespace Fixture{
    class Canvas : public QOpenGLWidget{
        Q_OBJECT
        public:
            Canvas(QWidget *widget);
    };
}
