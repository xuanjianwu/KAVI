#include "DefinitionPredicateInfo.h"
#include "DataWidget.h"
#include "NodeStructure.h"
#include "EditWidget.h"

KAVI_NS_BEGIN

using namespace XMLUtils;

DefinitionPredicateInfo::DefinitionPredicateInfo(DataWidget *data, int predicateID, QWidget *parent)
    :InfoPanel(data, predicateID, parent)
{
    ui.setupUi(this);

    Q_ASSERT(!selectedNode.isNull());

    if ( verifyNodeType(selectedNode, NST_PREDICATE) )
    {
        ui.predicateName->setText(subelementTagValue(selectedNode, "label"));

        fillArgumentTable();
    }
}

void DefinitionPredicateInfo::updateContent()
{
    ui.argumentsTable->clearContents();
    fillArgumentTable();
}

void DefinitionPredicateInfo::on_moveDown_pressed()
{
    if (ui.argumentsTable->selectedItems().count() > 0)
    {
        moveCurrentRow(MoveDown);
        emit madeChange(ArgumentOrderChanged);
    }
    else
    {
        qWarning() << "Please select one argument.";
    }

    //emit madeChange(ArgumentOrderChanged);
}

void DefinitionPredicateInfo::on_moveUp_pressed()
{
    if (ui.argumentsTable->selectedItems().count() > 0)
    {
        moveCurrentRow(MoveUp);
        emit madeChange(ArgumentOrderChanged);
    }
    else
    {
        qWarning() << "Please select one argument.";
    }

    //emit madeChange(ArgumentOrderChanged);
}

void DefinitionPredicateInfo::fillArgumentTable()
{
    ui.argumentsTable->setColumnCount(COLUMN_COUNT);
    ui.argumentsTable->setHorizontalHeaderLabels(QStringList("Argument class"));

    QDomElement connections = selectedNode.firstChildElement("connections");

    if (connections.isNull())
    {
        ui.argumentsTable->setRowCount(0);
        return;
    }

    QStringList verticalLabels;

    QDomNodeList argList = connections.childNodes();

    ui.argumentsTable->setRowCount(argList.count());
    for (int i=0; i < argList.count(); ++i)
    {
        verticalLabels.append(argList.at(i).toElement().attribute("argn"));

        int edgeID = getIntValue(argList.at(i).toElement());

        QPair<int, int> nodes = xmlData->associatedNodes(edgeID);

        ui.argumentsTable->setItem(i, CLASS_COLUMN,
                new QTableWidgetItem(xmlData->getNodeData(nodes.second, "label"), QTableWidgetItem::Type));
    }

    ui.argumentsTable->setVerticalHeaderLabels(verticalLabels);
}

void DefinitionPredicateInfo::moveCurrentRow(RowMoveDirection direction)
{
    QTableWidgetItem * current = ui.argumentsTable->currentItem();
    int targetIndex;
    switch (direction)
    {
    case MoveUp:
        if ( ((targetIndex = current->row() - 1) >= 0) &&
             (current->column() == CLASS_COLUMN) )
        {
            swapConnections(current->row(), targetIndex);
            swapTableItems(current, ui.argumentsTable->item(targetIndex, CLASS_COLUMN));
        }
        break;
    case MoveDown:
        if ( ((targetIndex = current->row() + 1) < ui.argumentsTable->rowCount()) &&
             (current->column() == CLASS_COLUMN) )
        {
            swapConnections(current->row(), targetIndex);
            swapTableItems(current, ui.argumentsTable->item(targetIndex, CLASS_COLUMN));
        }
        break;
    }
}

void DefinitionPredicateInfo::swapTableItems(QTableWidgetItem *first, QTableWidgetItem *second)
{
    QString temp = first->text();
    first->setText(second->text());
    second->setText(temp);
    ui.argumentsTable->setCurrentItem(second);
}

void DefinitionPredicateInfo::swapConnections(int firstArg, int secondArg)
{
    QDomElement connections = selectedNode.firstChildElement("connections");
    Q_ASSERT(!connections.isNull());

    QString firstArgNum;
    QString secondArgNum;

    firstArgNum.setNum(firstArg + 1, 10);
    secondArgNum.setNum(secondArg + 1, 10);

    QDomElement firstConn = findSubelementAttr(connections, "starts", "argn", firstArgNum);
    QDomElement secondConn = findSubelementAttr(connections, "starts", "argn", secondArgNum);

    int firstValue = getIntValue(firstConn);
    int secondValue = getIntValue(secondConn);

    setIntValue(firstConn, secondValue);
    setIntValue(secondConn, firstValue);
}



KAVI_NS_END
