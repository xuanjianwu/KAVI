#include "PlanValidationDialog.h"
#include "ui_PlanValidationDialog.h"

PlanValidationDialog::PlanValidationDialog(PlanValidator* planValidator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanValidationDialog)
{
    ui->setupUi(this);

    this->planValidator = planValidator;
    fillActionsTable();

    options.insert(0, "Directly create a new action");
    options.insert(1, "Modify current actions");

    connect(ui->advice, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_repair_clicked()));
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

    ui->actionsTable->horizontalHeader()->setStretchLastSection(true);
    ui->actionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->actionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->actionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);


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

void PlanValidationDialog::fillActionAdvice(int actionId)
{
    Plan* plan = this->planValidator->getPlan();
    QMap<int, PlanAction> actions = plan->getActions();
    PlanAction interruptedAction = actions.value(actionId);

    QMap<QString, bool> repairAdvice = interruptedAction.getRepairAdvice();
    QMap<QString, bool>::iterator iter;
    for (iter = repairAdvice.begin(); iter != repairAdvice.end(); iter++) {
        flawStates.append(iter.key());
        flawAdvice.append(iter.value());
    }

    for (int i = 0; i < flawStates.size(); i++)
    {
        QString adviceStr;
        adviceStr.append("Set ");
        adviceStr.append(flawStates.at(i));
        adviceStr.append(" to ");
        adviceStr.append(flawAdvice.at(i) == true ? "true" : "false");
        ui->advice->addItem(adviceStr);
    }
}

