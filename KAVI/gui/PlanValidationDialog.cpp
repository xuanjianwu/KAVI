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
    ui->actionsTable->setColumnCount(this->planValidator->getPlanSize());
    QStringList tableHeadersList;
    for (int i = 0; i < ui->actionsTable->columnCount(); i++)
    {
        tableHeadersList << QString::number(i);
    }
    ui->actionsTable->setHorizontalHeaderLabels(tableHeadersList);

    ui->actionsTable->setRowCount(2);
    tableHeadersList.clear();
    tableHeadersList << "Actions:" << "State:";
    ui->actionsTable->setVerticalHeaderLabels(tableHeadersList);


    Plan* plan = this->planValidator->getPlan();

    QMap<int, PlanAction> actions = plan->getActions();

    int interruptActionIndex = plan->getInterruptActionId();

    bool interrupted = false;

    foreach (PlanAction action, actions.values()) {
        QString formula = action.getFormula();
        int actionId = action.getId();
        QTableWidgetItem *actionItem = new QTableWidgetItem(formula, QTableWidgetItem::Type);
        actionItem->setFlags(actionItem->flags() & (~Qt::ItemIsEditable));
        if (actionId == interruptActionIndex)
        {
            interrupted = true;
            actionItem->setBackground(QBrush(QColor("#FF4500")));
        }
        if (interrupted)
        {
            actionItem->setBackground(QBrush(QColor("#D9D9D9")));
        }
        else {
            actionItem->setBackground(QBrush(QColor("#76EE00")));
        }
        ui->actionsTable->setItem(0, actionId, actionItem);
    }
}
