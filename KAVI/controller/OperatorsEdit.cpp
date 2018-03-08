#include "OperatorsEdit.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "XMLUtils.h"
#include "InfoPanel.h"
#include "OperatorsVariableDialog.h"
#include "OperatorsPredicateDialog.h"
#include "OperatorsPredicateInfo.h"
#include "OperatorsVariableInfo.h"



using namespace XMLUtils;

OperatorsEdit::OperatorsEdit(QWidget *parent)
    :DrivenEditWidget(parent)
{
    allowedEdgeMask = DEP_ASSOCIATION;
    allowedNodeMask = NST_VARIABLE | NST_PREDICATE;

    xmlDefinition = (DataWidget*)NULL;
}

void OperatorsEdit::defineOperatorFromPlanningDialog(QString newPredicateName, QList<QString> variableName, QList<QString> variableClass, bool predicateSet)
{
    // add the specified predicate node
    NodeStructure newNode;

    QPointF pos = diagram->newNodePos(pos);
    int newID = diagram->newNodeID();

    newNode.setData(nodePosition, pos);
    newNode.setData(nodeID, newID);
    newNode.setData(nodeLabel, newPredicateName);
    newNode.setData(nodeType, NST_PREDICATE);

    if (predicateSet == true)
    {
        newNode.setData(nodePredicateSet, QStringList(NSPS_EFFECT_POS));
    }
    else
    {
        newNode.setData(nodePredicateSet, QStringList(NSPS_EFFECT_NEG));
    }


    xmlData->addDataNode(newNode);
    emit sceneChanged(EllipseNodeAdded);

    // add the associated argument variable
    for (int i = 0; i < variableName.size(); i++)
    {
        NodeStructure newNode;

        QPointF newNodePos = diagram->newNodePos(pos);
        int newNodeID = diagram->newNodeID();

        newNode.setData(nodePosition, newNodePos);
        newNode.setData(nodeID, newNodeID);
        newNode.setData(nodeLabel, variableName.at(i));
        newNode.setData(nodeType, NST_VARIABLE);
        newNode.setData(nodeClass, variableClass.at(i));

        xmlData->addDataNode(newNode);

        emit sceneChanged(RectNodeAdded);

        // add assocaition edges
        int sX = newNodePos.x() > pos.x()? (pos.x() + diagram->getNode(newID)->getSize().width()/2) :
                                          (pos.x() - diagram->getNode(newID)->getSize().width()/2);
        int sY = newNodePos.y() > pos.y()? (pos.y() + diagram->getNode(newID)->getSize().height()/2) :
                                          (pos.y() - diagram->getNode(newID)->getSize().height()/2);
        QPointF sPoint(sX, sY);

        int eX = pos.x() > newNodePos.x()? (newNodePos.x() + diagram->getNode(newNodeID)->getSize().width()/2) :
                                      (newNodePos.x() - diagram->getNode(newNodeID)->getSize().width()/2);
        int eY = pos.y() > newNodePos.y()? (newNodePos.y() + diagram->getNode(newNodeID)->getSize().height()/2) :
                                      (newNodePos.y() - diagram->getNode(newNodeID)->getSize().height()/2);
        QPointF ePoint(eX, eY);

        EdgeStructure newEdge;
        newEdge.id = diagram->newEdgeID();
        newEdge.startPos = sPoint;
        newEdge.endPos = ePoint;
        newEdge.startNodeID = newID;
        newEdge.endNodeID = newNodeID;
        newEdge.purpose = DEP_ASSOCIATION;

        int arg = i+1;
        makeConnection(newEdge, arg);
    }
}

void OperatorsEdit::defineRectangleNode(QPointF pos, int newID)
{
    Q_ASSERT(xmlDefinition);

    // get the available classes from the definition
    QStringList availableClasses = xmlDefinition->getNodeLabelList(NST_CLASS);

    if ( availableClasses.count() == 0 )
    {
        QMessageBox::information(this, tr("KAVI"),
        tr("Variable have to belong to some class - there are no class defined yet."));
        return;
    }

    OperatorsVariableDialog * dialog = new OperatorsVariableDialog(availableClasses, this);

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->variableName().isEmpty() )
        {
            QMessageBox::information(this, tr("KAVI"), tr("Variable name can't be empty."));
            delete dialog;
            return;
        }

        if ( !nameChecker.exactMatch(dialog->variableName()) )
        {
            QMessageBox::warning(this, tr("KAVI"),
            tr("Name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
            delete dialog;
            return;
        }

        QStringList definedVariables = xmlData->getNodeLabelList(NST_VARIABLE);
        if ( definedVariables.contains(dialog->variableName(), Qt::CaseInsensitive) )
        {
            QMessageBox::warning(this, tr("KAVI"), tr("Variable name must be unique in action scope."));
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
        QMessageBox::information(this, tr("KAVI"), tr("There are no predicates defined yet."));
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





