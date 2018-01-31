#include "CheckUtils.h"
#include "XMLUtils.h"
#include "DataWidget.h"
#include "GraphClass.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"

CU_NS_BEGIN

using namespace XMLUtils;

QHash<int, QString> predicateArguments(const QDomElement &predicate)
{
    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QHash<int, QString> result;

    QDomElement connections = predicate.firstChildElement("connections");

    if (connections.isNull())
        return result;

    QDomElement diagram = predicate.parentNode().toElement();

    QDomElement actConn = connections.firstChildElement("starts");

    while( !actConn.isNull() )
    {
        int argNum = getIntAttribute(actConn, "argn");

        int edgeID = getIntValue(actConn);
        QDomElement edge = findSubelementAttr(diagram, "edge", "id", QString().setNum(edgeID));

        Q_ASSERT(!edge.isNull());

        int nodeID = associatedNodeID(edge, false);

        QDomElement node = findSubelementAttr(diagram, "node", "id", QString().setNum(nodeID));

        QString nodeLabel;

        if (node.firstChildElement("class").isNull())
            nodeLabel = subelementTagValue(node, "label");
        else
            nodeLabel = subelementTagValue(node, "class");

        result.insert(argNum, nodeLabel);

        actConn = actConn.nextSiblingElement("starts");
    }

    return result;
}

bool checkPredicateDefinition(const QDomElement &predicate, bool verbose)
{

}

bool checkPredicateInstance(const QDomElement &predicate, const QDomElement &definitionRoot, bool verbose)
{

}

bool checkClassInstance(const QDomElement &instance, const QDomElement &definitionRoot, bool verbose)
{

}

bool checkDefinition(const QDomElement &diagramRoot, bool verbose)
{

}

bool checkDependentDiagram(const QDomElement &diagramRoot, const QDomElement &definitionRoot, char rectType, bool verbose)
{

}

void brokenDiagrams(const QDomDocument &domain, const QDomElement &definitionRoot, const QString &groupTagName, const QString &itemTagName, QStringList &correct, QStringList &incorrect)
{

}

QDomElement selectPredicateDefinition(const QDomElement &definitionRoot, const QDomElement &predInstance)
{

}

int getArgNumber(const QDomElement &predicate, int edgeID)
{

}

int getArgCount(const QDomElement &predicate)
{

}

QStringList classFamily(const QString &className, const GraphClass *structure, const DataWidget *definition)
{

}

bool nodeMatches(const QDomElement &probedNode, const NodeStructure &patternNode)
{

}

int getMatchingNodeID(const QDomElement &root, const NodeStructure &nodeTemplate)
{

}

QList<int> selectMatchingIDList(const QDomElement &root, const NodeStructure &nodeTemplate)
{

}

QList<QDomElement> selectMatchingElementList(const QDomElement &root, const NodeStructure &nodeTemplate)
{

}



CU_NS_END
