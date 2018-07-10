#ifndef PANTOOL_H
#define PANTOOL_H

#include "tool.h"

class Pan : public Tool
{
public:
    Pan(QWidget *parent=0);
    ~Pan();
    void move(QMouseEvent *event, QList<Layer*> &selectedLayers);
    void press(QMouseEvent *event);
    void release(QMouseEvent *event);
};

#endif // PANTOOL_H
