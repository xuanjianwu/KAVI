#include "OperatorsPredicateDialog.h"
#include "NodeStructure.h"

KAVI_NS_BEGIN



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

    if ( ui.precondCheck->isChecked() )
        result << NSPS_PRECOND;

    if ( ui.effectPosCheck->isChecked() )
        result << NSPS_EFFECT_POS;

    if ( ui.effectNegCheck->isChecked() )
        result << NSPS_EFFECT_NEG;

    return result;
}

void OperatorsPredicateDialog::on_precondCheck_toggled(bool checked)
{
    ui.effectPosCheck->setEnabled( !checked && !ui.effectNegCheck->isChecked() );
}

void OperatorsPredicateDialog::on_effectPosCheck_toggled(bool checked)
{
    ui.effectNegCheck->setEnabled(!checked);
    ui.precondCheck->setEnabled(!checked);
}

void OperatorsPredicateDialog::on_effectNegCheck_toggled(bool checked)
{
    ui.effectPosCheck->setEnabled( !checked && !ui.precondCheck->isChecked() );
}


KAVI_NS_END
