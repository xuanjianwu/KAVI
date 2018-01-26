/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define Edge structure base on Edge, adding
 *          the user application's infomation
*/

#ifndef EDGESTRUCTURE_H
#define EDGESTRUCTURE_H

#include "KAVIBase.h"
#include "Edge.h"

KAVI_NS_BEGIN

struct EdgeStructure {
    // Edge structure ID
    int id;
    // Edge purpose
    char purpose;

    // start position
    QPointF startPos;
    // End position
    QPointF endPos;

    // start Node ID
    int startNodeID;
    // end Node ID;
    int endNodeID;

    /*
     * get orientation of Edge structure
     * @params: N/A
     * @return: EdgeOrientation
    */
    EdgeOrientation orientation() const;

    /*
     * get the pen of Edge structure
     * @params: N/A
     * @return: QPen
    */
    QPen  pen() const;

    /*
     * switch the orientation of Edge structure
     * @params: N/A
     * @return: N/A
    */
    void switchOrientation();
};

KAVI_NS_END

#endif // EDGESTRUCTURE_H
