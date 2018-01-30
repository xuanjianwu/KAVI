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
    /*
    * construct function, set the allowed node's mask and allowed edge's mask
    * and reset the dependent definition DataWidget for the Operators
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    ProblemsEdit(QWidget* parent = 0);

    /*
    * set the current task state. init or goal or nostate
    * @params:
    *       state  - specified state
    * @return: void
    */
    void setTaskState(PredicateState state);

    /*
    * set the predicate's visibility
    * @params:
    *       visible  - the visibility
    *       selector - the selected predicate
    * @return: void
    */
    void setPredicatesVisible(bool visible, PredicateState selector);

private:
    // the current predicate state
    PredicateState taskState;

    /*
    * define a new object node base on the class type in Definition, add new
    * node data to DataWidget, and DataWidget will add it to DiagramWidget
    * and emit sceneChanged(RectNodeAdded)
    * @params:
    *       pos   - the new node's position
    *       newID - the new node's ID
    * @return: void
    */
    void defineRectangleNode(QPointF pos, int newID);

    /*
    * define a new predicate node base on the predicate in defintion, add new
    * node data to DataWidget, and DataWidget will add it to DiagramWidget
    * and emit sceneChanged(EllipseNodeAdded)
    * @params:
    *       pos   - the new node's position
    *       newID - the new node's ID
    * @return: void
    */
    void defineEllipseNode(QPointF pos, int newID);

    /*
    * create the associated InfoPanel for node with DataWidget's Data
    * and emit updateInfoPanel(infoWidget)
    * @params:
    *       nodeID - specified node's ID
    * @return: void
    */
    void displayInfo(int nodeID);
}

KAVI_NS_END

#endif // PROBLEMSEDIT_H
