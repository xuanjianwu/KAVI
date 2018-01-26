/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define Edge base on Edge Point, Node, DiagramElement,
 *          only include the basic property of diagram.
*/

#ifndef EDGE_H
#define EDGE_H

#include "KAVIBase.h"

KAVI_NS_BEGIN

class Node;
class DiagramElement;
class EdgePoint;

class Edge: public QGraphicsLineItem {
public:
    /*
     * construct function
     * @params: N/A
     * @return: N/A
    */
    Edge();

    /*
     * construct function
     * @params:
     *      line  - the line to init Edge
     *      id    - Edge ID
     *      scene - the scene which Edge belongs to
     *      host  - the event target of Edge
     * @return: N/A
    */
    Edge(const QLineF& line, int id, QGraphicsScene* scene = 0, QObject* host = 0);

    /*
     * is the Edge ID a valid ID?
     * @params: N/A
     * @return: bool
    */
    bool isValid();

    /*
     * set the Edge's position
     * @params:
     *      newPos  - new position
     *      isStart - Edge's orientation
     * @return: void
    */
    void setPos(const QPointF& newPos, bool isStart);

    /*
     * move the Edge by vector
     * @params:
     *      vector  - target vector
     *      isStart - Edge's orientation
     * @return: void
    */
    void moveBy(const QPointF& vector, bool isStart);

    /*
     * stick the Edge to another diagram element
     * @params:
     *      another - target diagram element
     *      isStart - Edge's orientation
     * @return: void
    */
    void stickTo(const DiagramElement* another, bool isStart);

    /*
     * set Edge's orientation
     * @params:
     *      orientation - target orientation
     * @return: void
    */
    void setEdgeOrientation(EdgeOrientation orientation);

    /*
     * set EdgePoints' color
     * @params:
     *      color - target color
     * @return: void
    */
    void setPointColor(QColor color);

    /*
     * get Edge's ID
     * @params: N/A
     * @return: the ID in int
    */
    int getID();

    /*
     * get the connected Node's ID
     * @params:
     *      is Start - EdgePoint's orientation
     *      nodeList - the Nodes to search
     * @return: the target Node's ID
    */
    int connectedNodeID(bool isStart, QList<Node*> nodeList);

    /*
     * get the Edge's shape
     * @params: N/A
     * @return: QPainterPath
    */
    QPainterPath shape() const;

    /*
     * rewrite from QGraphicsLineItem, get the bound of edge as a rectangle
     * @params: N/A
     * @return: QRectF
    */
    QRectF boundingRect() const;

    // start point of Edge
    EdgePoint * start;
    // end point of Edge
    EdgePoint * end;

private:
    // Edge ID
    int edgeId;
    // event target of Edge
    QObject * eventTarget;
    // grab start position
    QPointF startGrabPos;
    // grab end position
    QPointF endGrabPos;

    /*
     * get angle of Edge
     * @params: N/A
     * @return: angle in radian measure
    */
    double getAngle();

    /*
     * filters events for the item watched
     * @params:
     *      watched - the item to set filters events for
     *      event   - the filtered event
     * @return: true to prevent further processing, false to be propagated further
    */
    bool sceneEventFilter(QGraphicsItem* watched, QEvent* event);

    /*
     * handle EdgePoint's click event
     * @params:
     *      isStart - EdgePoint's orientation
     *      pos     - position where click occur
     * @return: void
    */
    void handleClickEvent(bool isStart, QPointF pos);
};

KAVI_NS_END

#endif // EDGE_H
