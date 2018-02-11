/*
 * @author: liYc
 * @date  : 2018/2/8
 * @brief : the class dialog in definition view, dependent on knowledge base
*/

#ifndef DEFINITIONCLASSDIALOG_H
#define DEFINITIONCLASSDIALOG_H

#include "KAVIBase.h"
#include "KAVIClassKB.h"
#include "ui_DefinitionClassDialog.h"

namespace Ui {
class DefinitionClassDialog;
}

class DefinitionClassDialog : public QDialog
{
    Q_OBJECT

public:

    /*
    * construct function
    * @params:
    *       classKB - the dependent class knowledge base
    *       parent  - parent object
    * @return: N/A
    */
    explicit DefinitionClassDialog(KAVIClassKB *classKB, QWidget *parent = 0);

    /*
    * deconstruct function
    * @params: N/A
    * @return: N/A
    */
    ~DefinitionClassDialog();

    /*
    * get the current select class's name
    * @params: N/A
    * @return: class's name
    */
    QString className() const;

public slots:

    /*
    * after complete the edit, add the selected class to cached base
    * @params: N/A
    * @return: void
    */
    void editComplete();

private:
    // the string list model for the completer
    QStringListModel *listModel;
    // the dependent class knowledge base
    KAVIClassKB *classKB;
    Ui::DefinitionClassDialog *ui;
};

#endif // DEFINITIONCLASSDIALOG_H
