/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the problem view inherited from DrivenEditWidget
*/

#ifndef PROBLEMSEDIT_H
#define PROBLEMSEDIT_H

#include "KAVIBase.h"
#include "DrivenEditWidget.h"

KAVI_NS_BEGIN

class ProblemsEdit: public DrivenEditWidget {
    Q_OBJECT

public:
    ProblemsEdit(QWidget* parent = 0);

    void setTaskState(PredicateState state);
    void setPredicatesVisible(bool visible, PredicateState selector);

private:
    PredicateState taskState;
    void defineRectangleNode(QPointF pos, int newID);
    void defineEllipseNode(QPointF pos, int newID);
    void displayInfo(int nodeID);
}

KAVI_NS_END

#endif // PROBLEMSEDIT_H
