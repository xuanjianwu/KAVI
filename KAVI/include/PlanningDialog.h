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
#include "XMLUtils.h"

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

    void initEnvironment();

    void initProblemSelection();

    void initPlannerSelection(QList<QDomElement> plannersList);

signals:
    void exportDefaultPDDL();

private slots:
    void on_customProblem_clicked(bool checked);

    void on_clearOutput_clicked();

    void on_domainBrowse_clicked();

    void on_problemBrowse_clicked();

    void showPlannerOutput();

    void on_plannersSettings_clicked();

    void on_plannerSelection_currentIndexChanged(int index);

    void on_execPlanner_clicked();

private:
    void setDefaultDomainFile();
    void setDefaultProblemFile();

    void setDomainFile(QString domainFile);
    void setProblemFile(QString problemFile);

    void resetDomainFile();
    void resetProblemFile();
    void resetPlannerSelection();

    bool getXMLDocument();

    QString getPDDLFilePath();
    QString getXMLFilePath();

    Ui::PlanningDialog *ui;

    ExecPlanner* exe;
    QThread* plannerThread;

    QString domainFile;
    QString problemFile;

    QDomElement plans;
    QDomElement solveResult;

    QDomElement theSingleChosenPlanner;
    QList<QDomElement> plannersList;

    PlannerSuggestion* plannerSuggestion;

    bool forceFinish = false;
    bool stopRunningPlanners = false;


    QDomDocument KAVIPlannersDocument;
    QDomElement KAVIPlanners;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;
};

#endif // PLANNINGDIALOG_H
