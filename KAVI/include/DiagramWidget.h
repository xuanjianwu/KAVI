/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : This class inherits from QGraphicsScene and it is
 *          an implementation of the View, serves as the View
 *          in MVC.
*/

#ifndef DIAGRAMWIDGET_H
#define DIAGRAMWIDGET_H

#include "KAVIBase.h"



class Node;
class Edge;
struct NodeStructure;
struct EdgeStructure;

class DiagramWidget: public QGraphicsScene {
    Q_OBJECT

public:
    /*
    * construct function
    * @params:
    *       width   - this scene rect's width
    *       height  - this scene rect's height
    *       parent  - parent object
    * @return: N/A
    */
    DiagramWidget(int width, int height, QWidget* parent = 0);

    /*
    * reset and clear nodes, edges
    * @params: N/A
    * @return: N/A
    */
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

    int newNodeID();
    int newEdgeID();
    QPointF newNodePos(QPointF root);

    Node* getNode(int nodeID);

private:
    Node* nodeAt(const QPointF& pos);
    Edge* edgeAt(const QPointF& pos);

    QString layerColor(QString predicateSet);
    QString stateColor(QString predicateState);

    // the hash table of current scene diagram's Node <nodeID, node*>
    typedef QHash<int, Node*> NodeContainer;

    // the hash table of current scene diagran's edge <edgeID, edge*>
    typedef QHash<int, Edge*> EdgeContainer;

    // container of diagram widget nodes
    NodeContainer dgwNodes;

    // container of diagram widget edges
    EdgeContainer dgwEdges;

    // the current mode of operating diagram
    DiagramMode myMode;

    // the temp line while inserting edge
    QGraphicsLineItem* tempLine;

    /*
    * handle the left click in the diagram scene and occur relevant
    * event: DiagramLeftClick, EdgeLeftClick, NodeLeftClick
    * @params:
    *       pos - the click's positin
    * @return: N/A
    */
    void leftClick(QPointF pos);

    /*
    * handle the right click in the diagram scene and occur relevant
    * event: DiagramRightClick, EdgeRightClick, NodeRightClick
    * @params:
    *       pos - the click's positin
    * @return: N/A
    */
    void rightClick(QPointF pos);

    int collidingNodeID(QPointF coord);
    int collidingEdgeID(QPointF coord);

protected:
    /*
    * handle mouse press event in diagram scene, considering the diagram
    * mode, in detail, to left click or right click
    * @params:
    *       event - the mouse event
    * @return: N/A
    */
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    /*
    * handle mouse release event in diagram scene, considering the InsertLine
    * mode, it will occur a EdgeDefined event
    * @params:
    *       event - the mouse event
    * @return: N/A
    */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    /*
    * handle mouse move event in diagram scene, considering the InsertLine
    * mode, it will update the temp line
    * @params:
    *       event - the mouse event
    * @return: N/A
    */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};



#endif // DIAGRAMWIDGET_H
