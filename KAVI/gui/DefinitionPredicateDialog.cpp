#include "DefinitionPredicateDialog.h"
#include "ui_DefinitionPredicateDialog.h"

DefinitionPredicateDialog::DefinitionPredicateDialog(KAVIPredicateKB *predicateKB, QWidget *parent) :
    QDialog(parent), ui(new Ui::DefinitionPredicateDialog)
{
    ui->setupUi(this);

    this->predicateKB = predicateKB;
    QCompleter *completer = new QCompleter(this);
    completer->setFilterMode(Qt::MatchStartsWith);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    listModel = new QStringListModel(this->predicateKB->getData(), this);
    completer->setModel(listModel);

    ui->predicateNameEdit->setCompleter(completer);
    ui->predicateNameEdit->setFocus();

    //connect(ui->predicateNameEdit, SIGNAL(editingFinished()), this, SLOT(editComplete()));
}

DefinitionPredicateDialog::~DefinitionPredicateDialog()
{
    //delete predicateKB;
    //delete listModel;
    delete ui;
}

QString DefinitionPredicateDialog::predicateSign() const
{
    return ui->predicateNameEdit->text().simplified();
}

void DefinitionPredicateDialog::editComplete()
{
    // not solve: when press enter key, this function will be triggered twice
    QString text = ui->predicateNameEdit->text();
    if (QString::compare(text, QString("")) != 0)
    {
        if (this->predicateKB->addPredicate(text))
        {
            listModel->setStringList(this->predicateKB->getData());
        }
    }
}
