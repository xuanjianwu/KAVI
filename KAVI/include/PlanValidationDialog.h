/*
 * @author: liYc
 * @date  : 2018/3/6
 * @brief : the dialog for the plan validation
*/

#ifndef PLANVALIDATIONDIALOG_H
#define PLANVALIDATIONDIALOG_H

#include "KAVIBase.h"
#include "PlanningDialog.h"
#include "PlanValidator.h"
#include "RepairDialog.h"

namespace Ui {
class PlanValidationDialog;
}

class PlanValidationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanValidationDialog(PlanValidator* planValidator, QWidget *parent = 0);
    ~PlanValidationDialog();

    void fillActionsTable();

    void fillActionAdvice(int actionId);

    void fillActionInfomation(int actionId);

    void fillWorldStateChange(int actionId);

    void runRepair(int selectedOption);

signals:
    void createNewAction(PlanAction flawAction, QString flawFact);

private slots:
    void on_actionsTable_cellClicked(int row, int column);

    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_repair_clicked();

    void on_advice_currentRowChanged(int currentRow);

private:
    Ui::PlanValidationDialog *ui;

    QList<QString> flawStates;
    QList<bool> flawAdvice;

    QStringList optionsText;

    PlanValidator* planValidator;
};

#endif // PLANVALIDATIONDIALOG_H
