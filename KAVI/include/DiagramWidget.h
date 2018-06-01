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

    /*
     * set the diagram mode
     * @params:
     *      mode   - the target mode
     * @return: N/A
    */
    void setDiagramMode(DiagramMode mode);

    /*
     * set the target node's visiblity
     * @params:
     *      nodeID   - the target node's ID
     *      visible  - the target visiblity
     * @return: N/A
    */
    void setNodeVisible(int nodeID, bool visible);

    /*
     * set the target edge's visiblity
     * @params:
     *      edgeID   - the target edge's ID
     *      visible  - the target visiblity
     * @return: N/A
    */
    void setEdgeVisible(int edgeID, bool visible);

    /*
     * add new node to the diagram view
     * @params:
     *      node - the new node
     * @return: N/A
    */
    void addNode(const NodeStructure& node);

    /*
     * add new edge to the diagram view
     * @params:
     *      edge - the new edge
     * @return: N/A
    */
    void addEdge(const EdgeStructure& edge);

    /*
     * remove node from diagram view
     * @params:
     *      id - the target node's ID
     * @return: N/A
    */
    void removeNode(int id);

    /*
     * remove edge from diagram view
     * @params:
     *      id - the target edge's ID
     * @return: N/A
    */
    void removeEdge(int id);

    /*
     * update the node in diagram view
     * @params:
     *      update - the target node structure
     * @return: N/A
    */
    void updateNode(const NodeStructure& update);

    /*
     * transfer edge by vector
     * @params:
     *      id      - the target edge's ID
     *      isStart - the edge orientation
     *      vector  - the target vector
     * @return: N/A
    */
    void translateEdge(int id, bool isStart, QPointF vector);

    /*
     * stick the edge to node
     * @params:
     *      edgeID - the target edge's ID
     *      start  - the edge orientation
     *      nodeID - the target node's ID
     * @return: N/A
    */
    void stickEdgeToNode(int edgeID, bool start, int nodeID);

    /*
     * get the colliding node's ID with edge
     * @params:
     *      edgeID - the edge's ID
     *      start  - the edge orientation
     * @return: the colliding node's id
    */
    int collidingNodeID(int edgeID, bool start);

    /*
     * get a new node's id
     * @params: N/A
     * @return: new node's id
    */
    int newNodeID();

    /*
     * get a new edge's id
     * @params: N/A
     * @return: new edge's id
    */
    int newEdgeID();

    /*
    * get a new node position against the root
    * @params:
    *       root - the specified position to against
    * @return: the new position
    */
    QPointF newNodePos(QPointF root);

    /*
    * get the specified Node*
    * @params:
    *       nodeId - the expected node's ID
    * @return: the expected Node*
    */
    Node* getNode(int nodeID);

private:
    /*
     * get the node at point
     * @params:
     *      pos - the target point
     * @return: the target node
    */
    Node* nodeAt(const QPointF& pos);

    /*
     * get the edge at point
     * @params:
     *      pos - the target point
     * @return: the target edge
    */
    Edge* edgeAt(const QPointF& pos);

    /*
     * get the layer color of specified predicate set
     * @params:
     *      predicateSet - the specified predicate set
     * @return: layer color
    */
    QString layerColor(QString predicateSet);

    /*
     * get the state color of specified predicate state
     * @params:
     *      predicateState - the specified predicate state
     * @return: state color
    */
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

    /*
     * get the colliding node's ID at point
     * @params:
     *      coord - the target point
     * @return: the colliding node's id
    */
    int collidingNodeID(QPointF coord);

    /*
     * get the colliding edge's ID at point
     * @params:
     *      coord - the target point
     * @return: the colliding edge's id
    */
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
