#include "OperatorsVariableInfo.h"
#include "DataWidget.h"
#include "NodeStructure.h"
#include "EditWidget.h"

KAVI_NS_BEGIN

using namespace XMLUtils;

OperatorsVariableInfo::OperatorsVariableInfo(DataWidget *data, const DataWidget *definition, int variableID, int definitionID, QWidget *parent)
    :InfoPanel(data, variableID, parent)
{
    ui.setupUi(this);

    xmlDefinition = definition;
    variableDef = xmlDefinition->findNode(definitionID);

    ui.variableName->setText(subelementTagValue(selectedNode, "label"));
    initComboBox();
}

void OperatorsVariableInfo::on_classComboBox_currentIndexChanged(QString newClass)
{
    QDomElement classEl = selectedNode.firstChildElement("class");
    setStrValue(classEl, newClass);

    emit madeChange(NodeClassChanged);
}

void OperatorsVariableInfo::initComboBox()
{
    QString currentClass = getStrValue(selectedNode.firstChildElement("class"));

    ui.classComboBox->addItems(xmlDefinition->getNodeLabelList(NST_CLASS));

    int currentIndex = ui.classComboBox->findText(currentClass);
    ui.classComboBox->setCurrentIndex(currentIndex);
}



KAVI_NS_END
