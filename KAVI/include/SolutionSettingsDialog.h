/*
 * @author: liYc
 * @date  : 2018/2/25
 * @brief : the dialog for the KAVI solution settings
*/

#ifndef SOLUTIONSETTINGSDIALOG_H
#define SOLUTIONSETTINGSDIALOG_H

#include "KAVIBase.h"
#include "XMLUtils.h"
#include "CheckUtils.h"
#include "Convertor.h"

namespace Ui {
class SolutionSettingsDialog;
}

class SolutionSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolutionSettingsDialog(QWidget *parent = 0);
    ~SolutionSettingsDialog();

private slots:
    void on_PlannerSelected_currentIndexChanged(int index);

    void on_saveButton_clicked();

private:
    bool getXMLDocument();
    QString getXMLFilePath();

    void setAllSettings();
    void saveAllSettings();

    void setExperimentSettings();
    void saveExperimentSettings();

    void setPlannersSettings();
    void savePlannerSettings();

    void initPlannerParametersTable();
    void fillPlannerParametersTable(QDomElement planner);
    void savePlannerParameters(QDomElement planner);

private:
    Ui::SolutionSettingsDialog *ui;
    RunMode KAVIRunMode;
    QDomDocument xmlData;
    QDomElement rootElement;
    QDomElement selectdPlanner;
};

#endif // SOLUTIONSETTINGSDIALOG_H
