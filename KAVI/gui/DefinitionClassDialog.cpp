#include "DefinitionClassDialog.h"
#include "ui_DefinitionClassDialog.h"

DefinitionClassDialog::DefinitionClassDialog(KAVIClassKB *classKB, QWidget *parent)
    :QDialog(parent), ui(new Ui::DefinitionClassDialog)
{
    ui->setupUi(this);

    this->classKB = classKB;
    QCompleter *completer = new QCompleter(this);
    completer->setFilterMode(Qt::MatchStartsWith);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    listModel = new QStringListModel(this->classKB->getData(), this);
    completer->setModel(listModel);

    ui->classNameEdit->setCompleter(completer);
    ui->classNameEdit->setFocus();

    connect(ui->classNameEdit, SIGNAL(editingFinished()), this, SLOT(editComplete()));
}

DefinitionClassDialog::~DefinitionClassDialog()
{
    //delete classKB;
    //delete listModel;
    delete ui;
}

QString DefinitionClassDialog::className() const
{
    return ui->classNameEdit->text();
}

void DefinitionClassDialog::editComplete()
{
    // not solve: when press enter key, this function will be triggered twice
    QString text = ui->classNameEdit->text();
    if (QString::compare(text, QString("")) != 0)
    {
        if (this->classKB->addClass(text))
        {
            listModel->setStringList(this->classKB->getData());
        }
    }
}
