#include "DefinitionEdit.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "GraphClass.h"
#include "XMLUtils.h"
#include "CheckUtils.h"
#include "InfoPanel.h"
#include "DefinitionPredicateDialog.h"
#include "DefinitionPredicateInfo.h"
#include "DefinitionClassDialog.h"
#include "DefinitionClassInfo.h"
#include "igraph.h"


using namespace XMLUtils;
using namespace CheckUtils;

DefinitionEdit::DefinitionEdit(QWidget *parent)
    :EditWidget(parent)
{
    allowedEdgeMask = DEP_INHERITANCE | DEP_ASSOCIATION;
    allowedNodeMask = NST_CLASS | NST_PREDICATE;

    classKB = new KAVIClassKB();
    createFile(KBDIR, CLASSKBFILE);
    QString filePath;
    filePath.append(KBDIR).append(CLASSKBFILE);
    QFile classBaseFile(filePath);
    classKB->loadKB(classBaseFile);

    predicateKB = new KAVIPredicateKB();
    createFile(KBDIR, PREDICATEKBFILE);
    filePath.clear();
    filePath.append(KBDIR).append(PREDICATEKBFILE);
    QFile predicateBaseFile(filePath);
    predicateKB->loadKB(predicateBaseFile);
}

void DefinitionEdit::saveKB()
{
    QStringList definedClasses = xmlData->getNodeLabelList(NST_CLASS);
    foreach (QString str, definedClasses) {
        classKB->addClass(str);
    }
    QFile classBaseFile(CLASSKBFILE);
    classKB->saveKB(classBaseFile);


    NodeStructure predicateNode;
    predicateNode.setData(nodeType, NST_PREDICATE);
    QList<QDomElement> predicatesElement = xmlData->selectMatchingElementList(predicateNode);
    foreach (QDomElement predicate, predicatesElement) {
        QHash<int, QString> arguments = predicateArguments(predicate);
        QString predicateName = subelementTagValue(predicate, "label");
        QString predicateSign;
        predicateSign.append(predicateName);
        QList<int> argNumbers = arguments.keys();
        qSort(argNumbers.begin(), argNumbers.end());
        for (int i = 0; i < argNumbers.size(); i++)
        {
            predicateSign.append(" ");
            predicateSign.append(arguments.value(argNumbers[i]));
        }
        predicateKB->addPredicate(predicateSign);
    }
    QFile predicateBaseFile(PREDICATEKBFILE);
    predicateKB->saveKB(predicateBaseFile);
}

bool DefinitionEdit::createFile(QString filePath, QString fileName)
{
    QDir tempDir;
    //临时保存程序当前路径
    QString currentDir = tempDir.currentPath();
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(filePath))
    {
        //qDebug()<<"不存在该路径"<<endl;
        tempDir.mkpath(filePath);
    }
    QFile *tempFile = new QFile;
    //将程序的执行路径设置到filePath下
    tempDir.setCurrent(filePath);
    //qDebug()<<tempDir.currentPath();
    //检查filePath路径下是否存在文件fileName,如果停止操作。
    if(tempFile->exists(fileName))
    {
        //qDebug()<<"文件存在";
        return true;
    }
    //此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
    tempFile->setFileName(fileName);
    if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        //qDebug()<<"打开失败";
        return false;
    }
    tempFile->close();
    //将程序当前路径设置为原来的路径
    tempDir.setCurrent(currentDir);
    //qDebug()<<tempDir.currentPath();

    return true;
}

