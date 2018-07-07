#include "selecttool.h"

SelectTool::SelectTool(QWidget* parent):
    Tool(QIcon(":/tools/select.svg"),"Select Tool",
         QCursor(QIcon(":/tools/select.svg").pixmap(QSize(15,15)),0,0),
         Tool::SelectTool,parent)
{

}

SelectTool::~SelectTool()
{

}
