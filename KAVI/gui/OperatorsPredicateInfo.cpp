#include "OperatorsPredicateInfo.h"
#include "DataWidget.h"
#include "NodeStructure.h"
#include "CheckUtils.h"
#include "EditWidget.h"



using namespace CheckUtils;
using namespace XMLUtils;

OperatorsPredicateInfo::OperatorsPredicateInfo(DataWidget *data, const DataWidget *definition, int predicateID, QWidget *parent)
    :InfoPanel(data, predicateID, parent)
{
    ui.setupUi(this);

    xmlDefinition = definition;

    predicateDef = selectPredicateDefinition(definition->diagramRootElement(), data->findNode(predicateID));

    if ( verifyNodeType(selectedNode, NST_PREDICATE) )
    {
        ui.predicateName->setText(subelementTagValue(selectedNode, "label"));
        fillArgumentTable();
        setCheckBoxes();
    }
}

void OperatorsPredicateInfo::updateContent()
{
    ui.argumentsTable->clearContents();
    predicateDef = selectPredicateDefinition(xmlDefinition->diagramRootElement(), selectedNode);
    fillArgumentTable();
}

void OperatorsPredicateInfo::on_moveDown_pressed()
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

void OperatorsPredicateInfo::on_moveUp_pressed()
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

void OperatorsPredicateInfo::on_precondCheck_toggled(bool checked)
{
    ui.effectPosCheck->setEnabled( !checked && !ui.effectNegCheck->isChecked() );

    if (checked)
        includeInSet(NSPS_PRECOND);
    else
        excludeFromSet(NSPS_PRECOND);

    xmlData->refreshNode(getIntAttribute(selectedNode, "id"));

    emit madeChange(PredicateSetChanged);
}

void OperatorsPredicateInfo::on_effectPosCheck_toggled(bool checked)
{
    ui.precondCheck->setEnabled(!checked);
    ui.effectNegCheck->setEnabled(!checked);

    if (checked)
        includeInSet(NSPS_EFFECT_POS);
    else
        excludeFromSet(NSPS_EFFECT_POS);

    xmlData->refreshNode(getIntAttribute(selectedNode, "id"));

    emit madeChange(PredicateSetChanged);
}

void OperatorsPredicateInfo::on_effectNegCheck_toggled(bool checked)
{
    ui.effectPosCheck->setEnabled( !checked && !ui.precondCheck->isChecked() );

    if (checked)
        includeInSet(NSPS_EFFECT_NEG);
    else
        excludeFromSet(NSPS_EFFECT_NEG);

    xmlData->refreshNode(getIntAttribute(selectedNode,"id"));

    emit madeChange(PredicateSetChanged);
}

void OperatorsPredicateInfo::fillArgumentTable()
{
    ui.argumentsTable->setColumnCount(OP_COLUMN_COUNT);
    ui.argumentsTable->setHorizontalHeaderLabels(QStringList() << "Argument class" << "Variable");

    QDomElement connections = predicateDef.firstChildElement("connections");
    QDomElement localConn = selectedNode.firstChildElement("connections");

    QStringList verticalLabels;

    if ( connections.isNull())
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

        // set the content for Variable
        if ( !localElement.isNull() )
        {
            // this argument type associates with one variable
            int locEdgeID = getIntValue(localElement);

            QPair<int, int> nodeLoc = xmlData->associatedNodes(locEdgeID);

            ui.argumentsTable->setItem(i, OP_VARIABLE_COLUMN,
                new QTableWidgetItem(xmlData->getNodeData(nodeLoc.second, "label"), QTableWidgetItem::Type));
        }
        else
        {
            // this argument type does not associate with any variable
            ui.argumentsTable->setItem(i, OP_VARIABLE_COLUMN,
                new QTableWidgetItem(OP_EMPTY_ARGUMENT, QTableWidgetItem::Type));
        }

        int defEdgeID = getIntValue(argList.at(i).toElement());

        QPair<int, int> nodeDef = xmlDefinition->associatedNodes(defEdgeID);

        QTableWidgetItem * newItem = new QTableWidgetItem(xmlDefinition->getNodeData(nodeDef.second, "label"),
                                    QTableWidgetItem::Type);
        newItem->setFlags(Qt::ItemIsEnabled);
        newItem->setBackground(QBrush(Qt::lightGray));

        // set the content for argument class
        ui.argumentsTable->setItem(i, OP_CLASS_COLUMN, newItem);
    }

    ui.argumentsTable->setVerticalHeaderLabels(verticalLabels);
}

void OperatorsPredicateInfo::setCheckBoxes()
{
    QDomElement setElem = selectedNode.firstChildElement("set");

    while ( !setElem.isNull() )
    {

        if ( getStrValue(setElem) == NSPS_EFFECT_POS )
        {
            ui.effectPosCheck->setChecked(true);
        }

        if ( getStrValue(setElem) == NSPS_EFFECT_NEG )
        {
            ui.effectNegCheck->setChecked(true);
        }

        if ( getStrValue(setElem) == NSPS_PRECOND )
        {
            ui.precondCheck->setChecked(true);
        }

        setElem = setElem.nextSiblingElement("set");
    }
}

void OperatorsPredicateInfo::moveCurrentRow(RowMoveDirection direction)
{
    QTableWidgetItem * current = ui.argumentsTable->currentItem();
    int targetIndex;
    switch (direction)
    {
    case MoveUp:
        if ( ((targetIndex = current->row() - 1) >= 0) &&
             (current->column() == OP_VARIABLE_COLUMN) )
        {
            swapConnections(current->row(), targetIndex);
            swapTableItems(current, ui.argumentsTable->item(targetIndex, OP_VARIABLE_COLUMN));
        }

        break;
    case MoveDown:
        if ( ((targetIndex = current->row() + 1) < ui.argumentsTable->rowCount()) &&
             (current->column() == OP_VARIABLE_COLUMN) )
        {
            swapConnections(current->row(),targetIndex);
            swapTableItems(current, ui.argumentsTable->item(targetIndex, OP_VARIABLE_COLUMN));
        }
        break;
    }
}

void OperatorsPredicateInfo::swapTableItems(QTableWidgetItem *first, QTableWidgetItem *second)
{
    QString temp = first->text();
    first->setText(second->text());
    second->setText(temp);
    ui.argumentsTable->setCurrentItem(second);
}

void OperatorsPredicateInfo::swapConnections(int firstArg, int secondArg)
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

void OperatorsPredicateInfo::includeInSet(QString set)
{
    QDomDocument parentDoc = selectedNode.ownerDocument();

    if ( !findSubelementVal(selectedNode, "set", set).isNull() )
        return;

    QDomElement neighbour = selectedNode.lastChildElement("set");

    // predicate set should follow the node type
    if ( neighbour.isNull() )
        neighbour = selectedNode.firstChildElement("type");

    QDomElement newSet = parentDoc.createElement("set");
    setStrValue(newSet, set);

    selectedNode.insertAfter(newSet, neighbour);
}

void OperatorsPredicateInfo::excludeFromSet(QString set)
{
    QDomElement removed = findSubelementVal(selectedNode, "set" ,set);
    if ( removed.isNull() )
        return;

    selectedNode.removeChild(removed);
}




