#include "Convertor.h"
#include "GraphClass.h"
#include "EdgeStructure.h"
#include "NodeStructure.h"
#include "XMLUtils.h"
#include "CheckUtils.h"
#include "igraph.h"




using namespace XMLUtils;
using namespace CheckUtils;

Convertor::Convertor(QWidget *owner)
{
    ownerWidget = owner;
    currentDoc = QDomDocument();
}

QDomDocument Convertor::readFromXML(QFile &input)
{
    QDomDocument domainData;

    QString errorMsg;
    int errorLine;
    int errorColumn;

    if ( !domainData.setContent(&input, &errorMsg, &errorLine, &errorColumn) )
    {
        QString errMessage = QObject::tr("Error in XML syntax on line: %1 column: %2").arg(errorLine).arg(errorColumn);
        //QMessageBox::critical (ownerWidget, "KAVI", errMessage);
        qCritical() << "@Error in XML syntax on line:" << errorLine << "column:" << errorColumn;
        return QDomDocument();
    }

    if ( !basicStructureCheck(domainData) )
    {
        qCritical() << "@Basic Structure check failed";
        return QDomDocument();
    }

    return domainData;
}

void Convertor::writeToXML(const QDomDocument &doc, QFile &output)
{
    if (!basicStructureCheck(doc))
    {
        qCritical() << "@Error while saving document.";
        return;
    }

    QTextStream saveStream(&output);
    currentDoc.save(saveStream, OUTPUT_INDENT);
    qDebug() << "@Domain succesfully saved.";
}

void Convertor::writeDomainToPDDL(const QDomDocument &doc, QFile &output)
{
    if (!basicStructureCheck(doc))
    {
        qCritical() << "@Error while exporting document.";
        return;
    }

    outStream.setDevice(&output);

    QDomElement description = rootElement.firstChildElement("description");

    if (!description.isNull())
    {
        QStringList lines = getStrValue(description).split("\n");
        foreach(QString line, lines)
        {
            outStream << "; " << line << endl;
        }
        outStream << endl;
    }

    outStream << "(define (domain " << domainName << " )"<< endl;

    writeLine(1, "(:requirements :strips :typing :negative-preconditions)");

    writeItem(1, getDefinedTypes(), "(:types", ")");
    writeItem(1, getDefinedPredicates(), "(:predicates", ")");

    QDomElement action = operatorsElement.firstChildElement("action");
    while (!action.isNull())
    {
        QString actionName = action.attribute("name");
        if (actionName.isEmpty())
            qWarning() << "$Convertor::writeDomainToPDDL : missing action name";

        QDomElement actionDiagram = action.firstChildElement("diagram");

        writeAction(1, actionDiagram, actionName);

        action = action.nextSiblingElement("action");
    }

    outStream << ")\n";
}

void Convertor::writeProblemsToPDDL(const QDomDocument &doc, QString &targetDir)
{
    if (!basicStructureCheck(doc))
    {
        qCritical() << "@Error while exporting document.";
        return;
    }

    QDomElement task = problemsElement.firstChildElement("task");

    while (!task.isNull())
    {
        QString taskName = task.attribute("name");

        if (taskName.isEmpty())
            qWarning() << "$Convertor::writeDomainToPDDL : missing task name";

        QDomElement taskDiagram = task.firstChildElement("diagram");
        writeTask(taskDiagram, taskName, targetDir);

        task = task.nextSiblingElement("task");
    }
}

void Convertor::writeSingleProblemToPDDL(const QDomDocument &doc, QString problemName, QFile &output)
{
    if (!basicStructureCheck(doc))
    {
        qCritical() << "@Error while exporting document.";
        return;
    }

    QDomElement task = problemsElement.firstChildElement("task");

    while (!task.isNull())
    {
        QString taskName = task.attribute("name");

        if (taskName.isEmpty())
            qWarning() << "$Convertor::writeDomainToPDDL : missing task name";

        if (taskName.trimmed().contains(problemName.trimmed()))
        {
            QDomElement taskDiagram = task.firstChildElement("diagram");
            writeSingleTask(taskDiagram, taskName, output);
            break;
        }
        task = task.nextSiblingElement("task");
    }
}

