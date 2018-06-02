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
    /*
     * construct function
     * @params:
     *      planValidator - the associated plan validator instance
     *      parent        - parent object
     * @return: N/A
    */
    explicit PlanValidationDialog(PlanValidator* planValidator, QWidget *parent = 0);
    ~PlanValidationDialog();

    void fillActionsTable();

    void fillActionAdvice(int actionId);

    void fillActionInfomation(int actionId);

    void fillWorldStateChange(int actionId);

    /*
     * execute repair option
     * @params:
     *      selectedOption - the selected repair option
     * @return: N/A
    */
    void runRepair(int selectedOption);

signals:
    /*
     * signal sended to PlanningDialog, handling the create new action option
     * @params:
     *      flawAction - the flawed action
     *      flawFact   - the flawed fact
     * @return: N/A
    */
    void createNewAction(PlanAction flawAction, QString flawFact);

    /*
     * signal sended to PlanningDialog, handling the modify old actions option
     * @params: N/A
     * @return: N/A
    */
    void modifyOldActions();

private slots:
    void on_actionsTable_cellClicked(int row, int column);

    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_repair_clicked();

    void on_advice_currentRowChanged(int currentRow);

private:
    Ui::PlanValidationDialog *ui;

    // the flawed action's associated states
    QList<QString> flawStates;

    // the flawed action's associated advice
    QList<bool> flawAdvice;

    // // the flawed action's associated repair options
    QMap<int, QString> options;

    PlanValidator* planValidator;
};

#endif // PLANVALIDATIONDIALOG_H
