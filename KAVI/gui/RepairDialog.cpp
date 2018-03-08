#include "RepairDialog.h"
#include "ui_RepairDialog.h"

RepairDialog::RepairDialog(QStringList options, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairDialog)
{
    ui->setupUi(this);
    ui->options->addItems(options);
}

RepairDialog::~RepairDialog()
{
    delete ui;
}

int RepairDialog::getOption()
{
    return ui->options->currentIndex();
}

void RepairDialog::on_ok_clicked()
{
    this->accept();
}

void RepairDialog::on_cancel_clicked()
{
    this->reject();
}
