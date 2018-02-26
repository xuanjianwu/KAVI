/*
 * @author: liYc
 * @date  : 2018/2/25
 * @brief : the dialog for view and edit the KAVI solution settings
*/

#ifndef SOLUTIONSETTINGSDIALOG_H
#define SOLUTIONSETTINGSDIALOG_H

#include "KAVIBase.h"
#include "XMLUtils.h"
#include "Convertor.h"

namespace Ui {
class SolutionSettingsDialog;
}

class SolutionSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /*
    * construct function, init the settings from the xml file
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    explicit SolutionSettingsDialog(QWidget *parent = 0);

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~SolutionSettingsDialog();

private slots:
    /*
    * handle the current index changed of the planner combobox and set the associated settings
    * @params:
    *       index - the current index
    * @return: void
    */
    void on_PlannerSelected_currentIndexChanged(int index);

    /*
    * handle the save button clicked and save changes of all settings to xmlData
    * @params: N/A
    * @return: void
    */
    void on_saveButton_clicked();

    /*
    * handle the OK button clicked and save changes of all settings to xmlData, write xmlData to xml file
    * @params: N/A
    * @return: void
    */
    void on_buttonBox_accepted();

private:
    /*
    * read xml file to xmlData
    * @params: N/A
    * @return: bool
    */
    bool getXMLDocument();

    /*
    * get the xml file's path
    * @params: N/A
    * @return: the specified file path
    */
    QString getXMLFilePath();

    /*
    * write xmlData to xml file
    * @params: N/A
    * @return: bool
    */
    bool writeToXMLFile();

    /*
    * set all the settings in solution settings dialog
    * @params: N/A
    * @return: void
    */
    void setAllSettings();

    /*
    * save all the settings in solution settings dialog to xmlData
    * @params: N/A
    * @return: void
    */
    void saveAllSettings();

    /*
    * set the experiment settings
    * @params: N/A
    * @return: void
    */
    void setExperimentSettings();

    /*
    * save the experiment settings to xmlData
    * @params: N/A
    * @return: void
    */
    void saveExperimentSettings();

    /*
    * set the planners settings
    * @params: N/A
    * @return: void
    */
    void setPlannersSettings();

    /*
    * save the planners settings to xmlData
    * @params: N/A
    * @return: void
    */
    void savePlannerSettings();

    /*
    * init the planner parameters table: headers, format...
    * @params: N/A
    * @return: void
    */
    void initPlannerParametersTable();

    /*
    * fill the planner parameters table with contents of the selected planner
    * @params: N/A
    * @return: void
    */
    void fillPlannerParametersTable();

    /*
    * save the planner parameters to xmlData
    * @params: N/A
    * @return: void
    */
    void savePlannerParameters();

private:
    Ui::SolutionSettingsDialog *ui;
    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;
    // the xml document for KAVI solution settings xml file
    QDomDocument xmlData;
    // the root element of the xmlData
    QDomElement rootElement;
    // the current selected planner element
    QDomElement selectdPlanner;
};

#endif // SOLUTIONSETTINGSDIALOG_H
