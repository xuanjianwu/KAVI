/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the definition view inherited from EditWidget
*/

#ifndef DEFINITIONEDIT_H
#define DEFINITIONEDIT_H

#include "KAVIBase.h"
#include "EditWidget.h"
#include "KAVIClassKB.h"
#include "KAVIPredicateKB.h"


class DefinitionEdit: public EditWidget {
    Q_OBJECT

public:
    /*
    * construct function, set the allowed node's mask and allowed edge's mask
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    DefinitionEdit(QWidget* parent = 0);

    /*
    * update the cached knowledge base, and save knowledge base to file
    * @params: N/A
    * @return: void
    */
    void saveKB();

    /*
    * get the current dependent class knowledge base
    * @params: N/A
    * @return: knowleage base
    */
    KAVIClassKB *getClassKB() const;

    /*
    * get the current dependent predicate knowledge base
    * @params: N/A
    * @return: knowledge base
    */
    KAVIPredicateKB *getPredicateKB() const;

private:
    // the dependent class knowledgeBase
    KAVIClassKB *classKB;
    // the dependent predicate knowledgeBase
    KAVIPredicateKB *predicateKB;

    /*
    * define a new class node, add new node data to DataWidget,
    * and DataWidget will add it to DiagramWidget
    * and emit sceneChanged(RectNodeAdded)
    * @params:
    *       pos   - the new node's position
    *       newID - the new node's ID
    * @return: void
    */
    void defineRectangleNode(QPointF pos, int newID);

    /*
    * define a new predicate node, add new node data to DataWidget,
    * and DataWidget will add it to DiagramWidget
    * and emit sceneChanged(EllipseNodeAdded)
    * @params:
    *       pos   - the new node's position
    *       newID - the new node's ID
    * @return: void
    */
    void defineEllipseNode(QPointF pos, int newID);

    /*
    * renumber the order of predicate's argument, now it just minus 1（-1）
    * of the specified argument element, and update the number of the next
    * sibling Element in order
    * used after disconnectEdgeFromNode
    * @params:
    *       toRenumber - the specified argument element to renumber
    * @return: void
    *
    void renumberPredicateArguments(QDomElement toRenumber);
    */

    /*
    * create the associated InfoPanel for node with DataWidget's Data
    * and emit updateInfoPanel(infoWidget)
    * @params:
    *       nodeID - specified node's ID
    * @return: void
    */
    void displayInfo(int nodeID);

    /*
    * make connection, update edge and associated nodes in DataWidget
    * hanle two types of connections:
    * emit sceneChanged(InherEdgeAdded)
    * emit sceneChanged(AssocEdgeAdded)
    * @params:
    *       edge    - the edge to make connection
    *       argNum  - the associated argument's number for association edge
    * @return: void
    */
    void makeConnection(EdgeStructure& edge, int& argNum);

    /*
    * change connection, update edge and associated nodes in DataWidget
    * hanle two types of connections:
    * emit sceneChanged(AssocEdgeReconnected)
    * emit sceneChanged(InherEdgeReconnected)
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
    * handle two types of connections:
    * emit sceneChanged(AssocEdgeDeleted)
    * emit sceneChanged(InherEdgeDeleted)
    * @params:
    *       edgeID - the deleted edge's ID
    * @return: void
    */
    void deleteConnection(int edgeID);

    /*
    * verify edge's purpose,Multiple inheritance,Cyclic inheritance,overloaded predicates
    * first call EditWidget::verifyEdge for purpose verify and switch edge's orientation
    * @params:
    *       edge    - the edge to verify
    *       argNum  - set the new argument number in case DEP_ASSOCIATION
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
};



#endif // DEFINITIONEDIT_H