void PlanValidationDialog::fillActionInfomation(int actionId)
{
    Plan* plan = this->planValidator->getPlan();
    QMap<int, PlanAction> actions = plan->getActions();
    PlanAction action = actions.value(actionId);

    QString htmlStr;
    htmlStr += "<html>\n";
    htmlStr += "<body>\n";
    htmlStr += "<h2><b>";
    htmlStr += action.getFormula();
    htmlStr += "</b></h2>\n";

    htmlStr += "<p>Time: ";
    if (actionId == 0 || actionId == actions.size()-1)
    {
        htmlStr += "N/A";
    }
    else
    {
        htmlStr += QString::number(action.getTime());
    }

    htmlStr += "</p>\n";


    // show preconditions
    htmlStr += "<h3><u>Preconditions: </u></h3>\n";

    htmlStr += "<ul>\n";
    // show positive preconditions
    foreach (QString str, action.getPositivePreconditions()) {
        if (!action.getRepairAdvice().keys().contains(str))
        {
            htmlStr += "<li style=\"color:#32CD32;\">";
            htmlStr += str;
            htmlStr += "</li>\n";

            QMap<int, QSet<QString> > dependers = action.getPositivePreconditionsDependers();
            QMap<int, QSet<QString> >::iterator iter;

            htmlStr += "<ul type=\"circle\">\n";
            for (iter = dependers.begin(); iter != dependers.end(); iter++)
            {
                if (iter.value().contains(str)){

                    htmlStr += "<li>Positive effect of ";
                    htmlStr += "<u>";
                    htmlStr += actions.value(iter.key()).getFormula();
                    htmlStr += "</u>";
                    htmlStr += " at time: ";

                    if (iter.key() == 0 || iter.key() == actions.size()-1)
                    {
                        htmlStr += "N/A";
                    }
                    else
                    {
                        htmlStr += QString::number(actions.value(iter.key()).getTime());
                    }

                    htmlStr += "</li>\n";
                }
            }
            htmlStr += "</ul>\n";
        }
        else
        {
            htmlStr += "<li style=\"color:#FF4500;\">";
            htmlStr += str;
            htmlStr += " is not satisfied";
            htmlStr += "</li>\n";
        }
    }

    // show negative preconditions
    foreach (QString str, action.getNegativePreconditions()) {
        if (!action.getRepairAdvice().keys().contains(str))
        {
            htmlStr += "<li style=\"color:#32CD32;\">";
            htmlStr += "(not ";
            htmlStr += str;
            htmlStr += " )";
            htmlStr += "</li>\n";

            QMap<int, QSet<QString> > dependers = action.getNegativePreconditionsDependers();
            QMap<int, QSet<QString> >::iterator iter;

            htmlStr += "<ul type=\"circle\">\n";
            for (iter = dependers.begin(); iter != dependers.end(); iter++)
            {
                if (iter.value().contains(str)){

                    htmlStr += "<li>Negative effect of ";
                    htmlStr += "<u>";
                    htmlStr += actions.value(iter.key()).getFormula();
                    htmlStr += "</u>";
                    htmlStr += " at time: ";

                    if (iter.key() == 0 || iter.key() == actions.size()-1)
                    {
                        htmlStr += "N/A";
                    }
                    else
                    {
                        htmlStr += QString::number(actions.value(iter.key()).getTime());
                    }

                    htmlStr += "</li>\n";
                }
            }
            htmlStr += "</ul>\n";
        }
        else
        {
            htmlStr += "<li style=\"color:#FF4500;\">";
            htmlStr += "(not ";
            htmlStr += str;
            htmlStr += " )";
            htmlStr += " is not satisfied";
            htmlStr += "</li>\n";
        }
    }
    htmlStr += "</ul>\n";


    // show effects
    htmlStr += "<h3><u>Effects: </u></h3>\n";

    htmlStr += "<ul>\n";
    // show positive effects
    foreach (QString str, action.getPositiveEffects()) {
        if ( actionId < plan->getInterruptActionId() || this->planValidator->getPlan()->getInterruptActionId() < 0)
        {
            htmlStr += "<li>";
            htmlStr += str;
            htmlStr += "</li>\n";

            QMap<int, QSet<QString> > dependers = action.getPositiveEffectsDependers();
            QMap<int, QSet<QString> >::iterator iter;

            htmlStr += "<ul type=\"circle\">\n";
            for (iter = dependers.begin(); iter != dependers.end(); iter++)
            {
                if (plan->getInterruptActionId() > 0 && iter.key() > plan->getInterruptActionId())
                {
                    continue;
                }
                if (iter.value().contains(str)){

                    htmlStr += "<li>Positive precondition for ";
                    htmlStr += "<u>";
                    htmlStr += actions.value(iter.key()).getFormula();
                    htmlStr += "</u>";
                    htmlStr += " at time: ";

                    if (iter.key() == 0 || iter.key() == actions.size()-1)
                    {
                        htmlStr += "N/A";
                    }
                    else
                    {
                        htmlStr += QString::number(actions.value(iter.key()).getTime());
                    }

                    htmlStr += "</li>\n";
                }
            }
            htmlStr += "</ul>\n";
        }
        else
        {
            htmlStr += "<li>";
            htmlStr += str;
            htmlStr += "</li>\n";
        }
    }

    // show negative effects
    foreach (QString str, action.getNegativeEffects()) {
        if ( actionId < plan->getInterruptActionId() || this->planValidator->getPlan()->getInterruptActionId() < 0)
        {
            htmlStr += "<li>";
            htmlStr += "(not ";
            htmlStr += str;
            htmlStr += " )";
            htmlStr += "</li>\n";

            QMap<int, QSet<QString> > dependers = action.getNegativeEffectsDependers();
            QMap<int, QSet<QString> >::iterator iter;

            htmlStr += "<ul type=\"circle\">\n";
            for (iter = dependers.begin(); iter != dependers.end(); iter++)
            {
                if (plan->getInterruptActionId() > 0 && iter.key() > plan->getInterruptActionId())
                {
                    continue;
                }
                if (iter.value().contains(str)){

                    htmlStr += "<li>Negative precondition for ";
                    htmlStr += "<u>";
                    htmlStr += actions.value(iter.key()).getFormula();
                    htmlStr += "</u>";
                    htmlStr += " at time: ";

                    if (iter.key() == 0 || iter.key() == actions.size()-1)
                    {
                        htmlStr += "N/A";
                    }
                    else
                    {
                        htmlStr += QString::number(actions.value(iter.key()).getTime());
                    }

                    htmlStr += "</li>\n";
                }
            }
            htmlStr += "</ul>\n";
        }
        else
        {
            htmlStr += "<li>";
            htmlStr += "(not ";
            htmlStr += str;
            htmlStr += " )";
            htmlStr += "</li>\n";
        }
    }
    htmlStr += "</ul>\n";

    htmlStr += "</body>\n";
    htmlStr += "</html>";


    ui->actionInfomation->setHtml(htmlStr);
}

