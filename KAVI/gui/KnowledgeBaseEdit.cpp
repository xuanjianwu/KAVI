#include "KnowledgeBaseEdit.h"
#include "ui_KnowledgeBaseEdit.h"

KnowledgeBaseEdit::KnowledgeBaseEdit(KAVIClassKB *classKB, KAVIPredicateKB *predicateKB, QWidget *parent) :
    QDialog(parent), ui(new Ui::KnowledgeBaseEdit)
{
    ui->setupUi(this);

    this->classKB = classKB;
    this->predicateKB = predicateKB;

    loadDataFromBase(classKB, ui->classList);
    loadDataFromBase(predicateKB, ui->predicateList);
}

KnowledgeBaseEdit::~KnowledgeBaseEdit()
{
    delete ui;
}

void KnowledgeBaseEdit::loadDataFromBase(KAVIKB *base, QListWidget *list)
{
    list->addItems(base->getData());
}
