#include "DataWidget.h"
#include "DiagramWidget.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "CheckUtils.h"



using namespace CheckUtils;

DataWidget::DataWidget(DiagramWidget *diagram)
{
    diagramScene = diagram;
    XMLdata = QDomDocument();
}

QDomElement DataWidget::diagramRootElement() const
{
    return diagramData;
}

void DataWidget::loadData(QDomDocument diagramDocument)
{
    diagramScene->reset();

    if ( diagramDocument.isNull() )
    {
        XMLdata = diagramDocument;
        return;
    }

    XMLdata = diagramDocument;

    diagramData = XMLdata.firstChildElement("diagram");

    Q_ASSERT(!diagramData.isNull());

    QDomElement edge = diagramData.firstChildElement("edge");
    while( !edge.isNull() )
    {
        diagramScene->addEdge(loadEdge(edge));
        edge = edge.nextSiblingElement("edge");
    }

    QDomElement node = diagramData.firstChildElement("node");
    while( !node.isNull() )
    {
        diagramScene->addNode(loadNode(node));
        node = node.nextSiblingElement("node");
    }
}

QList<DataEdgepoint> DataWidget::associatedEdgepoints(int nodeID) const
{
    QDomElement node = findNode(nodeID);

    QList<DataEdgepoint> result;

    QDomElement connections = node.firstChildElement("connections");

    if (connections.isNull())
        return result;

    QDomElement edgepoint = connections.firstChild().toElement();
    while( !edgepoint.isNull() )
    {
        if (edgepoint.tagName() == "starts")
        {
            result.append(DataEdgepoint(getIntValue(edgepoint), EDGE_START));
            edgepoint = edgepoint.nextSiblingElement();
            continue;
        }

        if (edgepoint.tagName() == "ends")
        {
            result.append(DataEdgepoint(getIntValue(edgepoint), EDGE_END));
            edgepoint = edgepoint.nextSiblingElement();
            continue;
        }

        qWarning() << "DataWidget::associatedEdgepoints : unexpected subelement in <connections>";
        break;
    }

    return result;
}

QList<int> DataWidget::associatedEdges(int nodeID) const
{
    QDomElement node = findNode(nodeID);

    return connectedEdges(node);
}

QPair<int, int> DataWidget::associatedNodes(int edgeID) const
{
    QDomElement edge = findEdge(edgeID);

    int startID	= associatedNodeID(edge, true);
    int endID	= associatedNodeID(edge, false);

    return qMakePair(startID, endID);
}

QString DataWidget::getNodeData(int id, QString tagName) const
{
    QDomElement node = findNode(id);

    Q_ASSERT(!node.isNull());

    return subelementTagValue(node, tagName);
}

const EdgeStructure DataWidget::getEdgeDescription(int id) const
{
    QDomElement edge = findEdge(id);

    return loadEdge(edge);
}

QDomElement DataWidget::findNode(int id) const
{
    return findSubelementAttr(diagramData, "node", "id", QString().setNum(id));
}

QDomElement DataWidget::findEdge(int id) const
{
    return findSubelementAttr(diagramData, "edge", "id", QString().setNum(id));
}

char DataWidget::readEdgePurpose(int edgeID)
{
    QDomElement edge = findEdge(edgeID);

    QDomElement purpose = edge.firstChildElement("purpose");

    return detEdgePurpose(purpose);
}

char DataWidget::readNodeType(int nodeID)
{
    QDomElement node = findNode(nodeID);

    Q_ASSERT(!node.isNull());

    return detNodeType(node.firstChildElement("type"));
}

QSet<QString> DataWidget::getValidPredicateSet() const
{
    Q_ASSERT(!diagramData.isNull());

    QDomElement actNode = diagramData.firstChildElement("node");

    // <predicate'name, count of predicate's arg>
    QMultiMap<QString, int> argNumbers;
    while ( !actNode.isNull() )
    {
        if (verifyNodeType(actNode, NST_PREDICATE))
            argNumbers.insert(subelementTagValue(actNode, "label"), getArgCount(actNode));

        actNode = actNode.nextSiblingElement("node");
    }

    // the unique names of predicates
    QSet<QString> uniqueNames = QSet<QString>::fromList(argNumbers.uniqueKeys());
    QSet<QString> invalid;

    foreach(QString pred, uniqueNames)
    {
        // get the arg count set of the specified predicate's name, one predicate's
        // name may have several predicates with different arg's count
        QSet<int> argCountSet = QSet<int>::fromList(argNumbers.values(pred));

        // wo consider all the predicates with the same name should have the same
        // number of args, otherwise called invalid
        if ( argCountSet.size() != 1 )
            invalid.insert(pred);
    }

    return uniqueNames.subtract(invalid);
}

