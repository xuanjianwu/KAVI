#ifndef DEFINITIONPREDICATEDIALOG_H
#define DEFINITIONPREDICATEDIALOG_H

#include "KAVIBase.h"
#include "KAVIPredicateKB.h"
#include "ui_DefinitionPredicateDialog.h"

namespace Ui {
class DefinitionPredicateDialog;
}

class DefinitionPredicateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DefinitionPredicateDialog(KAVIPredicateKB *predicateKB, QWidget *parent = 0);
    ~DefinitionPredicateDialog();

    QString predicateSign() const;

public slots:
    void editComplete();

private:
    QStringListModel *listModel;
    KAVIPredicateKB *predicateKB;
    Ui::DefinitionPredicateDialog *ui;
};

#endif // DEFINITIONPREDICATEDIALOG_H
