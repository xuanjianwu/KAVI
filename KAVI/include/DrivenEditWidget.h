/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : this is the DrivenEditWidget inherited from EditWidget, only AssocEdge
 *          is allowed in OperatorsEdit and ProblemsEdit
*/

#ifndef DRIVENEDITWIDGET_H
#define DRIVENEDITWIDGET_H

#include "KAVIBase.h"
#include "EditWidget.h"

KAVI_NS_BEGIN

class DrivenEditWidget: public EditWidget {
    Q_OBJECT

public:
    /*
    * construct function
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    DrivenEditWidget(QWidget* parent = 0);

    /*
    * set the definition DataWidget for DrivenEditWidget, all changes
    * in DrivenEditWidget should depend on the definition DataWidget,
    * or should be compatible with the deinition DataWidget
    * @params:
    *       definition - specified definition DataWidget
    * @return: void
    */
    void setDefinition(const DataWidget* definition);
protected:
    // the dependent definition DataWidget
    const DataWidget* xmlDefinition;

    /*
    * make connection, update edge and associated nodes in DataWidget
    * hanle one type of connection:
    * emit sceneChanged(AssocEdgeAdded)
    * @params:
    *       edge    - the edge to make connection
    *       argNum  - the associated argument's number for association edge
    * @return: void
    */
    void makeConnection(EdgeStructure& edge, int& argNum);
    /*
    * change connection, update edge and associated nodes in DataWidget
    * hanle one type of connection:
    * emit sceneChanged(AssocEdgeReconnected)
    * @params:
    *       edge    - the edge to change connection
    *       isStart - the orientation for edgepoint
    *       toNode  - the edgepoint's new connected nodeID
    *       argNum  - the argument's count of toNode in case that association edge with toNode is predicate
    * @return: void
    */
    void changeConnection(EdgeStructure& edge, bool isStart, int toNode, int& argNum);

    /*
    * delete connection, update edge and associated nodes in DataWidget
    * handle one type of connections:
    * emit sceneChanged(AssocEdgeDeleted)
    * @params:
    *       edgeID - the deleted edge's ID
    * @return: void
    */
    void deleteConnection(int edgeID);

    /*
    * verify edge's purpose, and argument's compatibility with precidate
    * first call EditWidget::verifyEdge for purpose verify and switch edge's orientation
    * @params:
    *       edge    - the edge to verify
    *       argNum  - set the new argument number
    * @return: void
    */
    bool verifyEdge(EdgeStructure& edge, int& argNum);

    /*
    * verify the reconnection's type: (1)NoChange, cancel the reconnection and reset DiagramWidget
    * (2) PosChange, cancel the reconnection and update the edgepoint's position
    * (3) PosAssocChange, pass the reconnection and update DataWidget
    * @params:
    *       edge       - the edge of old connection
    *       newNodeID  - the new node connected by the move edgepoint
    *       startMoved - the moved edgepoint's orientation
    *       argNum     - set the new argument number
    * @return: void
    */
    ReconnectionValue verifyReconnection(EdgeStructure& edge, int newNodeID,
                                bool startMoved, int& argNum);

private:

    /*
    * fint a compatible position for the new argument of predicate
    * first, match the current exist argument of predicate with definition's predicates
    * second, if match succeed, fint a compatible position for the new argument
    * @params:
    *       predicate - the predicate to find position to add the new argument
    *       argument  - the new add argument's name
    *       instArg   - the current exist argument of predicate
    * @return: the compatible position to insert new argument
    */
    int findArgPosition(const QString& predicate, const QString& argument,
                        const QHash<int, QString>& instArg);
}

KAVI_NS_END

#endif // DRIVENEDITWIDGET_H