bool Convertor::basicStructureCheck(const QDomDocument &domainDocument)
{
    Q_ASSERT(!domainDocument.isNull());
    currentDoc = domainDocument;

    rootElement = currentDoc.firstChildElement();
    Q_ASSERT(!rootElement.isNull());

    domainName = rootElement.attribute("name");

    if (domainName.isNull())
        domainName = "UNNAMED";

    definitionElement = rootElement.firstChildElement("definition");
    operatorsElement = rootElement.firstChildElement("operators");
    problemsElement = rootElement.firstChildElement("problems");

    if ( definitionElement.isNull() || operatorsElement.isNull() || problemsElement.isNull() )
    {
        qCritical() << "@Document syntax error";
        return false;
    }
    else
        return true;
}

void Convertor::writeLine(int indent, QString text)
{
    Q_ASSERT(outStream.device());
    Q_ASSERT(!currentDoc.isNull());

    outStream << QString().fill(' ', OUTPUT_INDENT*indent);
    outStream << text << endl;
}

void Convertor::writeItem(int indent, QString text, QString header, QString footer)
{
    writeLine(indent, header);
    QStringList lines = text.split('\n');

    foreach (QString singleLine, lines)
        writeLine(indent + 1, singleLine);

    writeLine(indent, footer);
}

