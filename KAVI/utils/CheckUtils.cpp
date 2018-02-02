#include "CheckUtils.h"
#include "XMLUtils.h"
#include "DataWidget.h"
#include "GraphClass.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"

CU_NS_BEGIN

using namespace XMLUtils;
using namespace KAVI;

QHash<int, QString> predicateArguments(const QDomElement &predicate)
{
    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QHash<int, QString> result;

    QDomElement connections = predicate.firstChildElement("connections");

    if (connections.isNull())
        return result;

    QDomElement diagram = predicate.parentNode().toElement();

    QDomElement actConn = connections.firstChildElement("starts");

    while( !actConn.isNull() )
    {
        int argNum = getIntAttribute(actConn, "argn");

        int edgeID = getIntValue(actConn);
        QDomElement edge = findSubelementAttr(diagram, "edge", "id", QString().setNum(edgeID));

        Q_ASSERT(!edge.isNull());

        int nodeID = associatedNodeID(edge, false);

        QDomElement node = findSubelementAttr(diagram, "node", "id", QString().setNum(nodeID));

        QString nodeLabel;

        if (node.firstChildElement("class").isNull())
            nodeLabel = subelementTagValue(node, "label");
        else
            nodeLabel = subelementTagValue(node, "class");

        result.insert(argNum, nodeLabel);

        actConn = actConn.nextSiblingElement("starts");
    }

    return result;
}

bool checkPredicateDefinition(const QDomElement &predicate, bool verbose)
{
    Q_ASSERT(!predicate.isNull());
    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QString predLabel = subelementTagValue(predicate, "label");

    Q_ASSERT(!predLabel.isEmpty());

    NodeStructure tempNode;
    tempNode.setData(nodeType, NST_PREDICATE);
    tempNode.setData(nodeLabel, predLabel);

    QDomElement predParent = predicate.parentNode().toElement();

    QList<int> duplicities = selectMatchingIDList(predParent, tempNode);

    if (verbose)
    {
        if ( duplicities.size() > 1 )
            qDebug() << predLabel << " (overloaded)";
        else
            qDebug() << predLabel;
    }

    bool equalArgCnt = true;
    int argCnt = -1;

    foreach(int id, duplicities)
    {
        QDomElement predNode = findSubelementAttr(predParent, "node", "id", QString().setNum(id));

        if ( verbose )
        {
            QHash<int, QString> arguments = predicateArguments(predNode);

            if (argCnt < 0)
                argCnt = arguments.count();

            if ( equalArgCnt )
                equalArgCnt = (argCnt == arguments.count());

            if (arguments.isEmpty())
                qDebug() << "No arguments defined";
            else
            {
                for (int i=1; i <= arguments.size(); ++i)
                    qDebug() << i << ":" << arguments.value(i);
            }
            qDebug() << "----------------";
        }
        else
        {
            if ( argCnt < 0 )
                argCnt = getArgCount(predNode);

            if ( equalArgCnt )
                equalArgCnt = (argCnt == getArgCount(predNode));
            else
                break;
        }
    }

    if ( !equalArgCnt )
    {
        if (verbose)
            qWarning() << "All versions of overloaded predicate should have equal number of arguments.";

        return false;
    }

    return true;
}

bool checkPredicateInstance(const QDomElement &predicate, const QDomElement &definitionRoot, bool verbose)
{
    Q_ASSERT(!predicate.isNull());
    Q_ASSERT(!definitionRoot.isNull());
    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QString predLabel = subelementTagValue(predicate, "label");

    QDomElement predDefinition = selectPredicateDefinition(definitionRoot, predicate);
    if ( predDefinition.isNull() )
    {
        if (verbose)
            qWarning() << "No suitable definition found for" << predLabel;
        return false;
    }

    int defArgCnt = getArgCount(predDefinition);
    int instArgCnt = getArgCount(predicate);

    if ( instArgCnt > defArgCnt )
    {
        if (verbose)
            qWarning() << predLabel << "has too many arguments";
        return false;
    }

    if ( instArgCnt < defArgCnt )
    {
        if (verbose)
            qWarning() << predLabel << "has some arguments missing.";
        return false;
    }

    QDomElement setDefinition = predicate.firstChildElement("set");
    QDomElement stateDefinition = predicate.firstChildElement("state");
    if ( setDefinition.isNull() && stateDefinition.isNull() )
    {
        /*
        if (verbose && stateDefinition.isNull())
            qWarning() << predLabel << "has no set defined.";
        */
        /* tohle se nemuze stat - stav je pouze v diagramech pro problem:
         * init/goal */
        /*if (verbose && setDefinition.isNull())
            qWarning() << predLabel << "has no state defined.";
         */
        if (verbose && stateDefinition.isNull())
            qWarning() << predLabel << "has no state defined.";
        if (verbose && setDefinition.isNull())
            qWarning() << predLabel << "has no set defined.";

        return false;
    }

    if (verbose)
        qDebug() << predLabel << "is OK.";

    return true;
}

