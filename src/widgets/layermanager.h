#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QListWidget>
#include <QEvent>
#include <QFile>
#include <QPainter>

#include "../layers/rasterlayer.h"

class LayerManager : public QListWidget
{
    Q_OBJECT
public:
    LayerManager(QWidget *parent=0);
    ~LayerManager();

    void updateItems(QList<Layer*> items);
    inline QList<Layer*> getitems() { return _curItems; }

signals:
    void itemschanged();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    QList<Layer*> _curItems;
};

#endif // LAYERMANAGER_H
