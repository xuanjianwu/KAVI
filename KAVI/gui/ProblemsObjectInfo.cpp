#include "ProblemsObjectInfo.h"
#include "DataWidget.h"
#include "NodeStructure.h"
#include "EditWidget.h"

KAVI_NS_BEGIN

using namespace XMLUtils;

ProblemsObjectInfo::ProblemsObjectInfo(DataWidget *data, const DataWidget *definition, int objectID, int definitionID, QWidget *parent)
    :InfoPanel(data, objectID, parent)
{
    ui.setupUi(this);

    xmlDefinition = definition;
    objectDef = xmlDefinition->findNode(definitionID);

    ui.objectName->setText(subelementTagValue(selectedNode, "label"));
    initComboBox();
}

void ProblemsObjectInfo::on_classComboBox_currentIndexChanged(QString newClass)
{
    QDomElement classEl = selectedNode.firstChildElement("class");
    setStrValue(classEl, newClass);

    emit madeChange(NodeClassChanged);
}

void ProblemsObjectInfo::initComboBox()
{
    QString currentClass = subelementTagValue(selectedNode, "class");

    ui.classComboBox->addItems(xmlDefinition->getNodeLabelList(NST_CLASS));

    int currentIndex = ui.classComboBox->findText(currentClass);
    ui.classComboBox->setCurrentIndex(currentIndex);
}



KAVI_NS_END
