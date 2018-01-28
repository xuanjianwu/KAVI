/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : The EditWidget class is inherited from the QGraphicsView class,
 *          serves as the Controller in MVC.
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

    /*
    * construct function
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    EditWidget(QWidget* parent = 0);

    /*
    * reset DataWidget, changed value, and DataWidget will reset DiagramWidget
    * @params: N/A
    * @return: N/A
    */
    void reset();

    /*
    * return the changed value
    * @params: N/A
    * @return: is changed? bool
    */
    bool wasChanged();

    /*
    * set the edit mode, it will also set the diagram mode
    * @params:
    *       mode - specified edit mode
    * @return: void
    */
    void setEditMode(EditMode mode);

    /*
    * get the pointer of DataWidget
    * @params: N/A
    * @return: DataWidget's pointer
    */
    const DataWidget* xmlDataPointer();

signals:
    /*
    * this signal is occur by EditWidget and its inheritor,
    * this signal is handled by the KAVIMainWindow class, such
    * as on_definitionEdit_sceneChanged(int changeCode);
    * @params:
    *       changeCode - the changeCode carried by signal
    * @return: void
    */
    void sceneChanged(int changeCode);

    /*
    * this signal is occur by EditWidget's inheritor,
    * this signal is handled by the KAVIMainWindow class, such
    * as on_definitionEdit_updateInfoPanel(QWidget * infoWidget);
    * @params:
    *       infoWidget - the new infoWidget
    * @return: void
    */
    void updateInfoPanel(QWidget* infoWidget);

public slots:

    /*
    * set the changed value
    * this slot is called by handleExternChange, and the KAVIMainWindow
    * class, such as ui.definitionEdit->setChanged(false);
    * @params:
    *       value - specified value
    * @return: void
    */
    void setChanged(bool value);

    /*
    * handle extern change and set the changed value
    * this slot is called by the connect from InfoPanel's signal:madeChange
    * @params:
    *       changeCode - specified changeCode
    * @return: void
    */
    void handleExternChange(int changeCode);

    /*
    * load XML data to DataWidget and reset changed value
    * @params:
    *       diagramDocuement - specified XML document
    * @return: void
    */
    void loadXMLdata(QDomDocument diagramDocument);

protected:

    enum ReconnectionValue {
        NoChange,
        PosChange,
        PosAssocChange
    };

    // current edit mode
    EditMode editMode;

    // the associated Model of MVC
    DataWidget* xmlData;

    // the associated View of MVC
    DiagramWidget* diagram;

    // the name matching using regular expressions
    QRegExp nameChecker;

    //
    char allowedNodeMask;
    //
    char allowedEdgeMask;

    // specify if the EditWidget has catch change
    bool changed;

    /*
    * This function receives events to an object and should return true if the event was recognized and processed.
    * The event() function can be reimplemented to customize the behavior of an object.
    * Make sure you call the parent event class implementation for all the events you did not handle.
    * handle the events from the DiagramWidget, for example:
    *   DiagramEvent myEvent(DiagramRightClick, INVALID_ID, pos);
    *   QApplication::sendEvent(this->parent(), &myEvent);
    * @params:
    *       event - specified eventt
    * @return: the state of handling event
    */
    bool event(QEvent* event);

    /*
    * handle node reshape, stick the connected edge to the after-reshape node
    * in the DiagramWidget, but because the node's position does not change,
    * so it is no need to update DataWidget
    * @params:
    *       id - specified node's ID
    * @return: void
    */
    void handleNodeReshape(int id);

    /*
    * handle node move, change the node position and connected edges' position
    * in DataWidget
    * @params:
    *       id      - specified node's ID
    *       vector  - vector of move
    * @return: void
    */
    void handleNodeMove(int id, QPointF vector);

    /*
    * handle node drag, node drag is the process before node move, during the
    * node drag, no need to update position in DataWidget, update position will
    * be done while node move occurs, just translate connected edges in DiagramWidget
    * @params:
    *       id      - specified node's ID
    *       vector  - vector of drag
    * @return: void
    */
    void handleNodeDrag(int id, QPointF vector);

    /*
    * handle edge move, after edge move: (1): edge move means nothing or is not allowed, nochange
    * and reset edge in DiagramWidget; (2) edge move only causes that edgepoint moves in the node,
    * so only change edge position in DataWidget; (3) edge move causes change connection, need to
    * delete old connection and make new connection in DataWidget
    * @params:
    *       id      - move edge's ID
    *       isStart - edge's orientation
    *       vector  - vector of move
    * @return: void
    */
    void handleEdgeMove(int id, bool isStart, QPointF vector);

    /*
    * define a new edge, after verify, make new connection in DataWidget
    * @params:
    *       startID    - start node ID
    *       endID      - end node ID
    *       startPoint - start node position
    *       endPoint   - end node position
    *       newID      - ID for new edge
    * @return: void
    */
    void defineEdge(int startID, int endID, QPointF startPoint, QPointF endPoint, int newID);
    virtual void defineRectangleNode(QPointF pos, int newID) = 0;
    virtual void defineEllipseNode(QPointF pos, int newID) = 0;

    /*
    * delete node and associated edges in DataWidget
    * emit associated sceneChanged signal with changeCode
    * @params:
    *       nodeID - delete node's ID
    * @return: void
    */
    void deleteNode(int nodeID);

    /*
    * delete edge in DataWidget
    * emit associated sceneChanged signal with changeCode
    * @params:
    *       edgeID - delete edge's ID
    * @return: void
    */
    void deleteEdge(int edgeID);
    virtual void displayInfo(int nodeID) = 0;

    /*
    * determine the purpose of edge
    * @params:
    *       startType - type of start node
    *       endType   - type of end node
    * @return: edge purpose
    */
    char determineEdgePurpose(char startType, char endType);

    /*
    * verify the edge, only allowed edge can pass the verify. In case of diagram,
    * the edge's orientation between predicate and class,variable,object does not
    * matter, by default, such edge should start from predicate, otherwise switch
    * orientation.
    * @params:
    *       edge   - the edge to verify
    *       argNum - unused arg number
    * @return: void
    */
    virtual bool verifyEdge(EdgeStructure& edge, int& argNum);
    virtual ReconnectionValue verifyReconnection(EdgeStructure& edge,
                                                 int newNode, bool startMoved, int& argNum) = 0;

    virtual void makeConnection(EdgeStructure& edge, int& argNum) = 0;
    virtual void changeConnection(EdgeStructure& edge, bool isStart, int toNode, int& argNum) = 0;
    virtual void deleteConnection(int edgeID) = 0;
}

KAVI_NS_END

#endif // EDITWIDGET_H
