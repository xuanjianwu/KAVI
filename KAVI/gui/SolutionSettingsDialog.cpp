#include "SolutionSettingsDialog.h"
#include "ui_SolutionSettingsDialog.h"

using namespace XMLUtils;

SolutionSettingsDialog::SolutionSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolutionSettingsDialog)
{
    ui->setupUi(this);
    KAVIRunMode = Debug;
    selectdPlanner = QDomElement();

    getXMLDocument();
    rootElement = xmlData.firstChildElement("KAVIPlanners");
    setAllSettings();
}

SolutionSettingsDialog::~SolutionSettingsDialog()
{
    delete ui;
}

bool SolutionSettingsDialog::getXMLDocument()
{
    QString filePath = getXMLFilePath();
    QFile xmlFile(filePath.append(KAVIPLANNERS_FILE));

    if ( !xmlFile.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << xmlFile.fileName();
        return false;
    }

    QString error;
    int row = 0, column = 0;
    if (!xmlData.setContent(&xmlFile, false, &error, &row, &column))
    {
        QMessageBox::information(NULL, QString("Error"), QString("Parsing xml file failed at line row and column ") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        return false;
    }
    xmlFile.close();

    return true;
}

QString SolutionSettingsDialog::getXMLFilePath()
{
    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(PLANNERS_CONFIGS_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(PLANNERS_CONFIGS_DIR_RELEASE);
        break;
    default:
        break;
    }

    return filePath;
}

bool SolutionSettingsDialog::writeToXMLFile()
{
    QString filePath = getXMLFilePath();
    QFile xmlFile(filePath.append(KAVIPLANNERS_FILE));

    QString fileName = xmlFile.fileName();
    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }
    QFile newXmlFile(fileName);

    if ( !newXmlFile.open(QFile::WriteOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << newXmlFile.fileName();
        return false;
    }
    QTextStream saveStream(&newXmlFile);
    xmlData.save(saveStream, OUTPUT_INDENT);
    newXmlFile.close();

    qDebug() << "@KAVI planners xml file succesfully saved.";
}

void SolutionSettingsDialog::setAllSettings()
{
    setExperimentSettings();
    setPlannersSettings();
}

void SolutionSettingsDialog::saveAllSettings()
{
    saveExperimentSettings();
    savePlannerSettings();
}

void SolutionSettingsDialog::setExperimentSettings()
{
    QDomElement setttings = rootElement.firstChildElement("settings");
    QDomElement timeout = setttings.firstChildElement("timeout");
    QDomElement planValidation = setttings.firstChildElement("planValidation");

    QString timeoutEnabled = getStrAttribute(timeout, "enabled");
    if (timeoutEnabled == "true")
    {
        ui->TimeoutEnabled->setChecked(true);
    }
    else if (timeoutEnabled == "false")
    {
        ui->TimeoutEnabled->setChecked(false);
    }

    ui->TimeoutValue->setText(getStrValue(timeout));

    QString planValidationEnabled = getStrAttribute(planValidation, "enabled");
    if (planValidationEnabled == "true")
    {
        ui->PlanValidationEnabled->setChecked(true);
    }
    else if (planValidationEnabled == "false")
    {
        ui->PlanValidationEnabled->setChecked(false);
    }
}

void SolutionSettingsDialog::saveExperimentSettings()
{
    QDomElement setttings = rootElement.firstChildElement("settings");
    QDomElement timeout = setttings.firstChildElement("timeout");
    QDomElement planValidation = setttings.firstChildElement("planValidation");

    if (ui->TimeoutEnabled->checkState() == Qt::Checked)
    {
        setStrAttribute(timeout, "enabled", "true");
    }
    else if (ui->TimeoutEnabled->checkState() == Qt::Unchecked)
    {
        setStrAttribute(timeout, "enabled", "false");
    }

    setStrValue(timeout, ui->TimeoutValue->text());

    if (ui->PlanValidationEnabled->checkState() == Qt::Checked)
    {
        setStrAttribute(planValidation, "enabled", "true");
    }
    else if (ui->PlanValidationEnabled->checkState() == Qt::Unchecked)
    {
        setStrAttribute(planValidation, "enabled", "false");
    }
}

void SolutionSettingsDialog::setPlannersSettings()
{
    QDomElement planners = rootElement.firstChildElement("planners");
    QDomElement planner = planners.firstChildElement("planner");
    ui->PlannerSelected->addItem("Select one planner");
    ui->PlannerSelected->setCurrentIndex(0);
    while (!planner.isNull())
    {
        QDomElement plannerName = planner.firstChildElement("name");
        QDomElement plannerVersion = planner.firstChildElement("version");

        QString name = getStrValue(plannerName);
        QString version = getStrValue(plannerVersion);
        ui->PlannerSelected->addItem(name.append(" - ").append(version));

        planner = planner.nextSiblingElement("planner");
    }
    initPlannerParametersTable();
}

void SolutionSettingsDialog::savePlannerSettings()
{
    if (ui->PlannerSelected->currentIndex() < 1)
    {
        return;
    }
    QDomElement plannersElement = rootElement.firstChildElement("planners");
    selectdPlanner = findSubelementAttr(plannersElement, "planner", "id", QString::number(ui->PlannerSelected->currentIndex()));
    if (selectdPlanner.isNull())
    {
        return;
    }

    savePlannerParameters();

    QDomElement settingsElement = selectdPlanner.firstChildElement("settings");
    QDomElement timeoutElement = settingsElement.firstChildElement("timeout");

    if (ui->UseSpecificTimeout->checkState() == Qt::Checked)
    {
        setStrAttribute(timeoutElement, "enabled", "true");
    }
    else if (ui->UseSpecificTimeout->checkState() == Qt::Unchecked)
    {
        setStrAttribute(timeoutElement, "enabled", "false");
    }
    setStrValue(timeoutElement, ui->TimeoutValue_2->text());

    selectdPlanner.clear();
}

void SolutionSettingsDialog::initPlannerParametersTable()
{
    ui->ParameterTable->setColumnCount(PLANNER_PARAMETERS_TABLE_COLUMN_COUNT);
    QStringList tableHeadersList;
    tableHeadersList << "Id" << "Parameter"<< "Enable" << "Default Value" << "Description" ;
    ui->ParameterTable->setHorizontalHeaderLabels(tableHeadersList);
    ui->ParameterTable->horizontalHeader()->setStretchLastSection(true);
    ui->ParameterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ParameterTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->ParameterTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->ParameterTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->ParameterTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->ParameterTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->ParameterTable->verticalHeader()->setHidden(true);
}

void SolutionSettingsDialog::fillPlannerParametersTable()
{
    QDomElement settingsElement = selectdPlanner.firstChildElement("settings");
    QDomElement arguments = settingsElement.firstChildElement("arguments");
    QDomElement argument = arguments.firstChild().toElement();

    int argumentRowIndex = 0;
    ui->ParameterTable->setRowCount(arguments.childNodes().count());

    while (!argument.isNull())
    {
        int argumentID = getIntAttribute(argument, "id");
        QString parameter = getStrAttribute(argument, "parameter");
        QString description = getStrValue(argument.firstChildElement("description"));
        QString enable = getStrValue(argument.firstChildElement("enable"));
        QString value = getStrValue(argument.firstChildElement("value"));

        ui->ParameterTable->setItem(argumentRowIndex, 0, new QTableWidgetItem(QString::number(argumentID), QTableWidgetItem::Type));
        ui->ParameterTable->setItem(argumentRowIndex, 1, new QTableWidgetItem(parameter, QTableWidgetItem::Type));

        QComboBox* enableComboBox = new QComboBox();
        enableComboBox->addItem("true");
        enableComboBox->addItem("false");
        enableComboBox->setCurrentText(enable);
        ui->ParameterTable->setCellWidget(argumentRowIndex, 2, enableComboBox);

        ui->ParameterTable->setItem(argumentRowIndex, 3, new QTableWidgetItem(value, QTableWidgetItem::Type));
        ui->ParameterTable->setItem(argumentRowIndex, 4, new QTableWidgetItem(description, QTableWidgetItem::Type));
        ui->ParameterTable->item(argumentRowIndex, 4)->setToolTip(description);

        ui->ParameterTable->item(argumentRowIndex, 0)->setFlags((Qt::ItemFlags) 0);
        ui->ParameterTable->item(argumentRowIndex, 1)->setFlags((Qt::ItemFlags) 0);
        ui->ParameterTable->item(argumentRowIndex, 4)->setFlags((Qt::ItemFlags) 0);

        argumentRowIndex++;

        argument = argument.nextSibling().toElement();
    }
}

void SolutionSettingsDialog::savePlannerParameters()
{
    QDomElement settingsElement = selectdPlanner.firstChildElement("settings");
    QDomElement arguments = settingsElement.firstChildElement("arguments");
    QDomElement argument = arguments.firstChild().toElement();

    for (int i = 0; i < ui->ParameterTable->rowCount(); i++)
    {
        QComboBox* enableComboBox = dynamic_cast<QComboBox*>(ui->ParameterTable->cellWidget(i, 2));
        QDomElement enableElement = argument.firstChildElement("enable");
        QDomElement valueElement = argument.firstChildElement("value");
        setStrValue(enableElement, enableComboBox->currentText());
        setStrValue(valueElement, ui->ParameterTable->item(i, 3)->text());

        argument = argument.nextSibling().toElement();
    }
}

void SolutionSettingsDialog::on_PlannerSelected_currentIndexChanged(int index)
{
    ui->ParameterTable->clearContents();
    ui->ParameterTable->setRowCount(0);

    ui->Windows->setChecked(false);
    ui->Linux->setChecked(false);
    ui->MacOS->setChecked(false);
    ui->UseSpecificTimeout->setChecked(false);

    if (index < 1)
    {
        ui->UseSpecificTimeout->setEnabled(false);
        ui->TimeoutValue_2->setEnabled(false);
        ui->TimeoutValue_2->clear();
        return;
    }
    QDomElement plannersElement = rootElement.firstChildElement("planners");
    selectdPlanner = findSubelementAttr(plannersElement, "planner", "id", QString::number(index));
    if (selectdPlanner.isNull())
    {
        return;
    }
    fillPlannerParametersTable();

    ui->UseSpecificTimeout->setEnabled(true);
    ui->TimeoutValue_2->setEnabled(true);

    // set the os version for the selected planner
    QDomElement platformElement = selectdPlanner.firstChildElement("platform");
    QDomElement osElement = platformElement.firstChild().toElement();
    while (!osElement.isNull())
    {
        QString osName = osElement.tagName();
        if (osName == "windows")
        {
            ui->Windows->setChecked(true);
        }
        else if (osName == "linux")
        {
            ui->Linux->setChecked(true);
        }
        else if (osName == "mac")
        {
            ui->MacOS->setChecked(true);
        }
        osElement = osElement.nextSibling().toElement();
    }

    // set the use specific timeout checkbox
    QDomElement settingsElement = selectdPlanner.firstChildElement("settings");
    QDomElement timeoutElement = settingsElement.firstChildElement("timeout");
    QString timeoutEnable = getStrAttribute(timeoutElement, "enabled");
    if (timeoutEnable == "true")
    {
        ui->UseSpecificTimeout->setChecked(true);
    }
    else if (timeoutEnable == "false")
    {
        ui->UseSpecificTimeout->setChecked(false);
    }

    // set the specific timeout value
    QString timeoutValue = getStrValue(timeoutElement);
    ui->TimeoutValue_2->setText(timeoutValue);

    selectdPlanner.clear();
}


void SolutionSettingsDialog::on_saveButton_clicked()
{
    saveAllSettings();
}

void SolutionSettingsDialog::on_buttonBox_accepted()
{
    saveAllSettings();
    writeToXMLFile();
    this->accept();
}
