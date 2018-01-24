/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the operator view inherited from DrivenEditWidget
*/

#ifndef OPERATORSEDIT_H
#define OPERATORSEDIT_H

#include "KAVIBase.h"
#include "DrivenEditWidget.h"

KAVI_NS_BEGIN

class OperatorsEdit: public DrivenEditWidget {
    Q_OBJECT

public:
    OperatorsEdit(QWidget* parent = 0);

private:
    void defineRectangleNode(QPointF pos, int newID);
    void defineEllipseNode(QPointF pos, int newID);
    void displayInfo(int nodeID);
}

KAVI_NS_END

#endif // OPERATORSEDIT_H
