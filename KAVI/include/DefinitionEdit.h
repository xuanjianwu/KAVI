/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the definition view inherited from EditWidget
*/

#ifndef DEFINITIONEDIT_H
#define DEFINITIONEDIT_H

#include "KAVIBase.h"
#include "EditWidget.h"

KAVI_NS_BEGIN

class DefinitionEdit: public EditWidget {
    Q_OBJECT

public:
    DefinitionEdit(QWidget* parent = 0);

private:
    void defineRectangleNode(QPointF pos, int newID);
    void defineEllipseNode(QPointF pos, int newID);

    void renumberPredicateArguments(QDomElement toRenumber);

    void displayInfo(int nodeID);
    void makeConnection(EdgeStructure& edge, int& argNum);
    void changeConnection(EdgeStructure& edge, bool isStart, int toNode, int& argNum);
    void deleteConnection(int edgeID);

    bool verifyEdge(EdgeStructure& edge, int& argNum);
    ReconnectionValue verifyReconnection(EdgeStructure& edge, int newNode,
                                bool startMoved, int& argNum);
}

KAVI_NS_END

#endif // DEFINITIONEDIT_H
