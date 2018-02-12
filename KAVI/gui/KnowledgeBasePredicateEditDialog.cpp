#include "KnowledgeBasePredicateEditDialog.h"

KnowledgeBasePredicateEditDialog::KnowledgeBasePredicateEditDialog(QString content, QWidget *parent) :
    QDialog(parent), ui(new Ui::KnowledgeBasePredicateEditDialog)
{
    ui->setupUi(this);
    ui->predicateSign->setText(content);
}

KnowledgeBasePredicateEditDialog::~KnowledgeBasePredicateEditDialog()
{
    delete ui;
}

QString KnowledgeBasePredicateEditDialog::predicateSign() const
{
    return ui->predicateSign->text();
}
