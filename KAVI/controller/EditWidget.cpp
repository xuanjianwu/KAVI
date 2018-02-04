#include "EditWidget.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "Node.h"
#include "DiagramEvents.h"






EditWidget::EditWidget(QWidget *parent)
    :QGraphicsView(parent)
{
    diagram = new DiagramWidget(DIAGRAM_SIZE, DIAGRAM_SIZE, this);
    xmlData = new DataWidget(diagram);

    this->setScene(diagram);

    changed = false;
    nameChecker.setPattern(QString("^([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|[-_]){,%1}$").arg(MAX_LABEL_LENGTH));
    editMode = AddRect;
}

void EditWidget::reset()
{
    xmlData->loadData(QDomDocument());
    changed = false;
}

bool EditWidget::wasChanged()
{
    return changed;
}

void EditWidget::setEditMode(EditMode mode)
{
    editMode = mode;

    switch(mode)
    {
        case AddRect:
        case AddEllipse:
        case Delete:
            diagram->setDiagramMode(DetectClick);
        break;
        case AddEdge:
            diagram->setDiagramMode(InsertLine);
        break;
        default:
            Q_ASSERT(false);
    }
}

const DataWidget *EditWidget::xmlDataPointer()
{
    Q_ASSERT(xmlData);
    return xmlData;
}

void EditWidget::setChanged(bool value)
{
    changed = value;
}

void EditWidget::handleExternChange(int changeCode)
{
    setChanged(true);
    emit sceneChanged(changeCode);
}

void EditWidget::loadXMLdata(QDomDocument diagramDocument)
{
    xmlData->loadData(diagramDocument);
    changed = false;
}

bool EditWidget::event(QEvent *event)
{
    if (event->type() == DiagramLeftClick)
    {
        qDebug() << "$DiagramLeftClick";
        DiagramEvent *myEvent = static_cast<DiagramEvent *>(event);

        qDebug() << "$newNodeID:" << myEvent->itemID();

        Q_ASSERT(myEvent->data().canConvert(QVariant::PointF));

        QPointF newNodePos = myEvent->data().toPointF();

        if (editMode == AddRect)
            defineRectangleNode(newNodePos, myEvent->itemID());

        if (editMode == AddEllipse)
            defineEllipseNode(newNodePos, myEvent->itemID());

        changed = true;

        return true;
    }

    if (event->type() == DiagramRightClick)
    {
        // right click on diagram means nothing
        return true;
    }

    if (event->type() == NodeLeftClick)
    {
        qDebug() << "$NodeLeftClick";
        DiagramEvent *myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$clicked node ID:" << myEvent->itemID();

        if (editMode == Delete)
            deleteNode(myEvent->itemID());

        return true;
    }

    if (event->type() == NodeRightClick)
    {
        qDebug() << "$NodeRightClick";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$clicked node ID:" << myEvent->itemID();

        displayInfo(myEvent->itemID());

        return true;
    }

    if (event->type() == EdgeLeftClick)
    {
        qDebug() << "$EdgeLeftClick";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$clicked edge ID:" << myEvent->itemID();

        if (editMode == Delete)
            deleteEdge(myEvent->itemID());

        return true;
    }

    if (event->type() == EdgeRightClick)
    {
        // right click on the edge means nothing
        return true;
    }

    if (event->type() == NodeMoved)
    {
        qDebug() << "$NodeMoved";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$node ID:" << myEvent->itemID();

        Q_ASSERT(myEvent->data().canConvert(QVariant::PointF));
        QPointF vector = myEvent->data().toPointF();
        qDebug() << "$vector:" << vector;

        handleNodeMove(myEvent->itemID(), vector);

        return true;
    }

    if (event->type() == EdgeMoved)
    {
        qDebug() << "$EdgeMoved";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$edge ID:" << myEvent->itemID();

        Q_ASSERT(myEvent->data().canConvert(QVariant::List));
        QVariantList package = myEvent->data().toList();
        bool isStart = package.takeFirst().toBool();
        QPointF vector = package.takeFirst().toPointF();
        qDebug() << "$vector:" << vector;

        handleEdgeMove(myEvent->itemID(), isStart, vector);

        return true;
    }

    if (event->type() == NodeDrag)
    {
        qDebug() << "$NodeDrag";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$node ID:" << myEvent->itemID();

        Q_ASSERT(myEvent->data().canConvert(QVariant::PointF));
        QPointF vector = myEvent->data().toPointF();
        qDebug() << "$vector:" << vector;

        handleNodeDrag(myEvent->itemID(), vector);

        return true;
    }

    if (event->type() == EdgeDefined)
    {
        qDebug() << "$EdgeDefined";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$new edgeID:" << myEvent->itemID();

        Q_ASSERT(myEvent->data().canConvert(QVariant::List));

        QVariantList package = myEvent->data().toList();
        int startID = package.takeFirst().toInt();
        int endID =	package.takeFirst().toInt();
        QPointF startPos = package.takeFirst().toPointF();
        QPointF endPos = package.takeFirst().toPointF();
        qDebug() << "$start: " << startID << "," << startPos;
        qDebug() << "$end: " << endID << "," << endPos;

        if (editMode == AddEdge)
            defineEdge(startID, endID, startPos, endPos, myEvent->itemID());
        else
            qWarning() << "$EditWidget::event : EdgeDefined event in wrong mode";

        return true;
    }

    if (event->type() == NodeReshaped)
    {
        qDebug() << "$NodeReshaped";
        DiagramEvent * myEvent = static_cast<DiagramEvent *>(event);
        qDebug() << "$node ID:" << myEvent->itemID();

        //handleNodeReshape(myEvent->itemID());

        return true;
    }

    // call the parent event class implementation for all the events you did not handle.
    return QGraphicsView::event(event);
}

