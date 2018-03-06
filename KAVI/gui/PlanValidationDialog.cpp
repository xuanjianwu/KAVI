#include "PlanValidationDialog.h"
#include "ui_PlanValidationDialog.h"

PlanValidationDialog::PlanValidationDialog(PlanValidator* planValidator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanValidationDialog)
{
    ui->setupUi(this);

    this->planValidator = planValidator;
    fillActionsTable();
}

PlanValidationDialog::~PlanValidationDialog()
{
    delete ui;
}

void PlanValidationDialog::fillActionsTable()
{
    ui->actionsTable->setColumnCount(2);
    QStringList tableHeadersList;
    tableHeadersList << "Time" << "Action";
    ui->actionsTable->setHorizontalHeaderLabels(tableHeadersList);

//    tableHeadersList.clear();
//    ui->actionsTable->setRowCount(this->planValidator->getPlanSize());
//    for (int i = 0; i < ui->actionsTable->columnCount(); i++)
//    {
//        tableHeadersList << QString::number(i);
//    }
//    ui->actionsTable->setVerticalHeaderLabels(tableHeadersList);

    ui->actionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->actionsTable->setRowCount(this->planValidator->getPlan()->getPlanSize());
    ui->actionsTable->verticalHeader()->setHidden(true);

    Plan* plan = this->planValidator->getPlan();

    QMap<int, PlanAction> actions = plan->getActions();

    int interruptActionIndex = plan->getInterruptActionId();

    bool interrupted = false;

    foreach (PlanAction action, actions.values()) {
        QString formula = action.getFormula();
        int actionId = action.getId();
        QTableWidgetItem *timeItem;

        QTableWidgetItem *actionItem = new QTableWidgetItem(formula, QTableWidgetItem::Type);
        actionItem->setFlags(actionItem->flags() & (~Qt::ItemIsEditable));
        actionItem->setToolTip(formula);

        //QTableWidgetItem *stateItem;

        if (actionId == 0)
        {
            actionItem->setBackground(QBrush(QColor(Qt::white)));
            timeItem = new QTableWidgetItem("", QTableWidgetItem::Type);
        }
        else if (actionId == actions.size()-1)
        {
            actionItem->setBackground(QBrush(QColor(Qt::white)));
            timeItem = new QTableWidgetItem("", QTableWidgetItem::Type);
        }
        else if (actionId == interruptActionIndex)
        {
            interrupted = true;
            actionItem->setBackground(QBrush(QColor("#FF4500")));
            timeItem = new QTableWidgetItem(QString::number(action.getTime()), QTableWidgetItem::Type);
            //stateItem = new QTableWidgetItem("Non-applicable", QTableWidgetItem::Type);
            //stateItem->setFlags(stateItem->flags() & (~Qt::ItemIsEditable));
            //ui->actionsTable->setItem(1, actionId, stateItem);
        }
        else if (interrupted)
        {
            actionItem->setBackground(QBrush(QColor("#D9D9D9")));
            timeItem = new QTableWidgetItem(QString::number(action.getTime()), QTableWidgetItem::Type);
        }
        else {
            actionItem->setBackground(QBrush(QColor("#76EE00")));
            timeItem = new QTableWidgetItem(QString::number(action.getTime()), QTableWidgetItem::Type);
            //stateItem = new QTableWidgetItem("Non-applicable", QTableWidgetItem::Type);
            //stateItem->setFlags(stateItem->flags() & (~Qt::ItemIsEditable));
            //ui->actionsTable->setItem(1, actionId, stateItem);
        }
        timeItem->setFlags(timeItem->flags() & (~Qt::ItemIsEditable));
        ui->actionsTable->setItem(actionId, 0, timeItem);
        ui->actionsTable->setItem(actionId, 1, actionItem);
    }
}

void PlanValidationDialog::on_actionsTable_cellClicked(int row, int column)
{
    if (row == this->planValidator->getPlan()->getInterruptActionId())
    {
        ui->repair->setEnabled(true);
        ui->advice->setEnabled(true);
        ui->advice->clear();

        Plan* plan = this->planValidator->getPlan();
        QMap<int, PlanAction> actions = plan->getActions();
        PlanAction interruptedAction = actions.value(row);

        QMap<QString, bool> repairAdvice = interruptedAction.getRepairAdvice();
        QMap<QString, bool>::iterator iter;
        for (iter = repairAdvice.begin(); iter != repairAdvice.end(); iter++) {
            QString adviceStr;
            adviceStr.append("Set ");
            adviceStr.append(iter.key());
            adviceStr.append(" to ");
            adviceStr.append(iter.value() == true ? "true" : "false");
            ui->advice->append(adviceStr);
        }
    }
    else
    {
        ui->repair->setEnabled(false);
        ui->advice->setEnabled(false);
        ui->advice->clear();
    }
}

void PlanValidationDialog::on_okButton_clicked()
{
    this->accept();
}

void PlanValidationDialog::on_cancelButton_clicked()
{
    this->reject();
}
