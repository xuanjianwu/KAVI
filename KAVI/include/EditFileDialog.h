/*
 * @author: liYc
 * @date  : 2018/3/7
 * @brief : the class supports the file text edit
*/

#ifndef EDITFILEDIALOG_H
#define EDITFILEDIALOG_H

#include "KAVIBase.h"

namespace Ui {
class EditFileDialog;
}

class EditFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditFileDialog(QString filePath, QWidget *parent = 0);

    EditFileDialog(QString filePath, QString addText, QWidget *parent);

    ~EditFileDialog();

    void initFileEditFromFilePath();

    QString getFilePath() const;
    void setFilePath(const QString &value);

    bool getTextChanged() const;
    void setTextChanged(bool value);

    void createNewActionToFile(QString actionText);

private slots:
    void on_save_clicked();

    void on_undo_clicked();

    void on_ok_clicked();

    void on_cancel_clicked();

    void on_redo_clicked();

    void on_fileEdit_textChanged();

private:
    Ui::EditFileDialog *ui;
    QString filePath;

    QRegExp nameChecker;

    bool textChanged;
};

#endif // EDITFILEDIALOG_H
