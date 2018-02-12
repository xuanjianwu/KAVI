/*
 * @author: liYc
 * @date  : 2018/2/12
 * @brief : the dialog to edit predicate in knowledge base
*/

#ifndef KNOWLEDGEBASEPREDICATEEDITDIALOG_H
#define KNOWLEDGEBASEPREDICATEEDITDIALOG_H

#include "KAVIBase.h"
#include "ui_KnowledgeBasePredicateEditDialog.h"

namespace Ui {
class KnowledgeBasePredicateEditDialog;
}

class KnowledgeBasePredicateEditDialog : public QDialog
{
    Q_OBJECT

public:
    /*
    * construct function
    * @params:
    *       content - the specified predicate content used to edit, NULL in the case that Add predicate
    * @return:
    */
    explicit KnowledgeBasePredicateEditDialog(QString content, QWidget *parent = 0);
    ~KnowledgeBasePredicateEditDialog();

    /*
    * get the current predicate sign in the edit dialog
    * @params: N/A
    * @return: predicate sign
    */
    QString predicateSign() const;

private:
    Ui::KnowledgeBasePredicateEditDialog *ui;
};

#endif // KNOWLEDGEBASEPREDICATEEDITDIALOG_H
