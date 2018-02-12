/*
 * @author: liYc
 * @date  : 2018/2/12
 * @brief : the dialog to edit class in knowledge base
*/

#ifndef KNOWLEDGEBASECLASSEDITDIALOG_H
#define KNOWLEDGEBASECLASSEDITDIALOG_H

#include "KAVIBase.h"
#include "ui_KnowledgeBaseClassEditDialog.h"

namespace Ui {
class KnowledgeBaseClassEditDialog;
}

class KnowledgeBaseClassEditDialog : public QDialog
{
    Q_OBJECT

public:

    /*
    * construct function
    * @params:
    *       content - the specified class content used to edit, NULL in the case that Add class
    *       parent  - parent object
    * @return: N/A
    */
    explicit KnowledgeBaseClassEditDialog(QString content, QWidget *parent = 0);
    ~KnowledgeBaseClassEditDialog();

    /*
    * get the current class content in the edit dialog
    * @params: N/A
    * @return: class content/name
    */
    QString className() const;

private:
    Ui::KnowledgeBaseClassEditDialog *ui;
};

#endif // KNOWLEDGEBASECLASSEDITDIALOG_H
