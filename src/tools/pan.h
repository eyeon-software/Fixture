#ifndef PANTOOL_H
#define PANTOOL_H

#include "abstractperception.h"

class Pan : public AbstractPerception
{
public:
    Pan(QWidget *parent=0);
    ~Pan();
    void move(QMouseEvent *event);
    void press(QMouseEvent *event);
    void release(QMouseEvent *event);
};

#endif // PANTOOL_H
