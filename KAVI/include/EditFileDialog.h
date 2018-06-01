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
    /*
     * construct function with text file path
     * @params:
     *      filePath - text file path
     *      parent   - parent object
     * @return: N/A
    */
    explicit EditFileDialog(QString filePath, QWidget *parent = 0);

    /*
     * construct function with text file path and added text
     * @params:
     *      filePath - text file path
     *      addText  - the specified added text to the file
     *      parent   - parent object
     * @return: N/A
    */
    EditFileDialog(QString filePath, QString addText, QWidget *parent);

    /*
     * deconstruct function
     * @params: N/A
     * @return: N/A
    */
    ~EditFileDialog();

    /*
     * init the file edit from file path
     * @params: N/A
     * @return: N/A
    */
    void initFileEditFromFilePath();

    /*
     * get the file path
     * @params: N/A
     * @return: file path
    */
    QString getFilePath() const;

    /*
     * set the file path
     * @params:
     *      value - the specified file path
     * @return: N/A
    */
    void setFilePath(const QString &value);

    /*
     * if the text has changed
     * @params: N/A
     * @return: changed or not
    */
    bool getTextChanged() const;

    /*
     * set the text change flag
     * @params:
     *      value - the value of flag
     * @return: N/A
    */
    void setTextChanged(bool value);

    /*
     * add the text of new action to the file text
     * @params:
     *      actionText - the new action's text
     * @return: N/A
    */
    void createNewActionToFile(QString actionText);

private slots:
    /*
     * handle the save text event
     * @params: N/A
     * @return: N/A
    */
    void on_save_clicked();

    /*
     * handle the undo event
     * @params: N/A
     * @return: N/A
    */
    void on_undo_clicked();

    /*
     * handle the save text and quit event
     * @params: N/A
     * @return: N/A
    */
    void on_ok_clicked();

    /*
     * handle the cancel and quit event
     * @params: N/A
     * @return: N/A
    */
    void on_cancel_clicked();

    /*
     * handle the redo event
     * @params: N/A
     * @return: N/A
    */
    void on_redo_clicked();

    /*
     * handle the text changed event
     * @params: N/A
     * @return: N/A
    */
    void on_fileEdit_textChanged();

private:
    Ui::EditFileDialog *ui;

    // the editing file's path
    QString filePath;

    QRegExp nameChecker;

    // the text changed flag
    bool textChanged;
};

#endif // EDITFILEDIALOG_H
