/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the object dialog in problem view
*/

#ifndef PROBLEMSOBJECTDIALOG_H
#define PROBLEMSOBJECTDIALOG_H

#include "KAVIBase.h"
#include "ui_ProblemsObjectDialog.h"

KAVI_NS_BEGIN

class ProblemsObjectDialog: public QDialog {
    Q_OBJECT

public:
    ProblemsObjectDialog(QStringList classes, QWidget* parent = 0);

    QString objectClass();
    QString objectName();

private:

    Ui::ProblemsObjectDialog ui;
}

KAVI_NS_END

#endif // PROBLEMSOBJECTDIALOG_H
