#include "OperatorsEdit.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "XMLUtils.h"
#include "InfoPanel.h"
#include "OperatorsVariableDialog.h"
#include "OperatorsPredicateDialog.h"
#include "OperatorsPredicateInfo.h"
#include "OperatorsVariableInfo.h"

KAVI_NS_BEGIN

using namespace XMLUtils;

OperatorsEdit::OperatorsEdit(QWidget *parent)
    :DrivenEditWidget(parent)
{
    allowedEdgeMask = DEP_ASSOCIATION;
    allowedNodeMask = NST_VARIABLE | NST_PREDICATE;

    xmlDefinition = (DataWidget*)NULL;
}

void OperatorsEdit::defineRectangleNode(QPointF pos, int newID)
{
    Q_ASSERT(xmlDefinition);

    // get the available classes from the definition
    QStringList availableClasses = xmlDefinition->getNodeLabelList(NST_CLASS);

    if ( availableClasses.count() == 0 )
    {
        QMessageBox::information(this, tr("VIZ"),
        tr("Variable have to belong to some class - there are no class defined yet."));
        return;
    }

    OperatorsVariableDialog * dialog = new OperatorsVariableDialog(availableClasses, this);

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->variableName().isEmpty() )
        {
            QMessageBox::information(this, tr("VIZ"), tr("Variable name can't be empty."));
            delete dialog;
            return;
        }

        if ( !nameChecker.exactMatch(dialog->variableName()) )
        {
            QMessageBox::warning(this, tr("VIZ"),
            tr("Name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
            delete dialog;
            return;
        }

        QStringList definedVariables = xmlData->getNodeLabelList(NST_VARIABLE);
        if ( definedVariables.contains(dialog->variableName(), Qt::CaseInsensitive) )
        {
            QMessageBox::warning(this, tr("VIZ"), tr("Variable name must be unique in action scope."));
            delete dialog;
            return;
        }
        NodeStructure newNode;

        newNode.setData(nodePosition, pos);
        newNode.setData(nodeID, newID);
        newNode.setData(nodeLabel, dialog->variableName());
        newNode.setData(nodeType, NST_VARIABLE);
        newNode.setData(nodeClass, dialog->variableClass());

        xmlData->addDataNode(newNode);

        emit sceneChanged(RectNodeAdded);
    }

    delete dialog;
}

void OperatorsEdit::defineEllipseNode(QPointF pos, int newID)
{
    Q_ASSERT(xmlDefinition);

    // get the available predicates from the definition
    QSet<QString> availablePredicates = xmlDefinition->getValidPredicateSet();

    if ( availablePredicates.size() == 0 )
    {
        QMessageBox::information(this, tr("VIZ"), tr("There are no predicates defined yet."));
        return;
    }

    OperatorsPredicateDialog * dialog = new OperatorsPredicateDialog(availablePredicates.toList(), this);

    if ( dialog->exec() == QDialog::Accepted )
    {
        NodeStructure newNode;

        newNode.setData(nodePosition, pos);
        newNode.setData(nodeID, newID);
        newNode.setData(nodeLabel, dialog->predicateName());
        newNode.setData(nodeType, NST_PREDICATE);
        newNode.setData(nodePredicateSet, dialog->predicateSet());

        xmlData->addDataNode(newNode);

        emit sceneChanged(EllipseNodeAdded);
    }

    delete dialog;
}

void OperatorsEdit::displayInfo(int nodeID)
{
    Q_ASSERT(xmlData);
    Q_ASSERT(xmlDefinition);

    Q_ASSERT(nodeID != INVALID_ID);

    char clickedType = xmlData->readNodeType(nodeID);

    NodeStructure tempNode;

    QList<int> matchingDefinitions;
    InfoPanel * infoWidget = (InfoPanel*)NULL;
    switch(clickedType)
    {
    case NST_PREDICATE:
        infoWidget = new OperatorsPredicateInfo(xmlData, xmlDefinition, nodeID);
        break;
    case NST_VARIABLE:
        tempNode.setData(nodeLabel, xmlData->getNodeData(nodeID, "class"));
        tempNode.setData(nodeType, NST_CLASS);
        matchingDefinitions = xmlDefinition->selectMatchingIDList(tempNode);

        if ( matchingDefinitions.count() > 1 )
            qWarning() << "$ActionEdit::displayInfo : too many definitions for clicked node";

        infoWidget = new OperatorsVariableInfo(xmlData, xmlDefinition, nodeID, matchingDefinitions.first());
        break;
    default:
        qWarning() << "$ActionEdit::displayInfo : unexpected node type";
        return;
    }

    connect(infoWidget, SIGNAL(madeChange(int)), this, SLOT(handleExternChange(int)));

    emit updateInfoPanel(infoWidget);
}




KAVI_NS_END
