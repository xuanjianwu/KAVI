/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : This class manages data manipulation and serves as Model in
 *          MVC. It manipulates content of associated QDomDocument instance.
 *          That is definitionDocument, actionDocument or taskDocument
 *          - depending on the context
*/

#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "KAVIBase.h"
#include "XMLUtils.h"



class DiagramWidget;
class TreeModel;

struct NodeStructure;
struct EdgeStructure;

using namespace XMLUtils;

// <connected edge's ID, connected edgepoint's orientation>
typedef QPair<int, bool> DataEdgepoint;

class DataWidget {
public:
    /*
    * construct function
    * @params:
    *       diagram - the diagram scene(View) associated with DataWidget(Model)
    * @return: N/A
    */
    DataWidget(DiagramWidget* diagram);

    /*
    * get the root element with tag<diagram> of XMLdata
    * @params: N/A
    * @return: QDomElement
    */
    QDomElement diagramRootElement() const;

    /*
    * load XML data, reset diagram scene and add nodes, edges to diagram scene
    * @params:
    *       diagramDocument - expected XML document
    * @return: N/A
    */
    void loadData(QDomDocument diagramDocument);

    /*
    * get the associated edgepoints of node in format
    * <connected edge's ID, connected edgepoint's orientation>
    * @params:
    *       nodeID - expected node's ID
    * @return: <connected edge's ID, connected edgepoint's orientation>
    */
    QList<DataEdgepoint> associatedEdgepoints(int nodeID) const;

    /*
    * get the associated edges' ID of node
    * @params:
    *       nodeID - expected node's ID
    * @return: edges'ID in list
    */
    QList<int> associatedEdges(int nodeID) const;

    /*
    * get the associated nodes' ID of edge
    * @params:
    *       edgeID - expected edge's ID
    * @return: <start node's ID, end node's ID>
    */
    QPair<int, int> associatedNodes(int edgeID) const;

    /*
    * get the node'data from expectd tag<tagName>
    * @params:
    *       id      - node's ID
    *       tagName - specified tag in node
    * @return: tag's data
    */
    QString getNodeData(int id, QString tagName) const;

    QPointF getNodePos(int id) const;

    /*
    * get the edge description in EdgeStructure
    * @params:
    *       id - edge's ID
    * @return: EdgeStructure
    */
    const EdgeStructure getEdgeDescription(int id) const;

    /*
    * get the specified Node
    * @params:
    *       id - node's ID
    * @return: node's element
    */
    QDomElement findNode(int id) const;

    /*
    * get the specified Edge
    * @params:
    *       id - edge's ID
    * @return: edge's element
    */
    QDomElement findEdge(int id) const;

    /*
    * get the purpose of edge
    * @params:
    *       edgeID - edge's ID
    * @return: purpose in char
    */
    char readEdgePurpose(int edgeID);

    /*
    * get the type of node
    * @params:
    *       nodeID - node's ID
    * @return: type in char
    */
    char readNodeType(int nodeID);

    /*
    * get the set of valid predicates' name, valid means that
    * all predicates with same name have same number of arg
    * @params: N/A
    * @return: set of valid predicates' name
    */
    QSet<QString> getValidPredicateSet() const;

    /*
    * get the list of labels for all specified type's node
    * @params:
    *       type - specified type
    * @return: list of label
    */
    QStringList getNodeLabelList(char type) const;

    /*
    * get the set of labels for all specified type's node
    * @params:
    *       type - specified type
    * @return: set of label
    */
    QSet<QString> getNodeLabelSet(char type) const;

    /*
    * get the matching node's ID, if such ID is not unique, return the first one
    * @params:
    *       nodeTemplate - the template used to match node
    * @return: matching node's ID
    */
    int getMatchingNodeID(const NodeStructure& nodeTemplate) const;

    /*
    * get the list of matching nodes' ID
    * @params:
    *       nodeTemplate - the template used to match node
    * @return: list of matching nodes's ID
    */
    QList<int> selectMatchingIDList(const NodeStructure& nodeTemplate) const;

    /*
    * get the list of matching nodes' element
    * @params:
    *       nodeTemplate - the template used to match node
    * @return: list of matching nodes's element
    */
    QList<QDomElement> selectMatchingElementList(const NodeStructure& nodeTemplate) const;

