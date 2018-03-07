#include "EditWidget.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "Node.h"
#include "DiagramEvents.h"


#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()


EditWidget::EditWidget(QWidget *parent)
    :QGraphicsView(parent)
{
    initInteractiveView();

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

void EditWidget::initInteractiveView()
{
    m_translateButton = Qt::RightButton;
    m_scale = 1.0;
    m_zoomDelta = 0.1;
    m_translateSpeed = 1.0;
    m_bMouseTranslate = false;

    // 去掉滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    centerOn(0, 0);
}

void EditWidget::setTranslateSpeed(qreal speed)
{
    // 建议速度范围
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "InteractiveView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
    m_translateSpeed = speed;
}

qreal EditWidget::translateSpeed() const
{
    return m_translateSpeed;
}

void EditWidget::setZoomDelta(qreal delta)
{
    // 建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "InteractiveView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;
}

qreal EditWidget::zoomDelta() const
{
    return m_zoomDelta;
}

void EditWidget::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

void EditWidget::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

void EditWidget::zoom(float scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    m_scale *= scaleFactor;
}

void EditWidget::translate(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= m_scale;
    delta *= m_translateSpeed;

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
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

void EditWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        translate(QPointF(0, -2));  // 上移
        break;
    case Qt::Key_Down:
        translate(QPointF(0, 2));  // 下移
        break;
    case Qt::Key_Left:
        translate(QPointF(-2, 0));  // 左移
        break;
    case Qt::Key_Right:
        translate(QPointF(2, 0));  // 右移
        break;
    case Qt::Key_Plus:  // 放大
        zoomIn();
        break;
    case Qt::Key_Minus:  // 缩小
        zoomOut();
        break;
    case Qt::Key_Space:  // 逆时针旋转
        rotate(-5);
        break;
    case Qt::Key_Enter:  // 顺时针旋转
    case Qt::Key_Return:
        rotate(5);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void EditWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
        translate(mouseDelta);
    }

    m_lastMousePos = event->pos();

    QGraphicsView::mouseMoveEvent(event);
}

void EditWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton) {
        // 当光标底下没有 item 时，才能移动
        QPointF point = mapToScene(event->pos());
        if (scene()->itemAt(point, transform()) == NULL)  {
            m_bMouseTranslate = true;
            m_lastMousePos = event->pos();
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void EditWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton)
        m_bMouseTranslate = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void EditWidget::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
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

    qDebug() << "@handleNodeReshape for:" << id;
    foreach(DataEdgepoint point, edgepoints)
    {
        actID = point.first;
        actBool = point.second;
        qDebug() << "@edge:" << actID;
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
        qWarning() << "@This edge is not allowed here.";
        return false;
    }

    return true;
}



