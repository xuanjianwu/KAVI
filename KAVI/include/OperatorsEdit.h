/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the operator view inherited from DrivenEditWidget
*/

#ifndef OPERATORSEDIT_H
#define OPERATORSEDIT_H

#include "KAVIBase.h"
#include "DrivenEditWidget.h"



class OperatorsEdit: public DrivenEditWidget {
    Q_OBJECT

public:
    /*
    * construct function, set the allowed node's mask and allowed edge's mask
    * and reset the dependent definition DataWidget for the Operators
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    OperatorsEdit(QWidget* parent = 0);

    void defineOperatorFromPlanningDialog(QString newPredicateName, QList<QString> variableName, QList<QString> variableClass, bool predicateSet);

private:
    /*
    * define a new variable node base on the class type in Definition, add new
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
};



#endif // OPERATORSEDIT_H
