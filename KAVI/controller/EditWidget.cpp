#include "EditWidget.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "Node.h"
#include "DiagramEvents.h"


KAVI_NS_BEGIN



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

}

void EditWidget::handleNodeReshape(int id)
{

}

void EditWidget::handleNodeMove(int id, QPointF vector)
{

}

void EditWidget::handleNodeDrag(int id, QPointF vector)
{

}

void EditWidget::handleEdgeMove(int id, bool isStart, QPointF vector)
{

}

void EditWidget::defineEdge(int startID, int endID, QPointF startPoint, QPointF endPoint, int newID)
{

}

void EditWidget::deleteNode(int nodeID)
{

}

void EditWidget::deleteEdge(int edgeID)
{

}

char EditWidget::determineEdgePurpose(char startType, char endType)
{

}

bool EditWidget::verifyEdge(EdgeStructure &edge, int &argNum)
{

}

void EditWidget::deleteConnection(int edgeID)
{

}

void EditWidget::changeConnection(EdgeStructure &edge, bool isStart, int toNode, int &argNum)
{

}

void EditWidget::makeConnection(EdgeStructure &edge, int &argNum)
{

}

EditWidget::ReconnectionValue EditWidget::verifyReconnection(EdgeStructure &edge, int newNode, bool startMoved, int &argNum)
{

}

void EditWidget::displayInfo(int nodeID)
{

}

void EditWidget::defineEllipseNode(QPointF pos, int newID)
{

}

void EditWidget::defineRectangleNode(QPointF pos, int newID)
{

}



KAVI_NS_END
