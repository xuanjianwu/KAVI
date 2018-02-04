#include "EdgeStructure.h"
#include "EdgePoint.h"



EdgeOrientation EdgeStructure::orientation() const
{
    if ( purpose == DEP_INHERITANCE )
    {
        return Forward;
    }
    else
    {
        return NoOrientation;
    }
}

QPen EdgeStructure::pen() const
{
    if (purpose == DEP_ASSOCIATION)
            return QPen(QColor(EC_ASSOCIATION), EDGE_LINE_WIDTH, Qt::SolidLine);

    if (purpose == DEP_INHERITANCE)
            return QPen(QColor(EC_INHERITANCE), EDGE_LINE_WIDTH, Qt::DashLine);

    qWarning() << "EdgeStructure::pen : can't decide edge pen - invalid edge purpose specification";
    return QPen();
}

void EdgeStructure::switchOrientation()
{
    QPointF tempPos = startPos;
    startPos = endPos;
    endPos = tempPos;

    int tempID = startNodeID;
    startNodeID = endNodeID;
    endNodeID = tempID;
}


