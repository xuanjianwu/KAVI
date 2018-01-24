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

KAVI_NS_BEGIN

class DiagramWidget;
class TreeModel;

struct NodeStructure;
struct EdgeStructure;

using namespace XMLUtils;

class DataWidget {
public:
    DataWidget(DiagramWidget* diagram);

    QDomElement diagramRootElement() const;

    void loadData(QDomDocument diagramDocument);

    QList<DataEdgepoint> associatedEdgepoints(int nodeID) const;
    QList<int> associatedEdges(int nodeID) const;
    QPair<int, int> associatedNodes(int edgeID) const;

    QString getNodeData(int id, QString tagName) const;
    const EdgeStructure getEdgeDescription(int id) const;
    QDomElement findNode(int id) const;
    QDomElement findEdge(int id) const;

    char readEdgePurpose(int edgeID);
    char nodeType(int nodeID);

    QSet<QString> getValidPredicateSet() const;

    QStringList getNodeLabelList(char type) const;
    QSet<QString> getNodeLabelSet(char type) const;

    int getMatchingNodeID(const NodeStructure& nodeTemplate) const;
    QList<int> selectMatchingIDList(const NodeStructure& nodeTemplate) const;
    QList<QDomElement> selectMatchingElementList(const NodeStructure& nodeTemplate) const;

    void refreshNode(int nodeID);

    void addDataNode(const NodeStructure& node);
    void addDataEdge(const EdgeStructure& edge);

    void delDataNode(int id);
    EdgeDefinition delDataEdge(int id);

    void changeNodePos(int id, const QPointF& vector);
    void changeEdgePos(int id, bool isStart, const QPointF& vector);

    void changeEdgeAssociation(int id, bool isStart, int toNode);
    QDomElement disconnectEdgeFromNode(int edgeID, bool start, int nodeID);
    QDomElement connectEdgeToNode(int edgeID, bool start, int nodeID);

private:
    QDomDocument XMLdata;
    QDomElement diagramData;
    DiagramWidget* diagramScene;

    void addVector(QDomElement& pos, QPointF vector);
    void setPosition(QDomElement& pos, QPointF newPos);
    QDomElement	addChildElement(QDomElement& parent, QString childName, QString content);

    void resetPredicate(QDomElement& elem);

    const NodeStructure loadNode(const QDomElement& elem) const;
    const EdgeStructure loadEdge(const QDomElement& elem) const;
}

KAVI_NS_END

#endif // DATAWIDGET_H
