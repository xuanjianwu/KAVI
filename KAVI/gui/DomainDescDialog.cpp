#include "DomainDescDialog.h"

KAVI_NS_BEGIN


using namespace XMLUtils;

DomainDescDialog::DomainDescDialog(QDomElement &domainElement, QWidget *parent)
    :QDialog(parent)
{
    Q_ASSERT(!domainElement.isNull());
    domainEl = domainElement;

    ui.setupUi(this);

    ui.domainName->setText(domainElement.attribute("name", QString("Unnamed")));

    QDomElement description = domainElement.firstChildElement("description");

    if (!description.isNull())
        ui.domainDescription->setPlainText(description.text());
}

void DomainDescDialog::on_buttonBox_accepted()
{
    QRegExp nameChecker(QString("^([a-z]|[A-Z]|[0-9])+$"));

    QString name = ui.domainName->text();
    name.truncate(MAX_DOMAIN_NAME_LENGTH);
    if ( !name.isEmpty() && nameChecker.exactMatch(name) )
    {
        domainEl.setAttribute("name", name);
    }
    else
        QMessageBox::warning(this, tr("KAVI"), tr("Domain name not updated - wrong format.\n- only letters and digits allowed"));

    QDomElement description = domainEl.firstChildElement("description");

    if (description.isNull())
    {
        QDomDocument parentDoc = domainEl.ownerDocument();
        description = parentDoc.createElement("description");
        domainEl.insertBefore(description, domainEl.firstChild());
    }

    setStrValue(description, ui.domainDescription->toPlainText());
}



KAVI_NS_END