void DefinitionEdit::defineRectangleNode(QPointF pos, int newID)
{
    DefinitionClassDialog *dialog = new DefinitionClassDialog(classKB, this);

    if (dialog->exec() == QDialog::Accepted)
    {
        QString newClassName = dialog->className();
        QStringList definedClasses = xmlData->getNodeLabelList(NST_CLASS);

        if ( definedClasses.contains(newClassName, Qt::CaseInsensitive) )
        {
            QMessageBox::warning(this, tr("KAVI"), tr("Class name must be unique."));
            return;
        }

        if ( newClassName.toLower() == "object" )
        {
            QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
            return;
        }

        if ( !nameChecker.exactMatch(newClassName) )
        {
            QMessageBox::warning(this, tr("KAVI"),
            tr("Name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
            return;
        }

        NodeStructure newNode;

        newNode.setData(nodePosition, pos);
        newNode.setData(nodeID, newID);
        newNode.setData(nodeLabel, newClassName);
        newNode.setData(nodeType, NST_CLASS);

        xmlData->addDataNode(newNode);

        emit sceneChanged(RectNodeAdded);
    }
}

void DefinitionEdit::defineEllipseNode(QPointF pos, int newID)
{
    DefinitionPredicateDialog *dialog = new DefinitionPredicateDialog(predicateKB, this);

    if (dialog->exec() == QDialog::Accepted)
    {
        QString predicateSign = dialog->predicateSign();
        QStringList predicateSignList = predicateSign.split(" ");
        QString newPredicateName = predicateSignList[0];

        if ( newPredicateName.isEmpty() )
        {
            QMessageBox::information(this, tr("KAVI"), tr("Predicate name can't be empty."));
            return;
        }

        if ( !nameChecker.exactMatch(newPredicateName) )
        {
            QMessageBox::warning(this, tr("KAVI"),
            tr("Name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
            return;
        }

        NodeStructure newNode;

        newNode.setData(nodePosition, pos);
        newNode.setData(nodeID, newID);
        newNode.setData(nodeLabel, newPredicateName);
        newNode.setData(nodeType, NST_PREDICATE);

        xmlData->addDataNode(newNode);
        emit sceneChanged(EllipseNodeAdded);


        QStringList definedClasses = xmlData->getNodeLabelList(NST_CLASS);

        for (int i = 1; i < predicateSignList.size(); i++)
        {
            if (definedClasses.contains(predicateSignList[i], Qt::CaseInsensitive))
            {
                NodeStructure oldNode;

                oldNode.setData(nodeLabel, predicateSignList[i]);
                oldNode.setData(nodeType, NST_CLASS);

                int oldNodeID = xmlData->getMatchingNodeID(oldNode);
                QPointF oldNodePos = xmlData->getNodePos(oldNodeID);

                int sX = oldNodePos.x() > pos.x()? (pos.x() + diagram->getNode(newID)->getSize().width()/2) :
                                                  (pos.x() - diagram->getNode(newID)->getSize().width()/2);
                int sY = oldNodePos.y() > pos.y()? (pos.y() + diagram->getNode(newID)->getSize().height()/2) :
                                                  (pos.y() - diagram->getNode(newID)->getSize().height()/2);
                QPointF sPoint(sX, sY);

                int eX = pos.x() > oldNodePos.x()? (oldNodePos.x() + diagram->getNode(oldNodeID)->getSize().width()/2) :
                                              (oldNodePos.x() - diagram->getNode(oldNodeID)->getSize().width()/2);
                int eY = pos.y() > oldNodePos.y()? (oldNodePos.y() + diagram->getNode(oldNodeID)->getSize().height()/2) :
                                              (oldNodePos.y() - diagram->getNode(oldNodeID)->getSize().height()/2);
                QPointF ePoint(eX, eY);

                EdgeStructure newEdge;
                newEdge.id = diagram->newEdgeID();
                newEdge.startPos = sPoint;
                newEdge.endPos = ePoint;
                newEdge.startNodeID = newID;
                newEdge.endNodeID = oldNodeID;
                newEdge.purpose = DEP_ASSOCIATION;

                makeConnection(newEdge, i);
            }
            else
            {
                QPointF newNodePos = diagram->newNodePos(pos);
                int newNodeID = diagram->newNodeID();
                QString newNodelabel = predicateSignList[i];

                newNode.setData(nodePosition, newNodePos);
                newNode.setData(nodeID, newNodeID);
                newNode.setData(nodeLabel, newNodelabel);
                newNode.setData(nodeType, NST_CLASS);

                xmlData->addDataNode(newNode);
                emit sceneChanged(RectNodeAdded);


                int sX = newNodePos.x() > pos.x()? (pos.x() + diagram->getNode(newID)->getSize().width()/2) :
                                                  (pos.x() - diagram->getNode(newID)->getSize().width()/2);
                int sY = newNodePos.y() > pos.y()? (pos.y() + diagram->getNode(newID)->getSize().height()/2) :
                                                  (pos.y() - diagram->getNode(newID)->getSize().height()/2);
                QPointF sPoint(sX, sY);

                int eX = pos.x() > newNodePos.x()? (newNodePos.x() + diagram->getNode(newNodeID)->getSize().width()/2) :
                                              (newNodePos.x() - diagram->getNode(newNodeID)->getSize().width()/2);
                int eY = pos.y() > newNodePos.y()? (newNodePos.y() + diagram->getNode(newNodeID)->getSize().height()/2) :
                                              (newNodePos.y() - diagram->getNode(newNodeID)->getSize().height()/2);
                QPointF ePoint(eX, eY);

                EdgeStructure newEdge;
                newEdge.id = diagram->newEdgeID();
                newEdge.startPos = sPoint;
                newEdge.endPos = ePoint;
                newEdge.startNodeID = newID;
                newEdge.endNodeID = newNodeID;
                newEdge.purpose = DEP_ASSOCIATION;

                makeConnection(newEdge, i);
            }
        }       
    }
}

/*
void DefinitionEdit::renumberPredicateArguments(QDomElement toRenumber)
{
    if ( toRenumber.isNull() || (toRenumber.tagName() != "starts") )
        return;

    int value = getIntAttribute(toRenumber, "argn");
    value--;
    while ( !toRenumber.isNull() )
    {
        setIntAttribute(toRenumber, "argn", value);
        value ++;
        toRenumber = toRenumber.nextSiblingElement("starts");
    }
}
*/

void DefinitionEdit::displayInfo(int nodeID)
{
    Q_ASSERT(nodeID != INVALID_ID);

    qDebug() << "$DefinitionEdit::displayInfo";
    char clickedType = xmlData->readNodeType(nodeID);

    InfoPanel * infoWidget = (InfoPanel*)NULL;

    if (clickedType == NST_PREDICATE)
        infoWidget = new DefinitionPredicateInfo(xmlData, nodeID);

    if (clickedType == NST_CLASS)
        infoWidget = new DefinitionClassInfo(xmlData, nodeID);

    if (infoWidget == NULL)
    {
        qWarning() << "$DefinitionEdit::displayInfo : infoWidget == NULL";
        return;
    }

    connect(infoWidget, SIGNAL(madeChange(int)), this, SLOT(handleExternChange(int)));

    emit updateInfoPanel(infoWidget);
}

void DefinitionEdit::makeConnection(EdgeStructure &edge, int &argNum)
{
    if ( edge.purpose == DEP_INHERITANCE )
    {
        xmlData->addDataEdge(edge);
        xmlData->connectEdgeToNode(edge.id, EDGE_START, edge.startNodeID);
        xmlData->connectEdgeToNode(edge.id, EDGE_END, edge.endNodeID);

        emit sceneChanged(InherEdgeAdded);
        return;
    }

    Q_ASSERT(argNum != INVALID_ARGN);

    if ( edge.purpose == DEP_ASSOCIATION )
    {
        xmlData->addDataEdge(edge);
        QDomElement connEl = xmlData->connectEdgeToNode(edge.id, EDGE_START, edge.startNodeID);

        setIntAttribute(connEl, "argn", argNum);
        xmlData->connectEdgeToNode(edge.id, EDGE_END, edge.endNodeID);

        emit sceneChanged(AssocEdgeAdded);
        return;
    }

    qCritical() << "$DefinitionEdit::makeConnection : error in edge definition !";
}

void DefinitionEdit::changeConnection(EdgeStructure &edge, bool isStart, int toNode, int &argNum)
{
    xmlData->changeEdgeAssociation(edge.id, isStart, toNode);

    if ( edge.purpose == DEP_ASSOCIATION )
    {
        QDomElement toRenumber;

        if (isStart)
        {
            toRenumber = xmlData->disconnectEdgeFromNode(edge.id, isStart, edge.startNodeID);
            renumberPredicateArguments(toRenumber);
        }
        else
            xmlData->disconnectEdgeFromNode(edge.id, isStart, edge.endNodeID);

        QDomElement connEl = xmlData->connectEdgeToNode(edge.id, isStart, toNode);

        if (isStart)
        {
            setIntAttribute(connEl, "argn", argNum);
        }

        emit sceneChanged(AssocEdgeReconnected);
        return;
    }

    if ( edge.purpose == DEP_INHERITANCE )
    {
        if (isStart)
            xmlData->disconnectEdgeFromNode(edge.id, isStart, edge.startNodeID);
        else
            xmlData->disconnectEdgeFromNode(edge.id, isStart, edge.endNodeID);

        xmlData->connectEdgeToNode(edge.id, isStart, toNode);

        emit sceneChanged(InherEdgeReconnected);
        return;
    }

    Q_ASSERT(false);
}

void DefinitionEdit::deleteConnection(int edgeID)
{
    QDomElement edge = xmlData->findEdge(edgeID);
    if ( verifyEdgePurpose(edge, DEP_ASSOCIATION) )
        emit sceneChanged(AssocEdgeDeleted);

    if ( verifyEdgePurpose(edge, DEP_INHERITANCE) )
        emit sceneChanged(InherEdgeDeleted);

    EdgeDefinition removedEdge = xmlData->delDataEdge(edgeID);

    QDomElement nextConnection = xmlData->disconnectEdgeFromNode(edgeID, EDGE_START, removedEdge.first);
    xmlData->disconnectEdgeFromNode(edgeID, EDGE_END, removedEdge.second);

    if ( xmlData->readNodeType(removedEdge.first) == NST_PREDICATE )
        renumberPredicateArguments(nextConnection);
}

bool DefinitionEdit::verifyEdge(EdgeStructure &edge, int &argNum)
{
    if ( !EditWidget::verifyEdge(edge, argNum) )
        return false;

    if ( edge.purpose == DEP_INHERITANCE )
    {
        qDebug() << "$DefinitionEdit::verifyEdge: DEP_INHERITANCE";

        GraphClass * inheritanceStructure = new GraphClass(xmlData->diagramRootElement());
        inheritanceStructure->init(NST_CLASS, DEP_INHERITANCE, IGRAPH_DIRECTED);

        if ( inheritanceStructure->edgeCnt(edge.endNodeID, IGRAPH_IN) > 0 )
        {
            qWarning() << "@Multiple inheritance is not allowed.";
            delete inheritanceStructure;
            return false;
        }

        qDebug() << "$DefinitionEdit::makeConnection : startID:" << edge.startNodeID << "endID:" << edge.endNodeID;
        inheritanceStructure->addEdge(edge.startNodeID, edge.endNodeID);

        if ( inheritanceStructure->containsCycle() )
        {
            qWarning() << "@Cyclic inheritance is not allowed.";
            delete inheritanceStructure;
            return false;
        }

    }

    if ( edge.purpose == DEP_ASSOCIATION )
    {
        qDebug() << "$DefinitionEdit::verifyEdge: DEP_ASSOCIATION";

        QDomElement predicate = xmlData->findNode(edge.startNodeID);
        int argCnt = getArgCount(predicate);

        NodeStructure tempNode;
        tempNode.setData(nodeType, NST_PREDICATE);
        tempNode.setData(nodeLabel, subelementTagValue(predicate, "label"));

        QList<QDomElement> cloneList = xmlData->selectMatchingElementList(tempNode);
        if ( cloneList.size() > 1 )
        {
            qDebug() << "$DefinitionEdit::verifyEdge: overloaded";

            int maxArgNum = -1;
            foreach(QDomElement clone, cloneList)
            {
                int clArgs = getArgCount(clone);
                if (clArgs > maxArgNum)
                    maxArgNum = clArgs;
            }

            if ( (argCnt + 1) > (maxArgNum + 1) )
            {
                qWarning() << "@All overloaded predicates must be updated at once.";
                return false;
            }

            argNum = argCnt + 1;
        }
        else
        {
            qDebug() << "$DefinitionEdit::verifyEdge: not overloaded";

            argNum = argCnt + 1;
        }
    }

    return true;
}

EditWidget::ReconnectionValue DefinitionEdit::verifyReconnection(EdgeStructure &edge, int newNodeID, bool startMoved, int &argNum)
{
    if ( newNodeID <= 0)
    {
        qWarning() << "@No target for edge reconnection";
        return NoChange;
    }

    // the node's ID for node which is connected by the move edgepoint
    int fromNodeID;
    // the node's ID for node which is connected by the no-move edgepoint
    int stableEndID;

    if ( startMoved )
    {
        fromNodeID = edge.startNodeID;
        stableEndID = edge.endNodeID;
    }
    else
    {
        fromNodeID = edge.endNodeID;
        stableEndID = edge.startNodeID;
    }

    // edge connect the same node
    if ( stableEndID == newNodeID )
    {
        qWarning() << "@Edge has to connect two different nodes";
        return NoChange;
    }

    // edgepoint does not move outside the origial node
    if ( fromNodeID == newNodeID )
        return PosChange;

    char toNodeType = xmlData->readNodeType(newNodeID);
    char stableEndType = xmlData->readNodeType(stableEndID);

    char newEdgePurpose;

    // get the purpose of the expected new edge
    if (startMoved)
        newEdgePurpose = determineEdgePurpose(toNodeType, stableEndType);
    else
        newEdgePurpose = determineEdgePurpose(stableEndType, toNodeType);

    // edge's purpose can not change
    if ( newEdgePurpose != edge.purpose )
    {
        qWarning() << "@Change of edge purpose through reconnection is not allowed";
        return NoChange;
    }

    if ( edge.purpose == DEP_INHERITANCE )
    {
        GraphClass * inheritanceStructure = new GraphClass(xmlData->diagramRootElement());
        inheritanceStructure->init(NST_CLASS,DEP_INHERITANCE, IGRAPH_DIRECTED);

        if ( !startMoved && inheritanceStructure->edgeCnt(newNodeID, IGRAPH_IN) > 0 )
        {
            qWarning() << "@Multiple inheritance is not allowed.";
            delete inheritanceStructure;
            return NoChange;
        }

        // remove edge in old connection in the graph
        qDebug() << "$verifyReconnection: start=" << edge.startNodeID << "end=" << edge.endNodeID;
        inheritanceStructure->removeEdge(edge.startNodeID, edge.endNodeID);

        if ( startMoved )
            inheritanceStructure->addEdge(newNodeID, stableEndID);
        else
            inheritanceStructure->addEdge(stableEndID, newNodeID);

        if ( inheritanceStructure->containsCycle() )
        {
            qWarning() << "@Cyclic inheritance is not allowed.";
            delete inheritanceStructure;
            return NoChange;
        }
    }

    // get the new argument number
    if (startMoved)
    {
        QDomElement newStartNode = xmlData->findNode(newNodeID);
        Q_ASSERT(!newStartNode.isNull());

        if (verifyNodeType(newStartNode, NST_PREDICATE))
            argNum = getArgCount(newStartNode) + 1;
    }

    return PosAssocChange;
}




