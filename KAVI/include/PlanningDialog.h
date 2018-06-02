/*
 * @author: liYc
 * @date  : 2018/2/27
 * @brief : the dialog for the planning
*/

#ifndef PLANNINGDIALOG_H
#define PLANNINGDIALOG_H

#include "KAVIBase.h"
#include "PlannerSuggestion.h"
#include "ExecPlanner.h"
#include "PlanAnalyzer.h"
#include "SolutionSettingsDialog.h"
#include "PlanValidator.h"
#include "XMLUtils.h"
#include "PlanValidationDialog.h"
#include "EditFileDialog.h"

namespace Ui {
class PlanningDialog;
}

class PlanningDialog : public QDialog
{
    Q_OBJECT

public:
    /*
     * construct function
     * @params:
     *      parent - parent object
     * @return: N/A
    */
    explicit PlanningDialog(QWidget *parent = 0);
    ~PlanningDialog();

    /*
     * solve problem with single planner
     * @params:
     *      domain        - domain PDDL file
     *      problem       - problem PDDL file
     *      chosenPlanner - the chosen planner to solve problem
     * @return: N/A
    */
    void solveProblemWithSinglePlanner(QString domain, QString problem, QDomElement chosenPlanner);

    // get all available planners
    QDomElement getKAVIPlanners();

    void loadKAVIPlanners();

    QDomElement getKAVIValidators();

    void loadKAVIValidators();

    void initChosenValidator();

    /*
     * init PlanningDialog's environment
     * @params: N/A
     * @return: N/A
    */
    void initEnvironment();

    void initProblemSelection();

    /*
     * init the planner selection
     * @params:
     *      plannersList - the list of available planners
     * @return: N/A
    */
    void initPlannerSelection(QList<QDomElement> plannersList);

signals:
    void exportDefaultPDDL();

    /*
     * signal sended to KAVIMainWindow to create new action, handle the repair option
     * @params:
     *      action - the new added action
     *      index  - the flawed action's index
     * @return: N/A
    */
    void createNewAction(PlanAction action, QString index);

    /*
     * signal sended to KAVIMainWindow to modify old actions, handle the repair option
     * @params: N/A
     * @return: N/A
    */
    void modifyOldActions();

public slots:
    /*
     * append log from ExecPlanner to console
     * @params:
     *      text - the specified text log
     * @return: N/A
    */
    void appendToConsoleLog(QString text);

    /*
     * slot called by PlanValidationDialog to create new action
     * @params:
     *      flawAction - the flawed action
     *      index      - the flawed action's index
     * @return: N/A
    */
    void execRepairByCreateAction(PlanAction flawAction, QString index);

    /*
     * slot called by PlanValidationDialog to modify old actions
     * @params: N/A
     * @return: N/A
    */
    void execRepairByModifyActions();

private slots:
    void on_customProblem_clicked(bool checked);

    void on_domainBrowse_clicked();

    void on_problemBrowse_clicked();

    /*
     * slot to show plan report
     * @params: N/A
     * @return: N/A
    */
    void showPlannerOutput();

    void on_plannersSettings_clicked();

    void on_plannerSelection_currentIndexChanged(int index);

    void on_execPlanner_clicked();

    void on_planBrowse_clicked();

    void on_planValidation_clicked(bool checked);

    void on_execValidator_clicked();

    void on_domainPDDLFile_textChanged(const QString &arg1);

    void on_problemPDDLFile_textChanged(const QString &arg1);

    void on_planFile_textChanged(const QString &arg1);

    void on_editDomainPDDL_clicked();

    void on_editProblemPDDL_clicked();

    void on_editPlanFile_clicked();

    void on_clearConsoleLog_clicked();

private:
    QString getDomainName();
    QString getProblemName();

    void setDefaultDomainFile();
    void setDefaultProblemFile();
    void setDefaultPlanFile();

    void setDomainFile(QString domainFile);
    void setProblemFile(QString problemFile);
    void setPlanFile(QString planFile);

    void resetDomainFile();
    void resetProblemFile();
    void resetPlanFile();
    void resetPlannerSelection();

    bool getXMLDocument();
    bool getValidatorsXMLDocument();

    QString getPDDLFilePath();
    QString getXMLFilePath();
    QString getValidatorsXMLFilePath();

    Ui::PlanningDialog *ui;

    ExecPlanner* exe;
    QThread* plannerThread;

    // domain PDDL file
    QString domainFile;

    // problem PDDL file
    QString problemFile;

    // plan txt file
    QString planFile;

    // plans in xml format
    QDomElement plans;
    QDomElement solveResult;

    // the chosen planner's infomationm in xml format
    QDomElement theSingleChosenPlanner;

    // list of available planners
    QList<QDomElement> plannersList;

    // the chosen validator's infomation in xml format
    QDomElement theSingleChosenValidator;

    PlannerSuggestion* plannerSuggestion;

    bool forceFinish = false;
    bool stopRunningPlanners = false;


    QDomDocument KAVIPlannersDocument;
    QDomElement KAVIPlanners;

    QDomDocument KAVIValidatorsDocument;
    QDomElement KAVIValidators;

    PlanValidator* planValidator;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;
};

#endif // PLANNINGDIALOG_H