bool checkClassInstance(const QDomElement &instance, const QDomElement &definitionRoot, bool verbose)
{
    Q_ASSERT(!instance.isNull());
    Q_ASSERT(!definitionRoot.isNull());
    Q_ASSERT(verifyNodeType(instance, NST_VARIABLE) || verifyNodeType(instance, NST_OBJECT));

    QString instClass = subelementTagValue(instance, "class");
    QString instLabel = subelementTagValue(instance, "label");

    NodeStructure tempNode;
    tempNode.setData(nodeLabel, instClass);

    int defID = getMatchingNodeID(definitionRoot, tempNode);

    if ( defID != INVALID_ID )
    {
        if (verbose)
            qDebug() << instLabel << "is OK";

        return true;
    }
    else
    {
        if (verbose)
            qWarning() << instLabel << "has invalid class (" << instClass << ")";
        return false;
    }
}

bool checkDefinition(const QDomElement &diagramRoot, bool verbose)
{
    Q_ASSERT(!diagramRoot.isNull());
    Q_ASSERT(diagramRoot.tagName() == "diagram");

    NodeStructure tempNode;
    tempNode.setData(nodeType, NST_CLASS);

    QList<QDomElement> classList = selectMatchingElementList(diagramRoot, tempNode);
    if (verbose)
    {
        qDebug() << "Defined classes:";
        foreach(QDomElement actClass, classList)
        {
            qDebug() << subelementTagValue(actClass, "label");
        }
    }

    if (verbose)
        qDebug() << "Defined predicates:";

    tempNode.setData(nodeType, NST_PREDICATE);

    QList<QDomElement> predicateList = selectMatchingElementList(diagramRoot, tempNode);

    // total unique predicate's count
    int totalPredCnt = predicateList.count();

    // valid unique predicate's count
    int predOK = 0;

    QSet<QString> processed;
    foreach(QDomElement actPred, predicateList)
    {
        QString actName = subelementTagValue(actPred, "label");

        if (!processed.contains(actName))
            processed.insert(actName);
        else
        {
            --totalPredCnt;
            continue;
        }

        if ( checkPredicateDefinition(actPred, verbose) )
            predOK++;
    }

    // invalid unique predicate's count
    int predKO = totalPredCnt - predOK;

    if(verbose)
    {
        qDebug() << "------------------------";
        qDebug() << "#classes:" << classList.count();

        if ( predKO > 0)
        {
            qDebug() << "#predicates:" << predOK << "+" << predKO << "(valid + invalid)";
            qWarning() << "There are invalid predicates in definition! - check failed";
        }
        else
            qDebug() << "#predicates:" << predOK;
    }

    if ( predKO > 0)
        return false;
    else
        return true;
}

