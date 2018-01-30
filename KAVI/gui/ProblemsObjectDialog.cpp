#include "ProblemsObjectDialog.h"

KAVI_NS_BEGIN



ProblemsObjectDialog::ProblemsObjectDialog(QStringList classes, QWidget *parent)
    :QDialog(parent)
{
    ui.setupUi(this);
    ui.classComboBox->addItems(classes);
    ui.name->setFocus();
}

QString ProblemsObjectDialog::objectClass()
{
    return ui.classComboBox->currentText();
}

QString ProblemsObjectDialog::objectName()
{
    return ui.name->text();
}



KAVI_NS_END
