#ifndef TOOL_H
#define TOOL_H

#include <QAction>

class Tool : public QAction
{
public:
    enum ToolType{
        SelectTool,
        PanTool
    };

    Tool(QIcon ico,const QString &name,QCursor cur,ToolType type,QWidget* parent = 0);
    ~Tool();

    inline const QCursor getToolCursor() { return _cursor; }
    inline const QString getToolName() { return _name; }
    inline ToolType getToolType() { return _type; }

private:
    QCursor _cursor;
    QString _name;
    ToolType _type;

};

#endif // TOOL_H
