#include "ProblemsPredicateDialog.h"
#include "NodeStructure.h"






ProblemsPredicateDialog::ProblemsPredicateDialog(QStringList predicates, bool radioInit, QWidget *parent)
    :QDialog(parent)
{
    ui.setupUi(this);
    ui.predComboBox->addItems(predicates);
    ui.initButton->setChecked(radioInit);
    ui.goalButton->setChecked(!radioInit);
    ui.predComboBox->setFocus();
}

QString ProblemsPredicateDialog::predicateName()
{
    return ui.predComboBox->currentText();
}

QString ProblemsPredicateDialog::taskState()
{
    if (ui.initButton->isChecked())
        return NSTS_INIT;
    else
        return NSTS_GOAL;
}




