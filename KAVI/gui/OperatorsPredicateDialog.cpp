#include "OperatorsPredicateDialog.h"
#include "NodeStructure.h"





OperatorsPredicateDialog::OperatorsPredicateDialog(QStringList predicates, QWidget *parent)
    :QDialog(parent)
{
    ui.setupUi(this);
    ui.predComboBox->addItems(predicates);
    ui.predComboBox->setFocus();
}

QString OperatorsPredicateDialog::predicateName()
{
    return ui.predComboBox->currentText();
}

QStringList OperatorsPredicateDialog::predicateSet()
{
    QStringList result;

    if ( ui.precondPosCheck->isChecked() )
        result << NSPS_PRECOND_POS;

    if ( ui.precondNegCheck->isChecked() )
        result << NSPS_PRECOND_NEG;

    if ( ui.effectPosCheck->isChecked() )
        result << NSPS_EFFECT_POS;

    if ( ui.effectNegCheck->isChecked() )
        result << NSPS_EFFECT_NEG;

    return result;
}


void OperatorsPredicateDialog::on_effectPosCheck_toggled(bool checked)
{
    ui.effectNegCheck->setEnabled(!checked && !ui.precondNegCheck->isChecked() );
    ui.precondPosCheck->setEnabled(!checked && !ui.precondNegCheck->isChecked() );
    //ui.precondNegCheck->setEnabled(checked);
}

void OperatorsPredicateDialog::on_effectNegCheck_toggled(bool checked)
{
    ui.effectPosCheck->setEnabled( !checked && !ui.precondPosCheck->isChecked() );
    ui.precondNegCheck->setEnabled( !checked && !ui.precondPosCheck->isChecked() );
}


void OperatorsPredicateDialog::on_precondPosCheck_toggled(bool checked)
{
    ui.effectPosCheck->setEnabled( !checked && !ui.effectNegCheck->isChecked() );
    ui.precondNegCheck->setEnabled( !checked && !ui.effectNegCheck->isChecked() );
}

void OperatorsPredicateDialog::on_precondNegCheck_toggled(bool checked)
{
    ui.precondPosCheck->setEnabled(!checked && !ui.effectPosCheck->isChecked() );
    ui.effectNegCheck->setEnabled(!checked && !ui.effectPosCheck->isChecked() );
}
