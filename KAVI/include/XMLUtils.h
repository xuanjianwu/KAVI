/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : define XML utilities
*/

#ifndef XMLUTILS_H
#define XMLUTILS_H

#include "KAVIBase.h"

XMLU_NS_BEGIN

    // the connected nodes' ID <startNode's ID, endNode's ID> of edge
    typedef QPair<int, int> EdgeDefinition;

    /*
    * get the attribute's content with int datatype
    * @params:
    *       elem     - the specified element to get attribute's content
    *       attrName - the expected attribute's name
    * @return: attribute's content
    */
    int	getIntAttribute(const QDomElement& elem, QString attrName);

    /*
    * set the attribute's content with int datatype
    * @params:
    *       elem       - the specified element to set attribute's content
    *       attrName   - the expected attribute's name
    *       newContent - the new attribute content
    * @return: void
    */
    void setIntAttribute(QDomElement elem, const QString& attrName, int newContent);

    /*
    * get the element's content with int datatype
    * @params:
    *       elem - the specified element
    * @return: content value
    */
    int getIntValue(const QDomElement& elem);

    /*
    * set the element's content with int datatype
    * @params:
    *       elem  - the specified element
    *       value - the new content value
    * @return: void
    */
    void setIntValue(QDomElement& elem, int value);

    /*
    * get the element's content with float datatype
    * @params:
    *       elem - the specified element
    * @return: content value
    */
    float getFloatValue(const QDomElement& elem);

    /*
    * get the element's content with QString datatype
    * @params:
    *       elem - the specified element
    * @return: content value
    */
    QString	getStrValue(const QDomElement& elem);

    /*
    * set the element's content with QString datatype
    * @params:
    *       elem  - the specified element
    *       value - the new content value
    * @return: void
    */
    void setStrValue(QDomElement& elem, const QString value);

    /*
    * get the element's position
    * @params:
    *       elem - the specified element
    * @return: position's point
    */
    QPointF	getPosValue(const QDomElement& elem);

    /*
    * determine the type of node
    * @params:
    *       type - the specified node element
    * @return: node's type
    */
    char detNodeType(const QDomElement& type);

    /*
    * determine the purpose of edge
    * @params:
    *       purpose - the specified edge element
    * @return: edge's purpose
    */
    char detEdgePurpose(const QDomElement& purpose);

    /*
    * verify the edge purpose
    * @params:
    *       elem      - the specified edge element to verify
    *       purpose   - the expected edge's purpose to verify
    * @return: pass verify or not
    */
    bool verifyEdgePurpose(const QDomElement& elem, char purpose);

    /*
    * verify the node type
    * @params:
    *       elem      - the specified node element to verify
    *       type      - the expected node's type to verify
    * @return: pass verify or not
    */
    bool verifyNodeType(const QDomElement& elem, char type);

    /*
    * get the list of nodes' label with the specified node type
    * @params:
    *       searchRoot - the root element to search specified nodes' label
    *       wantedType - the expected node's type to search
    * @return: nodes' label
    */
    QStringList getNodeList(QDomElement searchRoot, char wantedType);

    /*
    * get the subelement with specified tagName and specified subelement attribute
    * @params:
    *       parent    - the parent element to search
    *       tagName   - the expected subelement's tag name
    *       attrName  - the specified attribute's name to search
    *       attrValue - the specified attribuet's value to search
    * @return: the expected subelement
    */
    QDomElement findSubelementAttr(const QDomElement& parent, const QString& tagName,
                                   const QString& attrName = QString(), const QString& attrValue = QString() );

    /*
    * get the subelement with specified tagName and specified subelement content
    * @params:
    *       parent    - the parent element to search
    *       tagName   - the expected subelement's tag name
    *       textVal   - the specified subelement content in text
    * @return: the expected subelement
    */
    QDomElement findSubelementVal(const QDomElement& parent, const QString& tagName, const QString& textVal);

    /*
    * add subelement with specified tagName and specified subelement attribute
    * @params:
    *       parent    - the parent element to add subelement
    *       tagName   - the tag name of the expected new subelement
    *       attrName  - the attribute name of the expected new subelement
    *       attrValue - the attribute value of the expected new subelement
    * @return: the reference to the new added subelement
    */
    QDomElement addSubelement(QDomElement parent, const QString& tagName,
                              const QString& attrName = QString(), const QString& attrValue = QString() );

    /*
    * get the value of subelement with tag named tagName
    * @params:
    *       parent   - the parent element to search
    *       tagName  - the expected subelement's tag name
    * @return: subelement's value
    */
    QString subelementTagValue(QDomElement parent, QString tagName);

    /*
    * select edges from parent element according to the edge mask
    * @params:
    *       parent   - the parent element to search
    *       edgeMask - the select mask
    * @return: list of <start node's ID, end node's ID>
    */
    QList<EdgeDefinition> selectEdges(QDomElement parent, char edgeMask);

    /*
    * select nodes from parent element according to the node mask
    * @params:
    *       parent   - the parent element to search
    *       nodeMask - the select mask
    * @return: list of node's ID
    */
    QList<int> selectNodeIDList(QDomElement parent, char nodeMask);

    /*
    * select nodes' label from parent element according to the node mask
    * @params:
    *       parent   - the parent element to search
    *       nodeMask - the select mask
    * @return: list of node's label
    */
    QSet<QString> selectNodeLabelSet(QDomElement parent, char nodeMask);

    /*
    * get the connected edges' ID from node element
    * @params:
    *       node   - the specified node to search
    * @return: list of edge's ID
    */
    QList<int> connectedEdges(const QDomElement& node);

    /*
    * get the defined arguments of predicate node
    * @params:
    *       predicate - the specified predicate node to search
    * @return: list of <argument's number, argument's associated edge ID>
    */
    QMap<int, int> definedArguments(const QDomElement& predicate);

    /*
    * get the content with specified tagName of the child element(including in the idList) of searchRoot
    * @params:
    *       idList     - the specified node's id list
    *       searchRoot - the root element to search
    *       tagName    - the specified tag name
    * @return: contents with the order according to the ID in idList
    */
    QStringList	correspondingNodeContent(QList<int>& idList, QDomElement searchRoot, QString tagName);

    /*
    * get the associated node's ID of edge
    * @params:
    *       edge  - the specified edge to search
    *       start - edge point's orientation
    * @return: associated node's ID
    */
    int associatedNodeID(const QDomElement& edge, bool start);

XMLU_NS_END

#endif // XMLUTILS_H