    /*
    * refresh the node in the scene
    * @params:
    *       nodeID - specified node's ID
    * @return: void
    */
    void refreshNode(int nodeID);

    /*
    * add node's data to DataWidget and add node to the diagram scene
    * @params:
    *       node - the new added node
    * @return: void
    */
    void addDataNode(const NodeStructure& node);

    /*
    * add edge's data to DataWidget and add edge to the diagram scene
    * @params:
    *       edge - the new added edge
    * @return: void
    */
    void addDataEdge(const EdgeStructure& edge);

    /*
    * delete node's data from DataWidget and remove node from diagram scene
    * @params:
    *       id - deleted node's ID
    * @return: void
    */
    void delDataNode(int id);

    /*
    * delete edge's data from DataWidget and remove edge from diagram scene
    * and return the connected nodes's ID in pair<startNode's ID, endNode's ID>
    * @params:
    *       id - deleted node's ID
    * @return: void
    */
    EdgeDefinition delDataEdge(int id);

    /*
    * change the node's data pos in DataWidget
    * @params:
    *       id     - specified node's ID
    *       vector - the added vector for changed node's pos
    * @return: void
    */
    void changeNodePos(int id, const QPointF& vector);

    /*
    * change the edge's data pos in DataWidget
    * @params:
    *       id      - specified edge's ID
    *       isStart - edge's orientation
    *       vector  - the added vector for changed edgepoint's pos
    * @return: void
    */
    void changeEdgePos(int id, bool isStart, const QPointF& vector);

    /*
    * change the edge association's node in DataWidget
    * @params:
    *       id      - specified edge's ID
    *       isStart - edge's orientation
    *       toNode  - the new association's nodeID
    * @return:
    */
    void changeEdgeAssociation(int id, bool isStart, int toNode);

    /*
    * remove edge form node in DataWidget, and return the next <starts> element
    * if the removed edge is <starts>, otherwise return null element
    * @params:
    *       edgeID - removed edge's ID
    *       start  - edge's orientation
    *       nodeID - specified node's ID
    * @return: the next <starts> element or null element
    */
    QDomElement disconnectEdgeFromNode(int edgeID, bool start, int nodeID);

    /*
    * add edge to node in DataWidget, and return the added element
    * @params:
    *       edgeID - added edge's ID
    *       start  - edge's orientation
    *       nodeID - specified node's ID
    * @return: the added element
    */
    QDomElement connectEdgeToNode(int edgeID, bool start, int nodeID);

private:
    // the XML data for the DataWidget
    QDomDocument XMLdata;
    // the root element with tag<diagram> of XMLdata
    QDomElement diagramData;
    // the diagram scene(View) associated with DataWidget(Model)
    DiagramWidget* diagramScene;

    /*
    * add the vector to position element in DataWidget
    * @params:
    *       pos    - specified position element
    *       vector - added vector
    * @return: void
    */
    void addVector(QDomElement& pos, QPointF vector);

    /*
    * set the new position for pos element in DataWidget
    * @params:
    *       pos    - specified position element
    *       newPos - the new position
    * @return: void
    */
    void setPosition(QDomElement& pos, QPointF newPos);

    /*
    * add a new child element with content in DataWidget
    * @params:
    *       parent    - parent element to add child
    *       childName - the added element's tag name
    *       content   - the added element's content
    * @return: the new added element
    */
    QDomElement	addChildElement(QDomElement& parent, QString childName, QString content);

    /*
    * remove all the <sets> element from elem in DataWidget
    * @params:
    *       elem  - the target element to remove <sets>
    * @return: void
    */
    void resetPredicate(QDomElement& elem);

    /*
    * load node structure from node element in DataWidget
    * @params:
    *       elem  - the specified node element to loaded
    * @return: node structure
    */
    const NodeStructure loadNode(const QDomElement& elem) const;

    /*
    * load edge structure from edge element in DataWidget
    * @params:
    *       elem  - the specified edge element to loaded
    * @return: edge structure
    */
    const EdgeStructure loadEdge(const QDomElement& elem) const;
};



#endif // DATAWIDGET_H
