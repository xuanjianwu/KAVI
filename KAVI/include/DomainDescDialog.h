/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : the domain description dialog
*/

#ifndef DOMAINDESCDIALOG_H
#define DOMAINDESCDIALOG_H

#include "KAVIBase.h"
#include "XMLUtils.h"
#include "ui_DomainDescDialog.h"

KAVI_NS_BEGIN

class DomainDescDialog: public QDialog {
    Q_OBJECT

public:
    /*
    * construct function, set the content of dependent domain element
    * @params:
    *       domainElement - the specified domain element
    *       parent        - parent object
    * @return: N/A
    */
    DomainDescDialog(QDomElement& domainElement, QWidget* parent = 0);

private slots:
    /*
    * handle the button box accepted, update the description of domain element
    * @params: N/A
    * @return: void
    */
    void on_buttonBox_accepted();

private:
    // the domain element to descrip
    QDomElement domainEl;

    Ui::DomainDescDialog ui;
}

KAVI_NS_END

#endif // DOMAINDESCDIALOG_H
