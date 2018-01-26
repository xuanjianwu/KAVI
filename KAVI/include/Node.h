/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define the basic node, only include the basic
 *          property of diagram
*/

#ifndef NODE_H
#define NODE_H

#include "KAVIBase.h"
#include "DiagramElement.h"

KAVI_NS_BEGIN

class Node: public DiagramElement {
public:
    /*
     * construct function
     * @params: N/A
     * @return: N/A
    */
    Node();

    /*
     * construct function
     * @params:
     *      nID     - ID of Node
     *      nShape  - shape of Node
     *      scene   - the graphic scene which Node belongs to
     *      host    - the event target of Node
     * @return:
    */
    Node(int nID, NodeShape nShape, QGraphicsScene * scene = 0, QObject * host = 0);

    /*
     * is the Node ID a valid ID?
     * @params: N/A
     * @return: bool
    */
    bool isValid();

    /*
     * get the ID of Node
     * @params: N/A
     * @return: int
    */
    int getID();

    /*
     * get the color of Node
     * @params: N/A
     * @return: Qcolor
    */
    const QColor& getColor();

    /*
     * set the color of Node
     * @params:
     *      color - target color
     * @return: void
    */
    void setColor(const QColor& color);

    /*
     * get the label of Node
     * @params: N/A
     * @return: QString
    */
    const QString getLabel();

    /*
     * set the label of Node and update node's size
     * @params:
     *      label - target label
     * @return: void
    */
    void setLabel(const QString& label);

    /*
     * add a color layer to Node
     * @params:
     *      col - target named color
     * @return: void
    */
    void setLayer(const QString& col);

    /*
     * clear all the color layers of Node
     * @params: N/A
     * @return: N/A
    */
    void clearLayers();

    /*
     * rewrite from QGraphicsItem, get the bound of Node as a rectangle
     * @params: N/A
     * @return: QRectF
    */
    QRectF boundingRect() const;

    /*
     * get the shape of diagram
     * @params: N/A
     * @return: QPaintPath
    */
    QPainterPath shape() const;

    /*
     * rewrite from QGraphicsItem, paints the contents of Node
     * @params:
     *      painter - the painter to paint
     *      option  - unused
     *      widget  - unused
     * @return: void
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    /*
     * handle mouse move event
     * @params:
     *      event - QGraphicsSceneMouseEvent
     * @return: void
    */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /*
     * handle mouse press event
     * @params:
     *      event - QGraphicsSceneMouseEvent
     * @return: void
    */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*
     * handle mouse release event
     * @params:
     *      event - QGraphicsSceneMouseEvent
     * @return: void
    */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /*
     * rewrite from QGraphicsItem, notify custom items that some part of the item's state changes
     * @params:
     *      change - parameter of the item that is changing
     *      value  - the new value, the type of the value depends on change
     * @return: the changed value or value
    */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    // Id of Node
    int id;
    // event target of Node
    QObject* eventTarget;
    // named color layers of Node/color's format in string
    QSet<QString> nodeLayers;
    // size of Node
    QSize nodeSize;
    // Node's label
    QGraphicsSimpleTextItem* nodeLabel;
    // shape of Node
    NodeShape nodeShape;
    // color of Node
    QColor color;
    // the start position while drag
    QPointF dragStartPos;

    /*
     * draw the layers of Node
     * @params:
     *      painter - the painter to draw
     * @return: void
    */
    void drawLayers(QPainter* painter);
};

KAVI_NS_END

#endif // NODE_H
