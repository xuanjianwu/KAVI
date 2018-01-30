/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the variable dialog in operator view
*/

#ifndef OPERATORSVARIABLEDIALOG_H
#define OPERATORSVARIABLEDIALOG_H

#include "KAVIBase.h"
#include "ui_OperatorsVariableDialog.h"

KAVI_NS_BEGIN

class OperatorsVariableDialog: public QDialog {
    Q_OBJECT

public:
    /*
    * construct function with the classes
    * @params:
    *       classes  - available classes used to add new variable
    *       parent   - parent object
    * @return: N/A
    */
    OperatorsVariableDialog(QStringList classes, QWidget* parent = 0);

    /*
    * get the selected class for the variable to add
    * @params: N/A
    * @return: the specified class's name
    */
    QString variableClass();

    /*
    * get the input name for the variable to add
    * @params: N/A
    * @return: the name for the new variable
    */
    QString variableName();

private:

    Ui::OperatorsVariableDialog ui;
}

KAVI_NS_END

#endif // OPERATORSVARIABLEDIALOG_H
