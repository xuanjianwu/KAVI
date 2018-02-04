#include "DefinitionClassInfo.h"
#include "DataWidget.h"





DefinitionClassInfo::DefinitionClassInfo(DataWidget *data, int classID, QWidget *parent)
    :InfoPanel(data, classID, parent)
{
    ui.setupUi(this);

    ui.className->setText(subelementTagValue(selectedNode, "label"));
}