void EditWidget::handleNodeReshape(int id)
{
    Q_ASSERT(xmlData);
    Q_ASSERT(diagram);

    // get the associated edgepoints of node in format
    // <connected edge's ID, connected edgepoint's orientation>
    QList<DataEdgepoint> edgepoints = xmlData->associatedEdgepoints(id);

    int actID;
    bool actBool;

    qDebug() << "handleNodeReshape for:" << id;
    foreach(DataEdgepoint point, edgepoints)
    {
        actID = point.first;
        actBool = point.second;
        qDebug() << "edge:" << actID;
        diagram->stickEdgeToNode(actID, actBool, id);
    }
}

void EditWidget::handleNodeMove(int id, QPointF vector)
{
    xmlData->changeNodePos(id, vector);

    QList<DataEdgepoint> assocEdges = xmlData->associatedEdgepoints(id);

    int actID;
    bool actBool;

    /*
    for(int i=0; i < assocEdges.size(); ++i)
    {
        actID = assocEdges.at(i).first;
        actBool = assocEdges.at(i).second;

        xmlData->changeEdgePos(actID, actBool, vector);
    }
    */
    foreach (DataEdgepoint point, assocEdges)
    {
        actID = point.first;
        actBool = point.second;

        xmlData->changeEdgePos(actID, actBool, vector);
    }

    changed = true;
}

void EditWidget::handleNodeDrag(int id, QPointF vector)
{
    Q_ASSERT(xmlData);
    Q_ASSERT(diagram);
    QList<DataEdgepoint> assocEdges = xmlData->associatedEdgepoints(id);

    int actID;
    bool actBool;

    foreach(DataEdgepoint point,assocEdges)
    {
        actID = point.first;
        actBool = point.second;
        diagram->translateEdge(actID, actBool, vector);
    }
}

