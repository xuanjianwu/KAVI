#include "EdgePoint.h"
#include "DiagramEvents.h"
#include <math.h>

const double Pi = 3.14159;

EdgePoint::EdgePoint(const QPointF& pos, bool isStart, QGraphicsItem* parent)
    :DiagramElement(parent)
{
    start = isStart;
    setPos(pos);
    color = Qt::green;
    edgepointShape = Point;
    lineAngle = 0.0;

    setZValue(EDGEPOINT_Z_VALUE);
    setFlags(ItemIsSelectable|ItemIsMovable);
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void EdgePoint::moveBy(const QPointF& vector) {
    QGraphicsItem::moveBy(vector.x(), vector.y());
}

QColor EdgePoint::getColor() const
{
    return color;
}

void EdgePoint::setColor(QColor newColor)
{
    color = newColor;
}

void EdgePoint::setShape(EdgePointShape shape)
{
    edgepointShape = shape;

    if ((shape == ArrowPos) || (shape == ArrowNeg))
    {
        initPolygon();
    }
}

QRectF EdgePoint::boundingRect() const
{
    QTransform matrix;

    switch(edgepointShape)
    {
        case Point:
            return QRectF(-EDGEPOINT_SIZE,-EDGEPOINT_SIZE,2*EDGEPOINT_SIZE,2*EDGEPOINT_SIZE);
        break;
        case ArrowPos:
        case ArrowNeg:
            matrix.rotate(-(180/Pi)*lineAngle);
            return (polygon*matrix).boundingRect();
        break;
        default:

            Q_ASSERT(false);
            return QRect();
    }
}

QPainterPath EdgePoint::shape() const
{
    QPainterPath path;
    QTransform matrix;

    switch(edgepointShape)
    {
        case Point:
            path.addEllipse(boundingRect());
        break;
        case ArrowPos:
        case ArrowNeg:
            matrix.rotate(-(180/Pi)*lineAngle);
            path.addPolygon(polygon*matrix);
        break;
        default:

            Q_ASSERT(false);
    }

    return path;
}

void EdgePoint::updateAngle(double angle)
{
    lineAngle = angle;
    update();
}

void EdgePoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setPen(QPen(Qt::gray,1));
    painter->setBrush(color);

    switch(edgepointShape)
    {
        case Point:
            painter->drawEllipse(boundingRect());
        break;
        case ArrowPos:
        case ArrowNeg:
            painter->save();
            painter->rotate(-(180/Pi)*lineAngle);
            painter->drawPolygon(polygon);
            painter->restore();
        break;
        default:

            Q_ASSERT(false);
    }
}

void EdgePoint::initPolygon()
{
    polygon = QPolygonF();

    QPointF arrowP0;
    QPointF arrowP1;
    QPointF arrowP2;

    if(sin(lineAngle) >= 0)
        lineAngle = (2*Pi) - lineAngle;

    switch(edgepointShape)
    {
        /* ArrowPos: ????----> */
        case ArrowPos:
            arrowP0 = QPointF(10*cos(lineAngle),10*sin(lineAngle));
            arrowP1 = arrowP0 - QPointF(sin(lineAngle + Pi / 3) * ARROW_SIZE,
                        cos(lineAngle + Pi / 3) * ARROW_SIZE);
            arrowP2 = arrowP0 - QPointF(sin(lineAngle + Pi - Pi / 3) * ARROW_SIZE,
                        cos(lineAngle + Pi - Pi / 3) * ARROW_SIZE);
        break;
        /* ArrowNeg: ????----< */
        case ArrowNeg:
            arrowP0 = QPointF(-10*cos(lineAngle),-10*sin(lineAngle));
            arrowP1 = arrowP0 + QPointF(sin(lineAngle + Pi / 3) * ARROW_SIZE,
                        cos(lineAngle + Pi / 3) * ARROW_SIZE);
            arrowP2 = arrowP0 + QPointF(sin(lineAngle + Pi - Pi / 3) * ARROW_SIZE,
                        cos(lineAngle + Pi - Pi / 3) * ARROW_SIZE);
        break;
        default:

            Q_ASSERT(false);
    }

    polygon << arrowP0 << arrowP1 << arrowP2;
}


