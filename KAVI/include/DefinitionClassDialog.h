#ifndef DEFINITIONCLASSDIALOG_H
#define DEFINITIONCLASSDIALOG_H

#include <QDialog>

namespace Ui {
class DefinitionClassDialog;
}

class DefinitionClassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DefinitionClassDialog(QWidget *parent = 0);
    ~DefinitionClassDialog();

private:
    Ui::DefinitionClassDialog *ui;
};

#endif // DEFINITIONCLASSDIALOG_H
