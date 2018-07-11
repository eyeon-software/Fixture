#ifndef ABSTRACTSELECTION_H
#define ABSTRACTSELECTION_H

#include "tool.h"
#include "../layers/layer.h"

#include <QMouseEvent>

class AbstractSelection : public virtual Tool
{
    Q_OBJECT

public:
    AbstractSelection(QWidget *parent=0);

public:
    inline void setLayers(QList<Layer*> &selectedLayers) { _layers = &selectedLayers; }

protected:
    QList<Layer*> *_layers;

};

#endif // ABSTRACTSELECTION_H
