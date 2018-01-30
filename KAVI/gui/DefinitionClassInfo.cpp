#include "DefinitionClassInfo.h"
#include "DataWidget.h"

KAVI_NS_BEGIN



DefinitionClassInfo::DefinitionClassInfo(DataWidget *data, int classID, QWidget *parent)
    :InfoPanel(data, classID, parent)
{
    ui.setupUi(this);

    ui.className->setText(subelementTagValue(selectedNode, "label"));
}



KAVI_NS_END