QStringList DataWidget::getNodeLabelList(char type) const
{
    QStringList result;

    NodeStructure tempNode;
    tempNode.setData(nodeType, type);

    Q_ASSERT(!diagramData.isNull());

    QDomElement actNode = diagramData.firstChildElement("node");

    while ( !actNode.isNull())
    {
        if (nodeMatches(actNode, tempNode))
            result.append(getStrValue(actNode.firstChildElement("label")));

        actNode = actNode.nextSiblingElement("node");
    }

    return result;
}

QSet<QString> DataWidget::getNodeLabelSet(char type) const
{
    QSet<QString> result;

    NodeStructure tempNode;
    tempNode.setData(nodeType,type);

    Q_ASSERT(!diagramData.isNull());

    QDomElement actNode = diagramData.firstChildElement("node");

    while ( !actNode.isNull())
    {
        if (nodeMatches(actNode, tempNode))
            result.insert(getStrValue(actNode.firstChildElement("label")));

        actNode = actNode.nextSiblingElement("node");
    }

    return result;
}

int DataWidget::getMatchingNodeID(const NodeStructure &nodeTemplate) const
{
    QList<int> candidates = selectMatchingIDList(nodeTemplate);

    if ( candidates.isEmpty() )
    {
        qWarning() << "$DataWidget::getMatchingNodeID : specified node not found.";
        return INVALID_ID;
    }

    if ( candidates.size() > 1 )
        qWarning() << "$DataWidget::getMatchingNodeID : specified node is not unique";

    return candidates.first();
}

QList<int> DataWidget::selectMatchingIDList(const NodeStructure &nodeTemplate) const
{
    return CheckUtils::selectMatchingIDList(diagramData, nodeTemplate);
}

QList<QDomElement> DataWidget::selectMatchingElementList(const NodeStructure &nodeTemplate) const
{
    return CheckUtils::selectMatchingElementList(diagramData, nodeTemplate);

}

void DataWidget::refreshNode(int nodeID)
{
    QDomElement targetNode = findNode(nodeID);

    diagramScene->updateNode(loadNode(targetNode));
}

void DataWidget::addDataNode(const NodeStructure &node)
{
    qDebug() << "$DataWidget::addDataNode";

    Q_ASSERT(!XMLdata.isNull());

    QDomElement newChild = XMLdata.createElement("node");

    setIntAttribute(newChild, "id", node.readData(nodeID).toInt());

    addChildElement(newChild, "label", node.readData(nodeLabel).toString());

    char nodetype = node.readData(nodeType).toChar().toLatin1();
    QStringList predSet;

    QVariant taskState = node.readData(nodeState);

    int i;
    switch(nodetype)
    {
    case NST_PREDICATE:
        addChildElement(newChild, "type", NT_PREDICATE);
        if ( !taskState.isNull() )
            addChildElement(newChild, "state", taskState.toString());

        predSet = node.readData(nodePredicateSet).toStringList();
        for (i = 0; i < predSet.size(); ++i)
            addChildElement(newChild, "set", predSet.at(i));
        break;
    case NST_CLASS:
        addChildElement(newChild, "type", NT_CLASS);
        break;
    case NST_OBJECT:
        addChildElement(newChild, "type", NT_OBJECT);
        addChildElement(newChild, "class", node.readData(nodeClass).toString());
        break;
    case NST_VARIABLE:
        addChildElement(newChild, "type", NT_VARIABLE);
        addChildElement(newChild, "class", node.readData(nodeClass).toString());
        break;
    default:
        Q_ASSERT(false);
    }

    QDomElement newChildPos = XMLdata.createElement("pos");
    setPosition(newChildPos, node.readData(nodePosition).toPointF());

    newChild.appendChild(newChildPos);

    QDomElement lastNode = diagramData.lastChildElement("node");

    if (lastNode.isNull())
        diagramData.appendChild(newChild);
    else
        diagramData.insertAfter(newChild, lastNode);

    diagramScene->addNode(node);
}

