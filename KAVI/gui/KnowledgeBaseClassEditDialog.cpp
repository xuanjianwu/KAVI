#include "KnowledgeBaseClassEditDialog.h"

KnowledgeBaseClassEditDialog::KnowledgeBaseClassEditDialog(QString content, QWidget *parent) :
    QDialog(parent), ui(new Ui::KnowledgeBaseClassEditDialog)
{
    ui->setupUi(this);
    ui->className->setText(content);
    if (QString::compare(content, QString("")) == 0)
    {
        setWindowTitle("Add class");
    }
}

KnowledgeBaseClassEditDialog::~KnowledgeBaseClassEditDialog()
{
    delete ui;
}

QString KnowledgeBaseClassEditDialog::className() const
{
    return ui->className->text().simplified();
}
