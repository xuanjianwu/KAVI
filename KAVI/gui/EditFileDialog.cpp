#include "EditFileDialog.h"
#include "ui_EditFileDialog.h"

EditFileDialog::EditFileDialog(QString filePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditFileDialog)
{
    ui->setupUi(this);

    setFilePath(filePath);
    setTextChanged(false);
    initFileEditFromFilePath();

    nameChecker.setPattern(QString("^([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|[-_]){,%1}$").arg(30));
}

EditFileDialog::EditFileDialog(QString filePath, QString addText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditFileDialog)
{
     ui->setupUi(this);

     setFilePath(filePath);
     setTextChanged(false);
     initFileEditFromFilePath();

     nameChecker.setPattern(QString("^([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|[-_]){,%1}$").arg(30));

     createNewActionToFile(addText);
}

EditFileDialog::~EditFileDialog()
{
    delete ui;
}

void EditFileDialog::initFileEditFromFilePath()
{
    setWindowTitle(this->filePath);

    QFile file(this->filePath);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << file.fileName();
        return;
    }
    ui->fileEdit->clear();

    QString content;
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        content.append(str);
    }
    ui->fileEdit->setText(content);
    file.seek(0);
    file.close();
}

QString EditFileDialog::getFilePath() const
{
    return filePath;
}

void EditFileDialog::setFilePath(const QString &value)
{
    filePath = value;
}

void EditFileDialog::on_save_clicked()
{
    if (!getTextChanged())
    {
        return;
    }

    if (QFile::exists(this->filePath))
    {
        QFile::remove(this->filePath);
    }

    QFile file(this->filePath);
    if ( !file.open(QFile::WriteOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << file.fileName();
        return;
    }

    QTextStream out(&file);
    out << ui->fileEdit->toPlainText();
}

void EditFileDialog::on_undo_clicked()
{
    ui->fileEdit->undo();
}

void EditFileDialog::on_ok_clicked()
{
    on_save_clicked();
    this->accept();
}

void EditFileDialog::on_cancel_clicked()
{
    this->reject();
}

void EditFileDialog::on_redo_clicked()
{
    ui->fileEdit->redo();
}

void EditFileDialog::on_fileEdit_textChanged()
{
    setTextChanged(true);
}

bool EditFileDialog::getTextChanged() const
{
    return textChanged;
}

void EditFileDialog::setTextChanged(bool value)
{
    textChanged = value;
}

void EditFileDialog::createNewActionToFile(QString actionText)
{
//    bool ok;
//    QString newOperatorName = QInputDialog::getText(this, tr("Please input a unique action name"),
//            tr("Name:"), QLineEdit::Normal, QString("operator"), &ok);

//    if (!ok)
//        return;

//    if (newOperatorName.isEmpty())
//    {
//        QMessageBox::information(this, tr("KAVI"), tr("Action name can't be empty."));
//        return;
//    }

//    if ( !nameChecker.exactMatch(newOperatorName) )
//    {
//        QMessageBox::warning(this, tr("KAVI"),
//        tr("Name has wrong format.\n - only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n - must start with letter"));
//        return;
//    }

    QString newWindowTitle = this->filePath;

    newWindowTitle.append(" - Please modify the new added action's name");

    setWindowTitle(newWindowTitle);

    QString newOperatorName = "NewActionName";

    actionText.prepend("\n").prepend(newOperatorName).prepend("    (:action ");

    QString oldText = ui->fileEdit->toPlainText();
    oldText.remove(oldText.lastIndexOf(")"), 1);

    oldText.append(actionText);
    oldText.append(")");

    ui->fileEdit->clear();
    ui->fileEdit->setText(oldText);
    ui->fileEdit->setFocus();
}
