#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include <QDebug>
#include "abstractselection.h"

class Transform : public AbstractSelection
{
public:
    Transform(QWidget *parent=0);
    ~Transform();
    void move(QMouseEvent *event);
    void press(QMouseEvent *event);
    void release(QMouseEvent *event);
private:
    int _curMousex, _curMousey,_prevMousex,_prevMousey;
    bool _leftClick,_firstMove;
};

#endif // SELECTTOOL_H