void PlanValidationDialog::fillWorldStateChange(int actionId)
{
    Plan* plan = this->planValidator->getPlan();
    QMap<int, PlanAction> actions = plan->getActions();
    PlanAction action = actions.value(actionId);

    StateHistory* stateHistory = this->planValidator->getStateHistory();
    QList<State> statesList = stateHistory->getStatesList();
    State state = statesList.at(actionId);

    QString htmlStr;
    htmlStr += "<html>\n";
    htmlStr += "<body>\n";
    htmlStr += "<h2><b>";
    htmlStr += action.getFormula();
    htmlStr += "</b></h2>\n";

    // show facts before action
    htmlStr += "<h3><u>State before action: </u></h3>\n";

    htmlStr += "<ul>\n";
    foreach (QString fact, state.getBeforeActionFacts()) {
        if (!state.getExcludeFacts().contains(fact))
        {
            htmlStr += "<li>";
            htmlStr += fact;
            htmlStr += "</li>\n";
        }
        else
        {
            htmlStr += "<li style=\"color:#FF4500;\">";
            htmlStr += "<s>";
            htmlStr += fact;
            htmlStr += "</s>";
            htmlStr += "</li>\n";
        }
    }
    htmlStr += "</ul>\n";

    // show facts after action
    htmlStr += "<h3><u>State after action: </u></h3>\n";

    htmlStr += "<ul>\n";
    foreach (QString fact, state.getAfterActionFacts()) {
        if (!state.getIncludeFacts().contains(fact))
        {
            htmlStr += "<li>";
            htmlStr += fact;
            htmlStr += "</li>\n";
        }
        else
        {
            htmlStr += "<li style=\"color:#4169E1;\">";
            //htmlStr += "<del>";
            htmlStr += fact;
            //htmlStr += "</del>";
            htmlStr += "</li>\n";
        }
    }
    if (actionId == plan->getInterruptActionId())
    {
        htmlStr += "<li>";
        htmlStr += "N/A";
        htmlStr += "</li>\n";
    }

    htmlStr += "</ul>\n";

    htmlStr += "</body>\n";
    htmlStr += "</html>";

    ui->worldStateChange->setHtml(htmlStr);
}

void PlanValidationDialog::runRepair(int selectedOption)
{

}

void PlanValidationDialog::on_actionsTable_cellClicked(int row, int column)
{
    ui->actionInfomation->clear();
    ui->worldStateChange->clear();
    ui->advice->clear();
    flawAdvice.clear();
    flawStates.clear();

    if (row <= this->planValidator->getPlan()->getInterruptActionId() || this->planValidator->getPlan()->getInterruptActionId() < 0)
    {
        ui->actionInfomation->setEnabled(true);
        ui->worldStateChange->setEnabled(true);
        fillActionInfomation(row);
        fillWorldStateChange(row);
    }
    else
    {
        ui->actionInfomation->setEnabled(false);
        ui->worldStateChange->setEnabled(false);
    }

    if (row == this->planValidator->getPlan()->getInterruptActionId())
    {
        //ui->repair->setEnabled(true);
        ui->advice->setEnabled(true);
        fillActionAdvice(row);
    }
    else
    {
        //ui->repair->setEnabled(false);
        ui->advice->setEnabled(false);
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


void PlanValidationDialog::on_repair_clicked()
{
    if (ui->advice->currentRow() >= 0)
    {
        RepairDialog* dialog = new RepairDialog(QStringList(options.values()), this);
        if (dialog->exec() == QDialog::Accepted)
        {
            int selectedOption = dialog->getOption();

            // Directly create a new action
            if (selectedOption == options.keys().at(0))
            {
                Plan* plan = this->planValidator->getPlan();
                QMap<int, PlanAction> actions = plan->getActions();
                PlanAction interruptedAction = actions.value(plan->getInterruptActionId());
                QString flawFact = flawStates.at(ui->advice->currentRow());
                this->accept();
                emit createNewAction(interruptedAction, flawFact);
            }
            // Modify current actions
            else if (selectedOption = options.keys().at(1))
            {
                this->accept();
                emit modifyOldActions();
            }
        }
    }
}

void PlanValidationDialog::on_advice_currentRowChanged(int currentRow)
{
    if (currentRow < 0)
    {
        ui->repair->setEnabled(false);
    }
    else
    {
        ui->repair->setEnabled(true);
    }
}
