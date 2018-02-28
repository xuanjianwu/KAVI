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

    static QDomElement getKAVIPlanners();

    void loadKAVIPlanners();

private slots:
    void on_customProblem_clicked(bool checked);

    void on_clearOutput_clicked();

    void on_domainBrowse_clicked();

    void on_problemBrowse_clicked();

    void showPlannerOutput();

private:
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

    static QDomElement KAVIPlanners;

    SolutionSettingsDialog* solutionSettingsDialog;
};

#endif // PLANNINGDIALOG_H