bool checkDependentDiagram(const QDomElement &diagramRoot, const QDomElement &definitionRoot, char rectType, bool verbose)
{
    Q_ASSERT(!diagramRoot.isNull());
    Q_ASSERT(diagramRoot.tagName() == "diagram");
    Q_ASSERT(!definitionRoot.isNull());
    Q_ASSERT(definitionRoot.tagName() == "diagram");
    Q_ASSERT(rectType == NST_VARIABLE || rectType == NST_OBJECT);

    NodeStructure tempNode;
    tempNode.setData(nodeType, rectType);

    if (verbose)
    {
        switch(rectType)
        {
        case NST_VARIABLE:
            qDebug() << "Defined variables:";
            break;
        case NST_OBJECT:
            qDebug() << "Defined objects:";
            break;
        }
    }

    QList<QDomElement> rectList = selectMatchingElementList(diagramRoot, tempNode);

    int rectOK = 0;
    int rectKO = 0;

    foreach(QDomElement actRect, rectList)
    {
        if (checkClassInstance(actRect, definitionRoot, verbose))
            rectOK++;
        else
            rectKO++;
    }

    if (verbose)
        qDebug() << "Defined predicates:";

    tempNode.setData(nodeType, NST_PREDICATE);

    QList<QDomElement> predicateList = selectMatchingElementList(diagramRoot, tempNode);

    int predOK = 0;
    int predKO = 0;

    foreach(QDomElement actPred, predicateList)
    {
        if ( checkPredicateInstance(actPred,definitionRoot,verbose) )
            predOK++;
        else
            predKO++;
    }

    if (verbose)
    {
        qDebug() << "------------------";

        if ( rectKO > 0 )
        {
            if(rectType == NST_VARIABLE)
            {
                qDebug() << "#variables:" << rectOK << "+" << rectKO << "(valid + invalid)";
                qWarning() << "There are invalid variables (!)";
            }
            if(rectType == NST_OBJECT)
            {
                qDebug() << "#objects:" << rectOK << "+" << rectKO << "(valid + invalid)";
                qWarning() << "There are invalid objects (!)";
            }

        }
        else
        {
            if(rectType == NST_VARIABLE)
                qDebug() << "#variables:" << rectOK;
            if(rectType == NST_OBJECT)
                qDebug() << "#objects:" << rectOK;
        }

        if ( predKO > 0 )
        {
            qDebug() << "#predicates:" << predOK << "+" << predKO << "(valid + invalid)";
            qWarning() << "There are invalid predicates (!)";
        }
        else
        {
            qDebug() << "#predicates:" << predOK;
        }
    }

    if ( (predKO > 0) || (rectKO > 0) )
    {
        if (verbose)
            qWarning() << "Check failed.";
        return false;
    }
    else
        return true;
}

void brokenDiagrams(const QDomDocument &domain, const QDomElement &definitionRoot, const QString &groupTagName, const QString &itemTagName, QStringList &correct, QStringList &incorrect)
{
    Q_ASSERT(!domain.isNull());
    Q_ASSERT(!definitionRoot.isNull());

    QDomElement rootEl = domain.firstChildElement();
    Q_ASSERT(rootEl.tagName() == "domain");

    QDomElement groupEl = rootEl.firstChildElement(groupTagName);

    Q_ASSERT(!groupEl.isNull());


    correct.clear();
    incorrect.clear();

    char nodeType = NST_UNKNOWN;
    if ( itemTagName == "action" )
        nodeType = NST_VARIABLE;

    if ( itemTagName == "task" )
        nodeType = NST_OBJECT;

    if (nodeType == NST_UNKNOWN)
    {
        qWarning() << "$CheckFn::brokenDiagrams : wrong input";
        return;
    }

    QDomElement item = groupEl.firstChildElement(itemTagName);

    while(!item.isNull())
    {
        QString itemName = item.attribute("name");
        QDomElement itemDiagram = item.firstChildElement("diagram");

        if ( checkDependentDiagram(itemDiagram, definitionRoot, nodeType, false) )
            correct << itemName;
        else
            incorrect << itemName;

        item = item.nextSiblingElement(itemTagName);
    }
}

QDomElement selectPredicateDefinition(const QDomElement &definitionRoot, const QDomElement &predInstance)
{
    qDebug() << "$CheckFn::selectPredicateDefinition : selecting best matching definition";

    NodeStructure tempNode;
    tempNode.setData(nodeLabel, subelementTagValue(predInstance, "label"));
    tempNode.setData(nodeType, NST_PREDICATE);

    QList<QDomElement> definitions = selectMatchingElementList(definitionRoot, tempNode);

    if ( definitions.isEmpty() )
        return QDomElement();

    QHash<int, QString> instHash = predicateArguments(predInstance);

    // if predicate instance's arguments is empty, that all definitions can match, select the first one
    if (instHash.isEmpty())
    {
        return definitions.first();
    }

    GraphClass * inheritanceStructure = new GraphClass(definitionRoot);
    inheritanceStructure->init(NST_CLASS, DEP_INHERITANCE, IGRAPH_DIRECTED);

    QHashIterator<int, QString> iter(instHash);
    while( !definitions.isEmpty() )
    {
        QDomElement currentDef = definitions.takeFirst();

        QHash<int, QString> defHash = predicateArguments(currentDef);

        bool compatible = true;

        iter.toFront();

        while (iter.hasNext())
        {
            iter.next();

            NodeStructure tempNode;
            tempNode.setData(nodeLabel, iter.value());
            tempNode.setData(nodeType, NST_CLASS);

            int instID = getMatchingNodeID(definitionRoot, tempNode);

            QString defClass = defHash.value(iter.key());
            tempNode.setData(nodeLabel, defClass);

            int defID = getMatchingNodeID(definitionRoot, tempNode);

            if (instID == defID)
                continue;

            QSet<int> descSet = inheritanceStructure->getDescendants(defID);

            if ( !descSet.contains(instID) )
            {
                compatible = false;
                break;
            }
        }

        if (!compatible)
            continue;

        delete inheritanceStructure;

        return currentDef;
    }

    delete inheritanceStructure;

    return QDomElement();
}

