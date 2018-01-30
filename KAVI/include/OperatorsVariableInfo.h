/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the infomation panel for variable in operator view
*/

#ifndef OPERATORSVARIABLEINFO_H
#define OPERATORSVARIABLEINFO_H

#include "KAVIBase.h"
#include "InfoPanel.h"
#include "ui_OperatorsVariableInfo.h"

KAVI_NS_BEGIN

class OperatorsVariableInfo: public InfoPanel {
    Q_OBJECT

public:
    /*
    * construct function, set the contents of InfoPanel
    * @params:
    *       data          - the dependent DataWidget
    *       definition    - the defintion DataWidget
    *       variableID    - the selected variable's ID
    *       definitionID  - the selected variable's associated class's ID in definition
    *       parent        - parent object
    * @return:
    */
    OperatorsVariableInfo(DataWidget* data, const DataWidget* definition, int variableID, int definitionID, QWidget* parent = 0);

private slots:

    /*
    * handle the current index changed of class combobox
    * madeChange(NodeClassChanged)
    * @params:
    *       newClass - the new class type for selected variable
    * @return: void
    */
    void on_classComboBox_currentIndexChanged(QString newClass);

private:
    // the dependent definition DataWidget
    const DataWidget* xmlDefinition;

    // the variable's associated class's element in the xmlDefinition
    QDomElement variableDef;

    /*
    * init the combobox for selected variable, and set selected variable as current index of
    * combobox
    * @params: N/A
    * @return: void
    */
    void initComboBox();

    Ui::OperatorsVariableInfo ui;
}

KAVI_NS_END

#endif // OPERATORSVARIABLEINFO_H
