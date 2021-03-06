/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : basic diagram element
*/

#ifndef DIAGRAMELEMENT_H
#define DIAGRAMELEMENT_H

#include "KAVIBase.h"



class DiagramElement : public QGraphicsItem {
public:
    /*
     * construct function
     * @params:
     *      parent - redefinition parent object
     * @return: N/A
    */
    DiagramElement(QGraphicsItem* parent = 0);

    /*
     * stick/move current diagram element to another element
     * step by step by the line between this two diagram
     * @params:
     *      another - diagram element
     * @return: void
    */
    void stick(const DiagramElement* another);

protected:
    /*
     * get the shape of diagram
     * @params: N/A
     * @return: QPaintPath
    */
    virtual QPainterPath shape() const = 0;
};



#endif // DIAGRAMELEMENT_H
