#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include <QMouseEvent>
#include "tool.h"
#include "../layers/layer.h"

class Transform : public Tool
{
public:
    Transform(QWidget *parent=0);
    ~Transform();
    void move(QMouseEvent *event, QList<Layer*> &selectedLayers);
    void press(QMouseEvent *event);
    void release(QMouseEvent *event);
private:
    int _curMousex, _curMousey,_prevMousex,_prevMousey;
    bool _leftClick,_firstMove;
};

#endif // SELECTTOOL_H
