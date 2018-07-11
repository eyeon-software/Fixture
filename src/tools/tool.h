#ifndef TOOL_H
#define TOOL_H

#include <QAction>
#include "../layers/layer.h"
class Tool : public QAction
{
public:
    enum ToolType{
        SELECTION,
        CREATION,
        MODIFICATION,
        PERCEPTION
    };

    Tool(QIcon ico,const QString &name,QCursor cur,ToolType type,QWidget* parent = 0);
    ~Tool();

    inline const QCursor getToolCursor() { return _cursor; }
    inline const QString getToolName() { return _name; }
    inline ToolType getToolType() { return _type; }
    virtual void move(QMouseEvent *event, QList<Layer*> &selectedLayers) = 0;
    virtual void press(QMouseEvent *event) = 0;
    virtual void release(QMouseEvent *event) = 0;

private:
    QCursor _cursor;
    QString _name;
    ToolType _type;

};

#endif // TOOL_H
