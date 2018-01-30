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
    /*
    * construct function with the classes
    * @params:
    *       classes  - available classes used to add new object
    *       parent   - parent object
    * @return: N/A
    */
    ProblemsObjectDialog(QStringList classes, QWidget* parent = 0);

    /*
    * get the selected class for the object to add
    * @params: N/A
    * @return: the specified class's name
    */
    QString objectClass();

    /*
    * get the input name for the object to add
    * @params: N/A
    * @return: the name for the new object
    */
    QString objectName();

private:

    Ui::ProblemsObjectDialog ui;
}

KAVI_NS_END

#endif // PROBLEMSOBJECTDIALOG_H
