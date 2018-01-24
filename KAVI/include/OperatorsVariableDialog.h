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
    OperatorsVariableDialog(QStringList classes, QWidget* parent = 0);

    QString variableClass();
    QString variableName();

private:

    Ui::OperatorsVariableDialog ui;
}

KAVI_NS_END

#endif // OPERATORSVARIABLEDIALOG_H
