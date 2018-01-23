/*
 * @author: liYc
 * @date  : 2018/1/23
 * @brief : the basic XML check for KAVI
*/

#ifndef CHECKUTILS_H
#define CHECKUTILS_H

#include "KAVIBase.h"

class DataWidget;
class GraphClass;

struct NodeStructure;

CU_NS_BEGIN
   /*
    * get the arguments from predicate
    * @params:
    *       predicate - target predicate
    * @return: <argID, argName>
    */
    QHash<int, QString> predicateArguments(const QDomElement& predicate);

   /*
    *
    * @params:
    *       predicate - target predicate
    *       verbose   - show verbose message or not
    * @return: bool
    */
    bool checkPredicateDefinition(const QDomElement& predicate, bool verbose);

    /*
    *
    * @params:
    *       predicate      - target predicate
    *       definitionRoot - target definition root
    *       verbose        - show verbose message or not
    * @return: bool
    */
    bool checkPredicateInstance(const QDomElement& predicate, const QDomElement& definitionRoot, bool verbose);

    /*
    *
    * @params:
    *       instance       - target class
    *       definitionRoot - target definition root
    *       verbos         - show verbose message or not
    * @return: bool
    */
    bool checkClassInstance(const QDomElement& instance, const QDomElement& definitionRoot, bool verbose);

    /*
    *
    * @params:
    *       diagramRoot - definition diagram
    *       verbose     - show verbose message or not
    * @return: bool
    */
    bool checkDefinition(const QDomElement& diagramRoot, bool verbose);


    bool checkDependentDiagram(const QDomElement& diagramRoot, const QDomElement& definitionRoot, char rectType, bool verbose);

    void brokenDiagrams(const QDomDocument& domain, const QDomElement& definitionRoot, const QString& groupTagName,
                        const QString& itemTagName, QStringList& correct, QStringList& incorrect);

    QDomElement selectPredicateDefinition(const QDomElement& definitionRoot, const QDomElement& predInstance);
    int getArgNumber(const QDomElement& predicate, int edgeID);
    int getArgCount(const QDomElement& predicate);
    QStringList classFamily(const QString& className, const GraphClass* structure, const DataWidget* definition);

    bool nodeMatches(const QDomElement& probedNode, const NodeStructure& patternNode);

    int getMatchingNodeID(const QDomElement& root, const NodeStructure& nodeTemplate);
    QList<int> selectMatchingIDList(const QDomElement& root, const NodeStructure& nodeTemplate);
    QList<QDomElement> selectMatchingElementList(const QDomElement& root, const NodeStructure& nodeTemplate);

CU_NS_END

#endif // CHECKUTILS_H
