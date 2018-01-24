/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : this is the driven EditWidget inherited from EditWidget
*/

#ifndef DRIVENEDITWIDGET_H
#define DRIVENEDITWIDGET_H

#include "KAVIBase.h"
#include "EditWidget.h"

KAVI_NS_BEGIN

class DrivenEditWidget: public EditWidget {
    Q_OBJECT

public:
    DrivenEditWidget(QWidget* parent = 0): EditWidget(parent) {}

    void setDefinition(const DataWidget* definition);
protected:
    const DataWidget* xmlDefinition;

    void makeConnection(EdgeStructure& edge, int& argNum);
    void changeConnection(EdgeStructure& edge, bool isStart, int toNode, int& argNum);
    void deleteConnection(int edgeID);
    bool verifyEdge(EdgeStructure& edge, int& argNum);
    ReconnectionValue verifyReconnection(EdgeStructure& edge, int newNodeID,
                                bool startMoved, int& argNum);

private:
    int findArgPosition(const QString& predicate, const QString& argument,
                        const QHash<int, QString>& instArg);
}

KAVI_NS_END

#endif // DRIVENEDITWIDGET_H
