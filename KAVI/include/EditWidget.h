/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : The EditWidget class is inherited from the QGraphicsView class,
 *          serves as the controller in MVC.
*/

#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include "KAVIBase.h"
#include "DiagramWidget.h"
#include "DataWidget.h"

KAVI_NS_BEGIN

struct EdgeStructure;

class EditWidget: public QGraphicsView {
    Q_OBJECT

public:
    EditWidget(QWidget* parent = 0);

    void reset();
    bool wasChanged();
    void setEditMode(EditMode mode);
    const DataWidget* xmlDataPointer();

signals:
    void sceneChanged(int changeCode);
    void updateInfoPanel(QWidget* infoWidget);

public slots:
    void setChanged(bool value);
    void handleExternChange(int changeCode);

    void loadXMLdata(QDomDocument diagramElement);

protected:

    enum ReconnectionValue {
        NoChange,
        PosChange,
        PosAssocChange
    };

    EditMode editMode;

    DataWidget* xmlData;
    DiagramWidget* diagram;

    QRegExp nameChecker;

    char allowedNodeMask;
    char allowedEdgeMask;

    bool changed;

    bool event(QEvent* event);
    void handleNodeReshape(int id);
    void handleNodeMove(int id, QPointF vector);
    void handleNodeDrag(int id, QPointF vector);
    void handleEdgeMove(int id, bool isStart, QPointF vector);
    void defineEdge(int startID, int endID, QPointF startPoint, QPointF endPoint, int newID);
    virtual void defineRectangleNode(QPointF pos, int newID) = 0;
    virtual void defineEllipseNode(QPointF pos, int newID) = 0;
    void deleteNode(int nodeID);
    void deleteEdge(int edgeID);
    virtual void displayInfo(int nodeID) = 0;

    char determineEdgePurpose(char startType, char endType);

    virtual bool verifyEdge(EdgeStructure& edge, int& argNum);
    virtual ReconnectionValue verifyReconnection(EdgeStructure& edge,
                                                 int newNode, bool startMoved, int& argNum) = 0;

    virtual void makeConnection(EdgeStructure& edge, int& argNum) = 0;
    virtual void changeConnection(EdgeStructure& edge, bool isStart, int toNode, int& argNum) = 0;
    virtual void deleteConnection(int edgeID) = 0;
}

KAVI_NS_END

#endif // EDITWIDGET_H