void DataWidget::addDataEdge(const EdgeStructure &edge)
{
    Q_ASSERT(!XMLdata.isNull());

    QDomElement newChild = XMLdata.createElement("edge");

    setIntAttribute(newChild, "id", edge.id);

    switch(edge.purpose)
    {
    case DEP_ASSOCIATION:
        addChildElement(newChild, "purpose", EP_ASSOCIATION);
        break;
    case DEP_INHERITANCE:
        addChildElement(newChild, "purpose", EP_INHERITANCE);
        break;
    default:
        Q_ASSERT(false);
    }

    QDomElement posTag;
    QDomElement connEl;

    connEl = XMLdata.createElement("start");
    setIntAttribute(connEl, "nid", edge.startNodeID);

    posTag = XMLdata.createElement("pos");
    setPosition(posTag, edge.startPos);

    connEl.appendChild(posTag);
    newChild.appendChild(connEl);


    connEl = XMLdata.createElement("end");
    setIntAttribute(connEl, "nid", edge.endNodeID);

    posTag = XMLdata.createElement("pos");
    setPosition(posTag, edge.endPos);

    connEl.appendChild(posTag);
    newChild.appendChild(connEl);

    QDomElement refChild = diagramData.lastChildElement("edge");

    if (!refChild.isNull())
        diagramData.insertAfter(newChild, refChild);
    else
        diagramData.appendChild(newChild);

    diagramScene->addEdge(edge);
}

void DataWidget::delDataNode(int id)
{
    QDomElement removedNode = findNode(id);

    QDomNode parent;

    if ( !removedNode.isNull() )
    {
        parent = removedNode.parentNode();
        parent.removeChild(removedNode);

        diagramScene->removeNode(id);
    }
    else
        qWarning() << "DataWidget::delDataNode : node id:" << id << "not found.";
}

EdgeDefinition DataWidget::delDataEdge(int id)
{
    QDomElement removedEdge = findEdge(id);

    QDomNode parent;

    EdgeDefinition result(INVALID_ID, INVALID_ID);

    if ( !removedEdge.isNull() )
    {
        QDomElement start = removedEdge.firstChildElement("start");
        QDomElement end = removedEdge.firstChildElement("end");

        result.first = getIntAttribute(start, "nid");
        result.second = getIntAttribute(end, "nid");

        parent = removedEdge.parentNode();
        parent.removeChild(removedEdge);

        diagramScene->removeEdge(id);
    }
    else
        qWarning() << "$DataWidget::delDataEdge : edge id:" << id << "not found.";

    return result;
}

void DataWidget::changeNodePos(int id, const QPointF &vector)
{
    qDebug() << "$DataWidget::changeNodePos";

    QDomElement node = findNode(id);

    QDomElement position = node.firstChildElement("pos");

    Q_ASSERT(!position.isNull());

    addVector(position, vector);
}

void DataWidget::changeEdgePos(int id, bool isStart, const QPointF &vector)
{
    QDomElement edge = findEdge(id);

    QDomElement edgepoint;
    if (isStart)
    {
        edgepoint = edge.firstChildElement("start");
    }
    else
    {
        edgepoint = edge.firstChildElement("end");
    }

    Q_ASSERT(!edgepoint.isNull());

    QDomElement position = edgepoint.firstChildElement("pos");

    Q_ASSERT(!position.isNull());

    addVector(position, vector);
}

void DataWidget::changeEdgeAssociation(int id, bool isStart, int toNode)
{
    QDomElement edge = findEdge(id);

    if (isStart)
        setIntAttribute(edge.firstChildElement("start"), "nid", toNode);
    else
        setIntAttribute(edge.firstChildElement("end"), "nid", toNode);
}

