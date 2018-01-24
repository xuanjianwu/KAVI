/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : define XML utilities
*/

#ifndef XMLUTILS_H
#define XMLUTILS_H

#include "KAVIBase.h"

XMLU_NS_BEGIN

    typedef QPair<int, int> EdgeDefinition;

    int	getIntAttribute(const QDomElement& elem, QString attrName);
    void setIntAttribute(QDomElement elem, const QString& attrName, int newContent);
    int getIntValue(const QDomElement& elem);
    void setIntValue(QDomElement& elem, int value);
    float getFloatValue(const QDomElement& elem);
    QString	getStrValue(const QDomElement& elem);
    void setStrValue(QDomElement& elem, const QString value);
    QPointF	getPosValue(const QDomElement& elem);

    char detNodeType(const QDomElement& type);

    char detEdgePurpose(const QDomElement& purpose);

    bool verifyEdgePurpose(const QDomElement& elem, char purpose);
    bool verifyNodeType(const QDomElement& elem, char type);
    QStringList getNodeList(QDomElement searchRoot, char wantedType);

    QDomElement findSubelementAttr(const QDomElement& parent, const QString& tagName,
                                   const QString& attrName = QString(), const QString& attrValue = QString() );
    QDomElement findSubelementVal(const QDomElement& parent, const QString& tagName, const QString& textVal);

    QDomElement addSubelement(QDomElement parent, const QString& tagName,
                              const QString& attrName = QString(), const QString& attrValue = QString() );

    QString subelementTagValue(QDomElement parent, QString tagName);
    QList<EdgeDefinition> selectEdges(QDomElement parent, char edgeMask);
    QList<int> selectNodeIDList(QDomElement parent, char nodeMask);
    QSet<QString> selectNodeLabelSet(QDomElement parent, char nodeMask);
    QList<int> connectedEdges(const QDomElement& node);
    QMap<int,int> definedArguments(const QDomElement& predicate);
    QStringList	correspondingNodeContent(QList<int>& idList, QDomElement searchRoot, QString tagName);
    int associatedNodeID(const QDomElement& edge, bool start);

XMLU_NS_END

#endif // XMLUTILS_H