int getArgNumber(const QDomElement &predicate, int edgeID)
{
    Q_ASSERT(!predicate.isNull());
    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QDomElement connections = predicate.firstChildElement("connections");

    Q_ASSERT(!connections.isNull());

    QDomElement wantedConn = findSubelementVal(connections, "starts", QString().setNum(edgeID));

    Q_ASSERT(!wantedConn.isNull());

    return getIntAttribute(wantedConn, "argn");
}

int getArgCount(const QDomElement &predicate)
{
    Q_ASSERT(!predicate.isNull());
    Q_ASSERT(verifyNodeType(predicate, NST_PREDICATE));

    QDomElement connections = predicate.firstChildElement("connections");

    if (connections.isNull())
        return 0;

    return connections.elementsByTagName("starts").count();
}

QStringList classFamily(const QString &className, const GraphClass *structure, const DataWidget *definition)
{
    NodeStructure tempNode;
    tempNode.setData(nodeLabel, className);
    tempNode.setData(nodeType, NST_CLASS);

    int classID = definition->getMatchingNodeID(tempNode);

    return structure->pathToRoot(classID);
}

bool nodeMatches(const QDomElement &probedNode, const KAVI::NodeStructure &patternNode)
{
    Q_ASSERT(!probedNode.isNull());

    QVariant testVal;

    testVal = patternNode.readData(nodeLabel);
    if ( !testVal.isNull() )
    {
        QDomElement labelEl = probedNode.firstChildElement("label");
        if ( getStrValue(labelEl) != testVal.toString() )
            return false;
    }

    //char patternNodeType = NST_UNKNOWN;
    char probedNodeType = NST_UNKNOWN;

    testVal = patternNode.readData(nodeType);
    if ( !testVal.isNull() )
    {
        QDomElement typeEl = probedNode.firstChildElement("type");
        if ( (probedNodeType = detNodeType(typeEl)) != testVal.toChar().toLatin1() )
            return false;
    }

    testVal = patternNode.readData(nodeState);
    if ( (probedNodeType == NST_PREDICATE) && !testVal.isNull() )
    {
        QDomElement stateEl = probedNode.firstChildElement("state");
        if ( getStrValue(stateEl) != testVal.toString() )
            return false;
    }

    testVal = patternNode.readData(nodeClass);
    if ( (probedNodeType != NST_PREDICATE) && !testVal.isNull() )
    {
        QDomElement classEl = probedNode.firstChildElement("class");
        if ( getStrValue(classEl) != testVal.toString() )
            return false;
    }

    return true;
}

int getMatchingNodeID(const QDomElement &root, const KAVI::NodeStructure &nodeTemplate)
{
    QList<int> candidates = selectMatchingIDList(root, nodeTemplate);

    if ( candidates.isEmpty() )
    {
        qWarning() << "$CheckFn::getMatchingNodeID : specified node not found.";
        return INVALID_ID;
    }

    if ( candidates.size() > 1 )
        qWarning() << "$CheckFn::getMatchingNodeID : specified node is not unique";

    return candidates.first();
}

QList<int> selectMatchingIDList(const QDomElement &root, const KAVI::NodeStructure &nodeTemplate)
{
    Q_ASSERT(!root.isNull());

    QList<int> result;

    QDomElement actNode = root.firstChildElement("node");

    while( !actNode.isNull() )
    {
        if ( nodeMatches(actNode, nodeTemplate) )
            result.append(getIntAttribute(actNode, "id"));

        actNode = actNode.nextSiblingElement("node");
    }

    return result;
}

QList<QDomElement> selectMatchingElementList(const QDomElement &root, const KAVI::NodeStructure &nodeTemplate)
{
    Q_ASSERT(!root.isNull());

    QList<QDomElement> result;

    QDomElement actNode = root.firstChildElement("node");

    while( !actNode.isNull() )
    {
        if ( nodeMatches(actNode, nodeTemplate) )
            result.append(actNode);

        actNode = actNode.nextSiblingElement("node");
    }

    return result;
}



CU_NS_END
