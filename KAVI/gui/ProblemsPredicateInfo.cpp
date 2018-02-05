#include "ProblemsPredicateInfo.h"
#include "DataWidget.h"
#include "NodeStructure.h"
#include "CheckUtils.h"
#include "EditWidget.h"



using namespace XMLUtils;
using namespace CheckUtils;

ProblemsPredicateInfo::ProblemsPredicateInfo(DataWidget *data, const DataWidget *definition, int predicateID, QWidget *parent)
    :InfoPanel(data, predicateID, parent)
{
    ui.setupUi(this);

    xmlDefinition = definition;

    predicateDef = selectPredicateDefinition(definition->diagramRootElement(), data->findNode(predicateID));

    if ( verifyNodeType(selectedNode,NST_PREDICATE) )
    {
        ui.predicateName->setText(subelementTagValue(selectedNode, "label"));
        fillArgumentTable();
        setRadioButtons();
    }
}

void ProblemsPredicateInfo::updateContent()
{
    ui.argumentsTable->clearContents();
    predicateDef = selectPredicateDefinition(xmlDefinition->diagramRootElement(), selectedNode);
    fillArgumentTable();
}

void ProblemsPredicateInfo::on_moveDown_pressed()
{
    if (ui.argumentsTable->selectedItems().count() > 0)
    {
        moveCurrentRow(MoveDown);
        emit madeChange(ArgumentOrderChanged);
    }
    else
    {
        qWarning() << "@Please select one argument.";
    }

    //emit madeChange(ArgumentOrderChanged);
}

void ProblemsPredicateInfo::on_moveUp_pressed()
{
    if (ui.argumentsTable->selectedItems().count() > 0)
    {
        moveCurrentRow(MoveUp);
        emit madeChange(ArgumentOrderChanged);
    }
    else
    {
        qWarning() << "@Please select one argument.";
    }

    //emit madeChange(ArgumentOrderChanged);
}

void ProblemsPredicateInfo::on_initRadioButton_toggled(bool checked)
{
    if (checked)
    {
        setPredicateState(NSTS_INIT);
        xmlData->refreshNode(getIntAttribute(selectedNode, "id"));
        emit madeChange(PredicateStateChanged);
    }

    //emit madeChange(PredicateStateChanged);
}

void ProblemsPredicateInfo::on_goalRadioButton_toggled(bool checked)
{
    if (checked)
    {
        setPredicateState(NSTS_GOAL);
        xmlData->refreshNode(getIntAttribute(selectedNode, "id"));
        emit madeChange(PredicateStateChanged);
    }

    //emit madeChange(PredicateStateChanged);
}

void ProblemsPredicateInfo::fillArgumentTable()
{
    ui.argumentsTable->setColumnCount(PR_COLUMN_COUNT);
    ui.argumentsTable->setHorizontalHeaderLabels(QStringList() << "Argument class" << "Object");

    QDomElement connections = predicateDef.firstChildElement("connections");
    QDomElement localConn = selectedNode.firstChildElement("connections");

    QStringList verticalLabels;

    if (connections.isNull())
        return;

    QDomNodeList argList = connections.childNodes();

    QDomElement localElement = QDomElement();

    ui.argumentsTable->setRowCount(argList.count());
    for (int i=0; i < argList.count(); ++i)
    {
        QString argNum = argList.at(i).toElement().attribute("argn");
        verticalLabels.append(argNum);

        if ( !localConn.isNull() )
            localElement = findSubelementAttr(localConn, "starts", "argn", argNum);

        // set the content for object
        if ( !localElement.isNull() )
        {
            // this argument type associates with one object
            int locEdgeID = getIntValue(localElement);

            QPair<int, int> nodeLoc = xmlData->associatedNodes(locEdgeID);

            ui.argumentsTable->setItem(i, PR_OBJECT_COLUMN,
                new QTableWidgetItem(xmlData->getNodeData(nodeLoc.second, "label"), QTableWidgetItem::Type));
        }
        else
        {
            // this argument type does not associate with any object
            ui.argumentsTable->setItem(i, PR_OBJECT_COLUMN,
                new QTableWidgetItem(PR_EMPTY_ARGUMENT, QTableWidgetItem::Type));
        }

        int defEdgeID = getIntValue(argList.at(i).toElement());

        QPair<int, int> nodeDef = xmlDefinition->associatedNodes(defEdgeID);

        QTableWidgetItem * newItem = new QTableWidgetItem(xmlDefinition->getNodeData(nodeDef.second, "label"),
                                    QTableWidgetItem::Type);
        newItem->setFlags(Qt::ItemIsEnabled);
        newItem->setBackground(QBrush(Qt::lightGray));

        // set the content for argument class
        ui.argumentsTable->setItem(i, PR_CLASS_COLUMN, newItem);
    }

    ui.argumentsTable->setVerticalHeaderLabels(verticalLabels);
}

