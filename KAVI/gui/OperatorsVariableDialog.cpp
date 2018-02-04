#include "OperatorsVariableDialog.h"





OperatorsVariableDialog::OperatorsVariableDialog(QStringList classes, QWidget *parent)
    :QDialog(parent)
{
    ui.setupUi(this);
    ui.classComboBox->addItems(classes);
    ui.name->setFocus();
}

QString OperatorsVariableDialog::variableClass()
{
    return ui.classComboBox->currentText();
}

QString OperatorsVariableDialog::variableName()
{
    return ui.name->text();
}




