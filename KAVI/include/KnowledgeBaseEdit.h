/*
 * @author: liYc
 * @date  : 2018/2/12
 * @brief : the dialog to view and edit knowledge base
*/

#ifndef KNOWLEDGEBASEEDIT_H
#define KNOWLEDGEBASEEDIT_H

#include "KAVIBase.h"
#include "KAVIClassKB.h"
#include "KAVIPredicateKB.h"
#include "ui_KnowledgeBaseEdit.h"

namespace Ui {
class KnowledgeBaseEdit;
}

class KnowledgeBaseEdit : public QDialog
{
    Q_OBJECT

public:
    explicit KnowledgeBaseEdit(KAVIClassKB *classKB, KAVIPredicateKB *predicateKB, QWidget *parent = 0);
    ~KnowledgeBaseEdit();
    void loadDataFromBase(KAVIKB *base, QListWidget *list);

private:
    // the dependent class knowledge base
    KAVIClassKB *classKB;
    // the dependent predicate knowledge base
    KAVIPredicateKB *predicateKB;
    Ui::KnowledgeBaseEdit *ui;
};

#endif // KNOWLEDGEBASEEDIT_H
