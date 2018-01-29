#include "DrivenEditWidget.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "XMLUtils.h"
#include "CheckUtils.h"
#include "GraphClass.h"

KAVI_NS_BEGIN

using namespace XMLUtils;
using namespace CheckUtils;

DrivenEditWidget::DrivenEditWidget(QWidget *parent): EditWidget(parent) {}

void DrivenEditWidget::setDefinition(const DataWidget *definition)
{
    xmlDefinition = definition;
}

void DrivenEditWidget::makeConnection(EdgeStructure &edge, int &argNum)
{
    xmlData->addDataEdge(edge);

    QDomElement connEl = xmlData->connectEdgeToNode(edge.id, EDGE_START, edge.startNodeID);

    Q_ASSERT(argNum != INVALID_ARGN);
    setIntAttribute(connEl, "argn", argNum);

    xmlData->connectEdgeToNode(edge.id, EDGE_END, edge.endNodeID);

    emit sceneChanged(AssocEdgeAdded);
}

void DrivenEditWidget::changeConnection(EdgeStructure &edge, bool isStart, int toNode, int &argNum)
{
    xmlData->changeEdgeAssociation(edge.id, isStart, toNode);

    QDomElement toRenumber;

    if (isStart)
    {
        toRenumber = xmlData->disconnectEdgeFromNode(edge.id, isStart, edge.startNodeID);
        renumberPredicateArguments(toRenumber);
    }
    else
        xmlData->disconnectEdgeFromNode(edge.id, isStart, edge.endNodeID);

    QDomElement connEl = xmlData->connectEdgeToNode(edge.id, isStart, toNode);

    if ( isStart )
    {
        Q_ASSERT(argNum != INVALID_ARGN);
        setIntAttribute(connEl, "argn", argNum);
    }

    emit sceneChanged(AssocEdgeReconnected);
}

void DrivenEditWidget::deleteConnection(int edgeID)
{
    EdgeDefinition removedEdge = xmlData->delDataEdge(edgeID);

    QDomElement nextConnection = xmlData->disconnectEdgeFromNode(edgeID, EDGE_START, removedEdge.first);
    xmlData->disconnectEdgeFromNode(edgeID, EDGE_END, removedEdge.second);

    if ( xmlData->readNodeType(removedEdge.first) == NST_PREDICATE )
        renumberPredicateArguments(nextConnection);

    emit sceneChanged(AssocEdgeDeleted);
}

bool DrivenEditWidget::verifyEdge(EdgeStructure &edge, int &argNum)
{
    if ( !EditWidget::verifyEdge(edge, argNum) )
        return false;

    QDomElement predicate = xmlData->findNode(edge.startNodeID);

    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QDomElement argument = xmlData->findNode(edge.endNodeID);

    Q_ASSERT( verifyNodeType(argument, NST_VARIABLE) || verifyNodeType(argument, NST_OBJECT) );

    QString predicateLabel = subelementTagValue(predicate, "label");
    QString argumentClass = subelementTagValue(argument, "class");

    QHash<int, QString> instArg = predicateArguments(predicate);

    argNum = findArgPosition(predicateLabel, argumentClass, instArg);

    if ( argNum == INVALID_ARGN )
    {
        qWarning() 	<< "Node of class:"
                << argumentClass
                << "is not compatible with" << predicateLabel
                << "in this context.";
        return false;
    }

    return true;
}

EditWidget::ReconnectionValue DrivenEditWidget::verifyReconnection(EdgeStructure &edge, int newNodeID, bool startMoved, int &argNum)
{

}

int DrivenEditWidget::findArgPosition(const QString &predicate, const QString &argument, const QHash<int, QString> &instArg)
{
    // instArg: predicate arguments of predicate in current EditWidget
    // defArg : predicate arguments of predicate from definition
    // argFamily: new argument's class family from definition

    NodeStructure tempNode;
    tempNode.setData(nodeLabel, predicate);
    tempNode.setData(nodeType, NST_PREDICATE);

    int result = INVALID_ARGN;

    QList<QDomElement> definitions = xmlDefinition->selectMatchingElementList(tempNode);
    if ( definitions.isEmpty() )
    {
        qWarning() << "No predicate definition available for: " << predicate;
        return result;
    }

    GraphClass * inheritanceStructure = new GraphClass(xmlDefinition->diagramRootElement());
    inheritanceStructure->init(NST_CLASS, DEP_INHERITANCE, IGRAPH_DIRECTED);

    QStringList argFamily = classFamily(argument, inheritanceStructure, xmlDefinition);

    Q_ASSERT(!argFamily.isEmpty());

    while ( !definitions.isEmpty() )
    {
        QDomElement currentDef = definitions.takeFirst();

        QHash<int, QString> defArg = predicateArguments(currentDef);

        bool exactMatch = true;

        QHashIterator<int, QString> i(instArg);

        // the argument match between current EditWidget and definition
        while (i.hasNext())
        {
            i.next();

            // actArgFamily: predicate argument's class family of predicate in current EditWidget from definition
            QStringList actArgFamily = classFamily(i.value(), inheritanceStructure, xmlDefinition);

            // the argument's class at index i from definition
            QString defClass = defArg.value(i.key());

            if (!actArgFamily.contains(defClass))
            {
                exactMatch = false;
                break;
            }
        }

        if( !exactMatch )
            continue;

        QSet<int> defArgSet = QSet<int>::fromList(defArg.keys());
        QSet<int> instArgSet = QSet<int>::fromList(instArg.keys());

        QSet<int> freePos = defArgSet - instArgSet;

        bool compatible = false;
        foreach(int pos, freePos)
        {
            QString defClass = defArg.value(pos);
            compatible = argFamily.contains(defClass);
            if(compatible)
                return pos;
        }
    }

    delete inheritanceStructure;

    return result;
}



KAVI_NS_END
