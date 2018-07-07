#include "tool.h"

Tool::Tool(QIcon ico, const QString &name, QCursor cur, ToolType type, QWidget* parent):
    QAction(ico,name,parent), _cursor(cur),_name(name),_type(type)
{
    setCheckable(true);
}

Tool::~Tool()
{

}
