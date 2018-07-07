#include "pantool.h"

PanTool::PanTool(QWidget *parent):
    Tool(QIcon(":/tools/pan.svg"),"Pan Tool",
         Qt::OpenHandCursor,Tool::PanTool,parent)
{

}

PanTool::~PanTool()
{

}
