/*
 * @author: liYc
 * @date  : 2018/1/22
 * @brief : define custom events of KAVI diagram
*/

#ifndef DIAGRAMEVENTS_H
#define DIAGRAMEVENTS_H

#include "KAVIBase.h"

KAVI_NS_BEGIN

// custom events

// left click on diagram, occur from DiagramWidget
extern QEvent::Type DiagramLeftClick;
// right click on diagram, occur from DiagramWidget
extern QEvent::Type DiagramRightClick;
// left click on node, occur from DiagramWidget
extern QEvent::Type NodeLeftClick;
// right click on node, occur from DiagramWidget
extern QEvent::Type NodeRightClick;
// left click on edge, occur from DiagramWidget
extern QEvent::Type EdgeLeftClick;
// right click on edge, occur from DiagramWidget
extern QEvent::Type EdgeRightClick;
// node moves, occur from Node
extern QEvent::Type NodeMoved;
// edge moves, occur from Edge
extern QEvent::Type EdgeMoved;
// node drags, occur from Node
extern QEvent::Type NodeDrag;
// define edge, occur from DiagramWidget
extern QEvent::Type EdgeDefined;
// reshape node, occur from Node
extern QEvent::Type NodeReshaped;

class DiagramEvent: public QEvent {
public:
    /*
     * construct function
     * @params:
     *      type   - the specified event type
     *      itemID - the ID of item that trigger event
     *      data   - the data of item
     * @return: N/A
    */
    DiagramEvent(Type type, int itemID, const QVariant& data = QVariant() );

    /*
     * get the item ID
     * @params: N/A
     * @return: item ID
    */
    int itemID() const;

    /*
     * get the data of item
     * @params: N/A
     * @return: item data in QVariant
    */
    QVariant data() const;
private:
    // the ID of item that trigger event
    int id;
    // the data of item that trigger event
    QVariant eventData;
};

KAVI_NS_END

#endif // DIAGRAMEVENTS_H