QDomElement DataWidget::disconnectEdgeFromNode(int edgeID, bool start, int nodeID)
{
    QDomElement targetNode = findNode(nodeID);

    QDomElement connections = targetNode.firstChildElement("connections");

    Q_ASSERT(!connections.isNull());

    int connCount = connections.childNodes().count();

    QDomElement result = QDomElement();

    if ( connCount > 1 )
    {
        qDebug() << "$disconnectEdgeFromNode: connCount > 1";
        QDomElement removed;

        if (start)
        {
            qDebug() << "$disconnectEdgeFromNode: removing <starts>";
            removed = findSubelementVal(connections, "starts", QString().setNum(edgeID));

            result = removed.nextSiblingElement("starts");
        }
        else
        {
            qDebug() << "$disconnectEdgeFromNode: removing <ends>";
            removed = findSubelementVal(connections, "ends", QString().setNum(edgeID));
        }

        connections.removeChild(removed);
    }
    else
        targetNode.removeChild(connections);

    return result;
}

QDomElement DataWidget::connectEdgeToNode(int edgeID, bool start, int nodeID)
{
    QDomElement node = findNode(nodeID);

    QDomElement connections = node.firstChildElement("connections");
    if (connections.isNull())
    {
        connections = XMLdata.createElement("connections");
        node.appendChild(connections);
    }

    QString edgeNum;
    edgeNum.setNum(edgeID);

    if (start)
        return addChildElement(connections, "starts", edgeNum);
    else
        return addChildElement(connections, "ends", edgeNum);
}

void DataWidget::addVector(QDomElement &pos, QPointF vector)
{
    QPointF oldPos = getPosValue(pos);
    setPosition(pos, oldPos + vector);
}

void DataWidget::setPosition(QDomElement &pos, QPointF point)
{
    QString strX;
    QString strY;

    strX.setNum(point.x(), 'g');
    strY.setNum(point.y(), 'g');

    QDomElement subX = pos.firstChildElement("x");
    QDomElement subY = pos.firstChildElement("y");

    if (subX.isNull())
        addChildElement(pos, "x", strX);
    else
        subX.replaceChild(XMLdata.createTextNode(strX), subX.firstChild());

    if (subY.isNull())
        addChildElement(pos, "y", strY);
    else
        subY.replaceChild(XMLdata.createTextNode(strY), subY.firstChild());
}

QDomElement DataWidget::addChildElement(QDomElement &parent, QString childName, QString content)
{
    QDomElement newTag = XMLdata.createElement(childName);
    newTag.appendChild(XMLdata.createTextNode(content));

    Q_ASSERT(!parent.isNull());

    parent.appendChild(newTag);
    return newTag;
}

void DataWidget::resetPredicate(QDomElement &elem)
{
    QDomElement setTag = elem.firstChildElement("set");

    while( !setTag.isNull() )
    {
        elem.removeChild(setTag);
        setTag = setTag.nextSiblingElement("set");
    }
}

const NodeStructure DataWidget::loadNode(const QDomElement &elem) const
{
    NodeStructure result;

    char nodetype = detNodeType(elem.firstChildElement("type"));

    result.setData(nodeID, getIntAttribute(elem, "id"));
    result.setData(nodeLabel, getStrValue(elem.firstChildElement("label")));
    result.setData(nodePosition, getPosValue(elem.firstChildElement("pos")));
    result.setData(nodeType, nodetype);

    QDomElement taskState = elem.firstChildElement("state");
    if ( !taskState.isNull() )
        result.setData(nodeState, getStrValue(taskState));

    if (nodetype & NST_PREDICATE)
    {
        QDomElement setElement = elem.firstChildElement("set");
        QStringList predSet;
        while( !setElement.isNull() )
        {
            predSet.append(getStrValue(setElement));
            setElement = setElement.nextSiblingElement("set");
        }

        result.setData(nodePredicateSet, predSet);
    }

    return result;
}

const EdgeStructure DataWidget::loadEdge(const QDomElement &elem) const
{
    EdgeStructure result;

    result.id = getIntAttribute(elem, "id");
    result.purpose = detEdgePurpose(elem.firstChildElement("purpose"));

    QDomElement startPoint = elem.firstChildElement("start");
    QDomElement endPoint = elem.firstChildElement("end");

    Q_ASSERT(!startPoint.isNull() && !endPoint.isNull());

    result.startNodeID = getIntAttribute(startPoint, "nid");
    result.endNodeID = getIntAttribute(endPoint, "nid");

    result.startPos = getPosValue(startPoint.firstChildElement("pos"));
    result.endPos = getPosValue(endPoint.firstChildElement("pos"));

    return result;
}




