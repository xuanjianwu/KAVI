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
    * get the arguments of predicate
    * @params:
    *       predicate - the specified predicate
    * @return: hash table of <argument number, argument's class>
    */
    QHash<int, QString> predicateArguments(const QDomElement& predicate);

   /*
    * check that all versions of overloaded predicates in current document should have equal number of arguments
    * @params:
    *       predicate - the specified predicate
    *       verbose   - show more message or not
    * @return: if all versions of overloaded predicates have equal number of arguments, return true;
    *          otherwise false.
    */
    bool checkPredicateDefinition(const QDomElement& predicate, bool verbose);

    /*
    * check that the predicate instance can well match with the definition, including argument's class and order and count
    * @params:
    *       predicate      - the specified predicate
    *       definitionRoot - the definition root element
    *       verbose        - show more message or not
    * @return: predicate instance is matched with definition or not
    */
    bool checkPredicateInstance(const QDomElement& predicate, const QDomElement& definitionRoot, bool verbose);

    /*
    * check that the class instance(variable or object) can well match with the definition
    * @params:
    *       instance       - class instance
    *       definitionRoot - the definition root element
    *       verbos         - show more message or not
    * @return: class instance is matched with definition or not
    */
    bool checkClassInstance(const QDomElement& instance, const QDomElement& definitionRoot, bool verbose);

    /*
    * check the definition's classes and predicates, for the predicates, check the overloaded
    * predicates' argument count
    * @params:
    *       diagramRoot - definition diagram
    *       verbose     - show more message or not
    * @return: definition is valid or not
    */
    bool checkDefinition(const QDomElement& diagramRoot, bool verbose);

    /*
    * check the diagram depending on definition, including all class instances and predicate instances
    * @params:
    *       diagramRoot    - the specified diagram root element
    *       definitionRoot - the definition diagram root element
    *       rectType       - the class instance's type filter
    *       verbose        - show more message or not
    * @return: current diagram is valid or not
    */
    bool checkDependentDiagram(const QDomElement& diagramRoot, const QDomElement& definitionRoot, char rectType, bool verbose);

    /*
    * broken all diagrams in operators and problems to single action diagram element or task diagram
    * element, then check the single diagram with definition by checkDependentDiagram. according to
    * the check result, put the action's name or task's name to correct or incorrect
    * @params:
    *       domain          - the domain document
    *       definitionRoot  - the definition root element
    *       groupTagName    - the group element tag's name: operators or problems
    *       itemTagName     - the item element tag's name: action or task
    *       correct         - get the checked correct action's or task's name
    *       incorrect       - get the checked incorrect action's or task's name
    * @return: void
    */
    void brokenDiagrams(const QDomDocument& domain, const QDomElement& definitionRoot, const QString& groupTagName,
                        const QString& itemTagName, QStringList& correct, QStringList& incorrect);

    /*
    * select the best matching definition's predicate for the predicate instance, including argument's class and order
    * @params:
    *       definitionRoot - the definition root element
    *       predInstance   - the predicate instance to match
    * @return: the matched definition element or not
    */
    QDomElement selectPredicateDefinition(const QDomElement& definitionRoot, const QDomElement& predInstance);

    /*
    * get the argument number of the specified edge's ID
    * @params:
    *       predicate - the predicate to search
    *       edgeID    - the argument's associated edge's ID
    * @return: the expected arguments's number
    */
    int getArgNumber(const QDomElement& predicate, int edgeID);

    /*
    * get the arguments' count of the specified predicate
    * @params:
    *       predicate - the predicate to search
    * @return: the count of arguments
    */
    int getArgCount(const QDomElement& predicate);

    /*
    * get the class family from current class to root, all the basic classes
    * @params:
    *       className  - the expected class's name
    *       structure  - the associated graph
    *       definition - the definition DataWidget
    * @return: list of class families's name
    */
    QStringList classFamily(const QString& className, const GraphClass* structure, const DataWidget* definition);

    /*
    * match the node with the specified pattern node structure template including nodeLabel,
    * nodeType, nodeState, nodeClass
    * @params:
    *       probedNode  - the expected node to match
    *       patternNode - the specified node structure template
    * @return: match succeed or not
    */
    bool nodeMatches(const QDomElement& probedNode, const KAVI::NodeStructure& patternNode);

    /*
    * get the first of the matching nodes' ID
    * @params:
    *       root         - the root element to search
    *       nodeTemplate - the matching template
    * @return: the matching node's ID
    */
    int getMatchingNodeID(const QDomElement& root, const KAVI::NodeStructure& nodeTemplate);

    /*
    * get the set of the matching nodes' ID
    * @params:
    *       root         - the root element to search
    *       nodeTemplate - the matching template
    * @return: the set of matching nodes' ID
    */
    QList<int> selectMatchingIDList(const QDomElement& root, const KAVI::NodeStructure& nodeTemplate);

    /*
    * get the set of the matching nodes' element
    * @params:
    *       root         - the root element to search
    *       nodeTemplate - the matching template
    * @return: the set of matching nodes' element
    */
    QList<QDomElement> selectMatchingElementList(const QDomElement& root, const KAVI::NodeStructure& nodeTemplate);

CU_NS_END

#endif // CHECKUTILS_H
