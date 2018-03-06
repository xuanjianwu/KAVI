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

namespace Ui {
class PlanningDialog;
}

class PlanningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanningDialog(QWidget *parent = 0);
    ~PlanningDialog();

    void solveProblemWithSinglePlanner(QString domain, QString problem, QDomElement chosenPlanner);

    QDomElement getKAVIPlanners();

    void loadKAVIPlanners();

    QDomElement getKAVIValidators();

    void loadKAVIValidators();

    void initChosenValidator();

    void initEnvironment();

    void initProblemSelection();

    void initPlannerSelection(QList<QDomElement> plannersList);

signals:
    void exportDefaultPDDL();

private slots:
    void on_customProblem_clicked(bool checked);

    void on_domainBrowse_clicked();

    void on_problemBrowse_clicked();

    void showPlannerOutput();

    void on_plannersSettings_clicked();

    void on_plannerSelection_currentIndexChanged(int index);

    void on_execPlanner_clicked();

    void on_planBrowse_clicked();

    void on_planValidation_clicked(bool checked);

    void on_execValidator_clicked();

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

    QString domainFile;
    QString problemFile;
    QString planFile;

    QDomElement plans;
    QDomElement solveResult;

    QDomElement theSingleChosenPlanner;
    QList<QDomElement> plannersList;

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
