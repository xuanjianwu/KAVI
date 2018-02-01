#include "ProblemsEdit.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "XMLUtils.h"
#include "InfoPanel.h"
#include "ProblemsObjectDialog.h"
#include "ProblemsPredicateDialog.h"
#include "ProblemsObjectInfo.h"
#include "ProblemsPredicateInfo.h"


KAVI_NS_BEGIN

using namespace XMLUtils;

ProblemsEdit::ProblemsEdit(QWidget *parent)
    :DrivenEditWidget(parent)
{
    allowedEdgeMask = DEP_ASSOCIATION;
    allowedNodeMask = NST_OBJECT | NST_PREDICATE;

    xmlDefinition = (DataWidget*)NULL;
}

void ProblemsEdit::setTaskState(PredicateState state)
{
    taskState = state;
}

void ProblemsEdit::setPredicatesVisible(bool visible, PredicateState selector)
{
    NodeStructure tempNode;

    tempNode.setData(nodeType, NST_PREDICATE);

    switch(selector)
    {
    case InitState:
        tempNode.setData(nodeState, NSTS_INIT);
        break;
    case GoalState:
        tempNode.setData(nodeState, NSTS_GOAL);
        break;
    default:
        Q_ASSERT(false);
    }

    QList<int> predicateList = xmlData->selectMatchingIDList(tempNode);

    foreach(int predID, predicateList)
    {
        diagram->setNodeVisible(predID, visible);

        QList<DataEdgepoint> edgepointList = xmlData->associatedEdgepoints(predID);
        foreach(DataEdgepoint point, edgepointList)
            diagram->setEdgeVisible(point.first, visible);
    }
}

void ProblemsEdit::defineRectangleNode(QPointF pos, int newID)
{
    Q_ASSERT(xmlDefinition);

    // get the available classes from the definition
    QStringList availableClasses = xmlDefinition->getNodeLabelList(NST_CLASS);

    if ( availableClasses.count() == 0 )
    {
        QMessageBox::information(this, tr("KAVI"),
        tr("Object class has to be defined - there are no classes defined yet."));
        return;
    }

    ProblemsObjectDialog * dialog = new ProblemsObjectDialog(availableClasses, this);

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->objectName().isEmpty() )
        {
            QMessageBox::information(this, tr("KAVI"), tr("Object name can't be empty."));
            delete dialog;
            return;
        }

        if ( !nameChecker.exactMatch(dialog->objectName()) )
        {
            QMessageBox::warning(this, tr("KAVI"),
            tr("Name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
            delete dialog;
            return;
        }

        QStringList definedObjects = xmlData->getNodeLabelList(NST_OBJECT);
        if ( definedObjects.contains(dialog->objectName(), Qt::CaseInsensitive) )
        {
            QMessageBox::warning(this, tr("KAVI"), tr("Object name must be unique in problem scope."));
            delete dialog;
            return;
        }

        NodeStructure newNode;

        newNode.setData(nodePosition, pos);
        newNode.setData(nodeID, newID);
        newNode.setData(nodeLabel, dialog->objectName());
        newNode.setData(nodeType ,NST_OBJECT);
        newNode.setData(nodeClass, dialog->objectClass());

        xmlData->addDataNode(newNode);

        emit sceneChanged(RectNodeAdded);
    }

    delete dialog;
}

void ProblemsEdit::defineEllipseNode(QPointF pos, int newID)
{
    // predicate must belong to init or goal state
    if (taskState == NoState)
        return;

    Q_ASSERT(xmlDefinition);

    // get the available predicates from the definition
    QSet<QString> availablePredicates = xmlDefinition->getValidPredicateSet();

    if ( availablePredicates.size() == 0 )
    {
        QMessageBox::information(this, tr("KAVI"), tr("There are no predicates defined yet."));
        return;
    }

    ProblemsPredicateDialog * dialog = new ProblemsPredicateDialog(availablePredicates.toList(), (taskState == InitState) , this);

    if ( dialog->exec() == QDialog::Accepted )
    {
        NodeStructure newNode;

        newNode.setData(nodePosition, pos);
        newNode.setData(nodeID, newID);
        newNode.setData(nodeLabel, dialog->predicateName());
        newNode.setData(nodeType, NST_PREDICATE);
        newNode.setData(nodeState, dialog->taskState());

        xmlData->addDataNode(newNode);

        emit sceneChanged(EllipseNodeAdded);
    }

    delete dialog;
}

void ProblemsEdit::displayInfo(int nodeID)
{
    Q_ASSERT(diagram);
    Q_ASSERT(xmlData);
    Q_ASSERT(xmlDefinition);

    Q_ASSERT(nodeID != INVALID_ID);

    char clickedType = xmlData->readNodeType(nodeID);

    NodeStructure tempNode;

    InfoPanel * infoWidget = (InfoPanel*)NULL;
    QList<int> matchingDefinitions;
    switch(clickedType)
    {
    case NST_PREDICATE:
        infoWidget = new ProblemsPredicateInfo(xmlData, xmlDefinition, nodeID);
        break;
    case NST_OBJECT:
        tempNode.setData(nodeLabel, xmlData->getNodeData(nodeID, "class"));
        tempNode.setData(nodeType, NST_CLASS);
        matchingDefinitions = xmlDefinition->selectMatchingIDList(tempNode);

        if ( matchingDefinitions.count() > 1 )
            qWarning() << "TaskEdit::displayInfo : too many definitions for clicked node";

        infoWidget = new ProblemsObjectInfo(xmlData, xmlDefinition, nodeID, matchingDefinitions.first());
        break;
    default:
        qWarning() << "$TaskEdit::displayInfo : unexpected node type";
        return;
    }

    connect(infoWidget, SIGNAL(madeChange(int)), this, SLOT(handleExternChange(int)));

    emit updateInfoPanel(infoWidget);
}



KAVI_NS_END