void Convertor::writeAction(int indent, QDomElement diagram, QString name)
{
    qDebug() << "$Convertor::writeAction : current action =" << name;
    writeLine(indent, name.prepend("(:action "));

    if (diagram.isNull())
    {
        qWarning() << "@Action" << name << "is empty.";
        writeLine(indent, ")");
        return;
    }

    writeLine(indent + 1, getActionParameters(diagram));

    QSet<int> actPrecondPos;
    QSet<int> actPrecondNeg;
    QSet<int> actEffectPos;
    QSet<int> actEffectNeg;

    QHash<int, QString> allPredicates;

    QDomElement actNode = diagram.firstChildElement("node");

    while( !actNode.isNull())
    {
        if ( !verifyNodeType(actNode, NST_PREDICATE) )
        {
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        int predID = getIntAttribute(actNode, "id");
        QDomElement setEl = actNode.firstChildElement("set");
        while(!setEl.isNull())
        {
            QString setStr = getStrValue(setEl);

            setEl = setEl.nextSiblingElement("set");

            if ( setStr == NSPS_PRECOND_POS )
            {
                actPrecondPos.insert(predID);
                continue;
            }

            if ( setStr == NSPS_PRECOND_NEG )
            {
                actPrecondNeg.insert(predID);
                continue;
            }

            if ( setStr == NSPS_EFFECT_POS )
            {
                actEffectPos.insert(predID);
                continue;
            }

            if ( setStr == NSPS_EFFECT_NEG )
            {
                actEffectNeg.insert(predID);
                continue;
            }
        }

        QString predStr = getPredicateInstance(actNode, diagram);

        allPredicates.insert(predID, predStr);

        actNode = actNode.nextSiblingElement("node");
    }

    // the positive list to build string
    QStringList positiveList;

    // the negative list to build string
    QStringList negativeList;

    // for the precondition string, the positive list equals the actPrecondPos
    foreach (int id, actPrecondPos)
    {
        positiveList << allPredicates.value(id);
    }
    // for the precondition string, the negative list equals the actPrecondNeg
    foreach (int id, actPrecondNeg)
    {
        negativeList << allPredicates.value(id);
    }

    // build the precondition string
    QString precondString = buildSetString(positiveList, negativeList);
    positiveList.clear();
    negativeList.clear();

    // for the effect string, the positive list equals the actEffectPos,
    // the negative list equals the actEffectNeg
    foreach (int id, actEffectPos)
        positiveList << allPredicates.value(id);

    foreach (int id, actEffectNeg)
        negativeList << allPredicates.value(id);

    // build the effect string
    QString effectString = buildSetString(positiveList, negativeList);

    writeItem(indent + 1, precondString, ":precondition (and ", ")");
    writeItem(indent + 1, effectString, ":effect (and ", ")");

    writeLine(indent,")");
}

void Convertor::writeTask(QDomElement &diagram, const QString &name, const QString &targetDir)
{
    Q_ASSERT(!diagram.isNull());

    QStringList fileName;
    fileName << targetDir << "/" << domainName << "_" << name << ".pddl";

    QFile outfile(fileName.join(""));

    if ( !outfile.open(QIODevice::Truncate | QIODevice::WriteOnly) )
    {
        //QMessageBox::critical(ownerWidget, QObject::tr("Error"), QObject::tr("Can't open file:\n%1").arg(fileName.join("")));
        qCritical() << "@Can't open file:\n" << fileName.join("");
        return;
    }

    qDebug() << "$Convertor::writeProblem : writing to file: " << fileName.join("");

    outStream.setDevice(&outfile);

    outStream << "(define (problem " << name << ")" << endl;

    if (!domainName.isEmpty())
    {
        QStringList toWrite;
        toWrite << "(:domain" << domainName << ")";
        writeLine(1, toWrite.join(" "));
    }
    else
        writeLine(1, "(domain UNNAMED)");

    writeItem(1, getObjectList(diagram), "(:objects",")");

    QStringList initLiterals;
    QStringList goalLiterals;

    QDomElement actNode = diagram.firstChildElement("node");

    while ( !actNode.isNull() )
    {
        if ( !verifyNodeType(actNode, NST_PREDICATE) )
        {
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        QString predicateState = subelementTagValue(actNode, "state");

        if ( predicateState == NSTS_INIT )
        {
            initLiterals.append(getPredicateInstance(actNode, diagram, false));
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        if ( predicateState == NSTS_GOAL )
        {
            goalLiterals.append(getPredicateInstance(actNode, diagram, false));
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        qWarning() << "$Convertor::writeProblem : predicate state is not defined";
    }

    writeItem(1, initLiterals.join("\n"), "(:init", ")");

    writeLine(1, "(:goal");
    writeItem(2, goalLiterals.join("\n"), "(and ", ")");
    writeLine(1, ")");

    outStream << ")";

    outfile.close();
}

void Convertor::writeSingleTask(QDomElement &diagram, const QString &name, QFile &outfile)
{
    //Q_ASSERT(!diagram.isNull());
    if (diagram.isNull())
    {
        //return;
    }

    qDebug() << "$Convertor::writeProblem : writing to file: " << outfile.fileName();

    outStream.setDevice(&outfile);

    outStream << "(define (problem " << name << ")" << endl;

    if (!domainName.isEmpty())
    {
        QStringList toWrite;
        toWrite << "(:domain" << domainName << ")";
        writeLine(1, toWrite.join(" "));
    }
    else
        writeLine(1, "(domain UNNAMED)");

    writeItem(1, getObjectList(diagram), "(:objects",")");

    QStringList initLiterals;
    QStringList goalLiterals;

    QDomElement actNode = diagram.firstChildElement("node");

    while ( !actNode.isNull() )
    {
        if ( !verifyNodeType(actNode, NST_PREDICATE) )
        {
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        QString predicateState = subelementTagValue(actNode, "state");

        if ( predicateState == NSTS_INIT )
        {
            initLiterals.append(getPredicateInstance(actNode, diagram, false));
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        if ( predicateState == NSTS_GOAL )
        {
            goalLiterals.append(getPredicateInstance(actNode, diagram, false));
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        qWarning() << "$Convertor::writeProblem : predicate state is not defined";
    }

    writeItem(1, initLiterals.join("\n"), "(:init", ")");

    writeLine(1, "(:goal");
    writeItem(2, goalLiterals.join("\n"), "(and ", ")");
    writeLine(1, ")");

    outStream << ")";

    outfile.close();
}

QString Convertor::getDefinedTypes()
{
    QDomElement diagramElement = definitionElement.firstChildElement("diagram");
    //Q_ASSERT(!diagramElement.isNull());
    if (diagramElement.isNull())
    {
        return QString();
    }

    GraphClass * graph = new GraphClass(diagramElement);

    if (!graph->init(NST_CLASS, DEP_INHERITANCE, IGRAPH_DIRECTED))
    {
        qCritical() << "$Convertor::getDefinedTypes : inheritance graph can was not initialized";
        return QString();
    }

    QString result;

    // get the list of the root class of every different class families
    QList<int> queue = graph->childrenNodes(INVALID_ID);

    int currentParent = graph->nodeParentID(queue.first());

    qDebug() << "$Convertor::getDefinedTypes : processing queue ...";

    while (queue.size() > 0)
    {
        int actNodeID = queue.takeFirst();

        if ( currentParent != graph->nodeParentID(actNodeID) )
        {
            if (currentParent == INVALID_ID)
                result.append("- object\n");
            else
                result.append(graph->nodeLabel(currentParent).prepend("- ").append("\n"));

            currentParent = graph->nodeParentID(actNodeID);
        }

        result.append(graph->nodeLabel(actNodeID).append(" "));

        queue << graph->childrenNodes(actNodeID);

    }
    qDebug() << "$Convertor::getDefinedTypes : queue processed.";
    if (result.isEmpty())
    {
        qWarning() << "@There are no types defined in this domain";
        return QString();
    }
    else
    {
        if ( currentParent != INVALID_ID )
            result.append(graph->nodeLabel(currentParent).prepend("- "));
        else
            result.append("- object");

        return result;
    }
}

QString Convertor::getDefinedPredicates()
{
    QDomElement diagramElement = definitionElement.firstChildElement("diagram");
    //Q_ASSERT(!diagramElement.isNull());
    if (diagramElement.isNull())
    {
        return QString();
    }

    QString result;

    QSet<QString> predicateSet = selectNodeLabelSet(diagramElement, NST_PREDICATE) ;

    QList<QString> predicateList = predicateSet.toList();
    while (!predicateList.isEmpty())
    {
        result.append( getPredicateDefinition(diagramElement, predicateList.takeFirst()) );
        if (!predicateList.isEmpty())
            result.append("\n");
    }

    return result;
}

QString Convertor::getPredicateDefinition(QDomElement diagram, QString predName)
{
    Q_ASSERT(!diagram.isNull());

    // all the overloaded arguments of predicate
    QMultiMap<int, QString> arguments;

    QDomElement actNode = diagram.firstChildElement("node");
    while (!actNode.isNull())
    {
        if (!verifyNodeType(actNode, NST_PREDICATE) || (subelementTagValue(actNode, "label") != predName))
        {
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        QHash<int, QString> argHash = predicateArguments(actNode);

        for (int i=1; i <= argHash.size(); ++i)
        {
            if (!arguments.contains(i, argHash.value(i)))
                arguments.insert(i, argHash.value(i));
        }

        actNode = actNode.nextSiblingElement("node");
    }

    // all the typs of arguments, include overloaded types
    QStringList argTypes;

    QList<int> argNumList = arguments.uniqueKeys();
    foreach(int i, argNumList)
    {
        QList<QString> possibleTypes = arguments.values(i);

        if (possibleTypes.size() > 1)
        {
            QStringList typeList(possibleTypes);
            typeList.prepend("(either");
            typeList.append(")");
            argTypes.append(typeList.join(" "));
        }
        else
            argTypes.append(possibleTypes.first());
    }

    // all the names of arguments
    QStringList argNames = generateArgumentNames(argTypes.size(), 'a');

    QString result = buildPredicateString(predName, &argNames, &argTypes);

    return result;
}

QString Convertor::getPredicateInstance(QDomElement predicate, QDomElement diagram, bool ques)
{
    Q_ASSERT(!predicate.isNull());

    QMap<int, int> connections = definedArguments(predicate);

    QList<int> arguments;

    foreach (int id, connections)
    {
        QDomElement edge = findSubelementAttr(diagram, "edge", "id", QString().setNum(id));
        arguments.append(associatedNodeID(edge, false));
    }

    QStringList argNames = correspondingNodeContent(arguments, diagram, "label");

    qDebug() << "$Convertor::getPredicateInstance : arguments :" << argNames.join(" ");

    QString predName = subelementTagValue(predicate, "label");

    QString result = buildPredicateString(predName, &argNames, (QStringList*)0, ques);

    return result;
}

QString Convertor::getObjectList(QDomElement diagram)
{
    //Q_ASSERT(!diagram.isNull());
    if (diagram.isNull())
    {
        return QString();
    }

    QDomElement actNode = diagram.firstChildElement("node");

    QMultiMap<QString, QString> objectMap;
    QStringList keyList;

    while (!actNode.isNull())
    {
        if ( !verifyNodeType(actNode, NST_OBJECT) )
        {
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        QString objectName = subelementTagValue(actNode, "label");
        QString objectClass = subelementTagValue(actNode, "class");

        objectMap.insert(objectClass, objectName);
        if ( !keyList.contains(objectClass) )
            keyList.append(objectClass);

        actNode = actNode.nextSiblingElement("node");
    }

    QString result;
    while ( !keyList.isEmpty() )
    {
        QString currentClass = keyList.takeFirst();
        QList<QString> members = objectMap.values(currentClass);

        foreach (QString obj, members)
        {
            result.append(" ");
            result.append(obj);
        }

        result.append(" - ");
        result.append(currentClass);
        if (!keyList.isEmpty())
            result.append("\n");
    }

    return result;
}

QString Convertor::buildPredicateString(QString &name, QStringList *argNames, QStringList *argTypes, bool ques)
{
    QString result;

    result.append("(");
    result.append(name);

    while( !argNames->isEmpty() )
    {
        if (ques)
            result.append(" ?");
        else
            result.append(" ");

        result.append(argNames->takeFirst());

        if (argTypes && !argTypes->isEmpty())
        {
            result.append(" - ");
            result.append(argTypes->takeFirst());
        }
    }

    result.append(")");

    return result;
}

QString Convertor::buildSetString(QStringList &positive, QStringList &negative)
{
    QString result = positive.join("\n");

//    foreach(QString pred, negative)
//    {
//        result.append("\n(not ");
//        result.append(pred);
//        result.append(")");
//    }
    result.append("\n");
    for (int i = 0; i < negative.size(); i++)
    {
        QString pred = negative.at(i);
        if (i > 0)
        {
            result.append("\n");
        }
        result.append("(not ");
        result.append(pred);
        result.append(")");
    }

    return result;
}

QStringList Convertor::generateArgumentNames(int size, char startingChar)
{
    QStringList result;


    for (int i = 0; i < size; ++i)
    {
        QChar test(startingChar + i);

        if (!test.isPrint())
            qWarning() << "$Convertor::generateArgumentNames : non printable character";

        result << QChar(startingChar + i);
    }

    return result;
}

QString Convertor::getActionParameters(QDomElement diagram)
{
    //Q_ASSERT(!diagram.isNull());
    if (diagram.isNull())
    {
        return QString();
    }

    QString result = ":parameters (";
    QDomElement actNode = diagram.firstChildElement("node");

    while( !actNode.isNull() )
    {
        if ( !verifyNodeType(actNode, NST_VARIABLE) )
        {
            actNode = actNode.nextSiblingElement("node");
            continue;
        }

        QString varName = subelementTagValue(actNode, "label");
        QString varType = subelementTagValue(actNode, "class");

        result.append("?");
        result.append(varName);
        result.append(" - ");
        result.append(varType);

        actNode = actNode.nextSiblingElement("node");
        if (!actNode.isNull())
            result.append(" ");
    }

    result.append(")");

    return result;
}




