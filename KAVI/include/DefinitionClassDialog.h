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
    explicit DefinitionClassDialog(KAVIClassKB *classKB, QWidget *parent = 0);
    ~DefinitionClassDialog();

    QString className() const;

public slots:
    void editComplete();

private:
    QStringListModel *listModel;
    KAVIClassKB *classKB;
    Ui::DefinitionClassDialog *ui;
};

#endif // DEFINITIONCLASSDIALOG_H
