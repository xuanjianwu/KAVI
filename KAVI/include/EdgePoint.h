/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define Edge Point, include the property of diagram
*/

#ifndef EDGEPOINT_H
#define EDGEPOINT_H

#include "KAVIBase.h"
#include "DiagramElement.h"

KAVI_NS_BEGIN

class EdgePoint: public DiagramElement {
public:
    /*
     * construct function
     * @params:
     *      pos     - EdgePoint's position
     *      isStart - EdgePoint's orientation
     *      parent  - EdgePoint's parent object
     * @return:
    */
    EdgePoint(const QPointF& pos, bool isStart, QGraphicsItem* parent = 0);

    /*
     * move the EdgePoint by vector
     * @params:
     *      vector - target vector
     * @return: void
    */
    void moveBy(const QPointF& vector);

    /*
     * get EdgePoint's color
     * @params: N/A
     * @return: QColor
    */
    QColor getColor() const;

    /*
     * set EdgePoint's color
     * @params:
     *      newColor - target color
     * @return: void
    */
    void setColor(QColor newColor);

    /*
     * set EdgePoint' shape and init arrow vector if necessary
     * @params:
     *      shape - target shape
     * @return: void
    */
    void setShape(EdgePointShape shape);

    /*
     * rewrite from QGraphicsItem, get the bound of edge point as a rectangle
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
     * update the line angle
     * @params:
     *      angle - the new angle
     * @return: void
    */
    void updateAngle(double angle);

    /*
     * rewrite from QGraphicsItem, paints the contents of edge point
     * @params:
     *      painter - the painter to paint
     *      option  - unused
     *      widget  - unused
     * @return: void
    */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    // specify the EdgePoint's orientation
    bool start;
    // specify the EdgePoint's shape
    EdgePointShape edgepointShape;
    // specify the EdgePoint's color
    QColor color;
    // the vector of the three points in arrow which present as a triangle
    QPolygonF polygon;
    // specify the line angle
    double lineAngle;

    /*
     * init the polygon of the triangle arrow's three points
     * @params: N/A
     * @return: void
    */
    void initPolygon();
};

KAVI_NS_END

#endif // EDGEPOINT_H
