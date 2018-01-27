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
