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
    DomainDescDialog(QDomElement& domainElement, QWidget* parent = 0);

private slots:

    void on_buttonBox_accepted();

private:

    QDomElement domainEl;

    Ui::DomainDescDialog ui;
}

KAVI_NS_END

#endif // DOMAINDESCDIALOG_H