void EditWidget::handleEdgeMove(int id, bool isStart, QPointF vector)
{
    Q_ASSERT(diagram);
    int diagNode = diagram->collidingNodeID(id, isStart);

    Q_ASSERT(xmlData);
    EdgeStructure reconnectedEdge = xmlData->getEdgeDescription(id);

    int argNum = INVALID_ARGN;

    switch( verifyReconnection(reconnectedEdge, diagNode, isStart, argNum) )
    {
    case NoChange:
        diagram->translateEdge(id, isStart, -vector);
        break;
    case PosChange:
        xmlData->changeEdgePos(id, isStart, vector);
        changed = true;
        break;
    case PosAssocChange:
        changeConnection(reconnectedEdge, isStart, diagNode, argNum);
        changed = true;
        break;
    default:
        Q_ASSERT(false);
    }
}

void EditWidget::defineEdge(int startID, int endID, QPointF startPoint, QPointF endPoint, int newID)
{
    EdgeStructure newEdge;
    newEdge.id = newID;
    newEdge.startPos = startPoint;
    newEdge.endPos = endPoint;
    newEdge.startNodeID = startID;
    newEdge.endNodeID = endID;

    int argNum = INVALID_ARGN;
    if ( verifyEdge(newEdge, argNum) )
    {
        makeConnection(newEdge, argNum);
        changed = true;
    }
}

void EditWidget::renumberPredicateArguments(QDomElement toRenumber)
{
    if ( toRenumber.isNull() || (toRenumber.tagName() != "starts") )
        return;

    int value = getIntAttribute(toRenumber, "argn");
    value--;
    while ( !toRenumber.isNull() )
    {
        setIntAttribute(toRenumber, "argn", value);
        value ++;
        toRenumber = toRenumber.nextSiblingElement("starts");
    }
}

void EditWidget::deleteNode(int nodeID)
{
    char nodeType = xmlData->readNodeType(nodeID);

    QList<int> edges = xmlData->associatedEdges(nodeID);
    foreach(int edgeID, edges)
        deleteConnection(edgeID);

    xmlData->delDataNode(nodeID);

    changed = true;

    switch(nodeType)
    {
    case NST_PREDICATE:
        emit sceneChanged(EllipseNodeDeleted);
        break;
    case NST_VARIABLE:
    case NST_OBJECT:
    case NST_CLASS:
        emit sceneChanged(RectNodeDeleted);
        break;
    default:
        Q_ASSERT(false);
    }
}

void EditWidget::deleteEdge(int edgeID)
{
    char edgePurpose = xmlData->readEdgePurpose(edgeID);

    deleteConnection(edgeID);
    changed = true;

    switch(edgePurpose)
    {
    case DEP_ASSOCIATION:
        emit sceneChanged(AssocEdgeDeleted);
        break;
    case DEP_INHERITANCE:
        emit sceneChanged(InherEdgeDeleted);
        break;
    default:
        Q_ASSERT(false);
    }
    return;
}

char EditWidget::determineEdgePurpose(char startType, char endType)
{
    /*	C - class
        V - variable		"X" edge is forbiden
        O - object          "A" asociation edge
        P - predicate		"I" inheritance edge

      C V O P
    C I X X X
    V X X X X
    O X X X X
    P A A A X	*/

    if ( startType == endType )
    {
        if ( startType == NST_CLASS )
            return DEP_INHERITANCE;
        else
            return DEP_NO_PURPOSE;
    }
    else
    {
        if ( (startType == NST_PREDICATE) && (endType != NST_PREDICATE) )
            return DEP_ASSOCIATION;
        else
            return DEP_NO_PURPOSE;
    }
}

bool EditWidget::verifyEdge(EdgeStructure &edge, int &argNum)
{
    Q_UNUSED(argNum);

    char startType = xmlData->readNodeType(edge.startNodeID);
    char endType = xmlData->readNodeType(edge.endNodeID);

    if ( (startType != NST_PREDICATE) && (endType == NST_PREDICATE) )
    {
        edge.switchOrientation();
        edge.purpose = determineEdgePurpose(endType, startType);
    }
    else
    {
        edge.purpose = determineEdgePurpose(startType, endType);
    }

    if ( !(edge.purpose & allowedEdgeMask) )
    {
        qWarning() << "This edge is not allowed here.";
        return false;
    }

    return true;
}



