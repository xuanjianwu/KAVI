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
