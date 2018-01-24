/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : This class inherits from QGraphicsScene and it is
 *          an implementation of the View, serves as the view
 *          in MVC.
*/

#ifndef DIAGRAMWIDGET_H
#define DIAGRAMWIDGET_H

#include "KAVIBase.h"

KAVI_NS_BEGIN

class Node;
class Edge;
struct NodeStructure;
struct EdgeStructure;

class DiagramWidget: public QGraphicsScene {
    Q_OBJECT

public:
    DiagramWidget(int width, int height, QWidget* parent = 0);

    void reset();

    void setDiagramMode(DiagramMode mode);

    void setNodeVisible(int nodeID, bool visible);
    void setEdgeVisible(int edgeID, bool visible);
    void addNode(const NodeStructure& node);
    void addEdge(const EdgeStructure& edge);

    void removeNode(int id);
    void removeEdge(int id);

    void updateNode(const NodeStructure& update);
    void translateEdge(int id, bool isStart, QPointF vector);

    void stickEdgeToNode(int edgeID, bool start, int nodeID);
    int collidingNodeID(int edgeID, bool start);

private:
    Node* nodeAt(const QPointF& pos);
    Edge* edgeAt(const QPointF& pos);

    QString layerColor(QString predicateSet);
    QString stateColor(QString predicateState);

    typedef QHash<int, Node*> NodeContainer;
    typedef QHash<int, Edge*> EdgeContainer;

    int newNodeID();
    int newEdgeID();

    NodeContainer dgwNodes;
    EdgeContainer dgwEdges;

    DiagramMode myMode;


    QGraphicsLineItem* tempLine;

    void leftClick(QPointF pos);
    void rightClick(QPointF pos);

    int collidingNodeID(QPointF coord);
    int collidingEdgeID(QPointF coord);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
}

KAVI_NS_END

#endif // DIAGRAMWIDGET_H