void ProblemsPredicateInfo::setRadioButtons()
{
    QDomElement state = selectedNode.firstChildElement("state");

    Q_ASSERT(!state.isNull());

    if ( getStrValue(state) == NSTS_INIT )
    {
        ui.initRadioButton->setChecked(true);
        return;
    }

    if ( getStrValue(state) == NSTS_GOAL )
    {
        ui.goalRadioButton->setChecked(true);
        return;
    }

    Q_ASSERT(false);
}

void ProblemsPredicateInfo::moveCurrentRow(RowMoveDirection direction)
{
    QTableWidgetItem * current = ui.argumentsTable->currentItem();

    if (current->column() != PR_OBJECT_COLUMN)
        return;

    int targetIndex;
    switch (direction)
    {
    case MoveUp:
        if ( ((targetIndex = current->row() - 1) >= 0) )
        {
            swapConnections(current->row(), targetIndex);
            swapTableItems(current, ui.argumentsTable->item(targetIndex, PR_OBJECT_COLUMN));
        }

        break;
    case MoveDown:
        if ( ((targetIndex = current->row() + 1) < ui.argumentsTable->rowCount()) )
        {
            swapConnections(current->row(), targetIndex);
            swapTableItems(current, ui.argumentsTable->item(targetIndex, PR_OBJECT_COLUMN));
        }
        break;
    }
}

void ProblemsPredicateInfo::swapTableItems(QTableWidgetItem *first, QTableWidgetItem *second)
{
    QString temp = first->text();
    first->setText(second->text());
    second->setText(temp);
    ui.argumentsTable->setCurrentItem(second);
}

void ProblemsPredicateInfo::swapConnections(int firstArg, int secondArg)
{
    QDomElement connections = selectedNode.firstChildElement("connections");

    QString firstArgNum;
    QString secondArgNum;

    firstArgNum.setNum(firstArg + 1, 10);
    secondArgNum.setNum(secondArg + 1, 10);

    QDomElement firstConn = findSubelementAttr(connections, "starts", "argn", firstArgNum);
    QDomElement secondConn = findSubelementAttr(connections, "starts", "argn", secondArgNum);

    if ( secondConn.isNull() )
    {
        //firstConn.setAttribute("argn", secondArg);
        firstConn.setAttribute("argn", secondArg + 1);
    }
    else
    {
        int firstValue = getIntValue(firstConn);
        int secondValue = getIntValue(secondConn);

        setIntValue(firstConn, secondValue);
        setIntValue(secondConn, firstValue);
    }
}

void ProblemsPredicateInfo::setPredicateState(QString newState)
{
    QDomElement stateEl = selectedNode.firstChildElement("state");

    if ( stateEl.isNull() )
    {
        QDomDocument owner = selectedNode.ownerDocument();
        stateEl = owner.createElement("state");
        selectedNode.appendChild(stateEl);
    }

    setStrValue(stateEl, newState);
}




