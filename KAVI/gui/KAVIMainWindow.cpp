#include "KAVIMainWindow.h"
#include "NodeStructure.h"
#include "EdgeStructure.h"
#include "TreeModel.h"
#include "XMLUtils.h"
#include "CheckUtils.h"
#include "igraph.h"
#include "GraphClass.h"
#include "DiagramEvents.h"
#include "Convertor.h"
#include "EditWidget.h"
#include "InfoPanel.h"

#include "DomainDescDialog.h"


using namespace XMLUtils;
using namespace CheckUtils;
using namespace KAVIGraph;



KAVIMainWindow::KAVIMainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    ui.setupUi(this);

    domainChanged = false;

    nameChecker.setPattern(QString("^([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|[-_]){,%1}$").arg(MAX_NAME_LENGTH));

    dataConvertor = new Convertor(this);

    classTreeModel = NULL;

    // add the three dock widget to the menuView
    ui.menuView_V->addAction(ui.classDock->toggleViewAction());
    ui.menuView_V->addAction(ui.propertyEditorDock->toggleViewAction());
    ui.menuView_V->addAction(ui.logDock->toggleViewAction());

    // every moment, only one of this four insert mode can be active
    QActionGroup * toolbarGroup = new QActionGroup(this);
    toolbarGroup->addAction(ui.actionAddRectNode);
    toolbarGroup->addAction(ui.actionAddEllipseNode);
    toolbarGroup->addAction(ui.actionAddEdge);
    toolbarGroup->addAction(ui.actionDelete);
    // If exclusive is true, only one checkable action in the action group can ever
    // be active at any time. If the user chooses another checkable action in the
    // group, the one they chose becomes active and the one that was active becomes
    // inactive.
    toolbarGroup->setExclusive(true);

    // when new a domain, clear the old action and task
    connect(ui.actionNew_N, SIGNAL(triggered()), ui.actionSelector, SLOT(clear()));
    connect(ui.actionNew_N, SIGNAL(triggered()), ui.taskSelector, SLOT(clear()));

    ui.taskEdit->setTaskState(InitState);

    // default insert mode
    ui.actionAddRectNode->setChecked(true);

    // init the info panel
    InfoPanel *infoWidget = new InfoPanel(ui.propertyEditorDock);
    ui.propertyEditorDock->setWidget(infoWidget);
    infoWidget->show();

    // new a domain when MainWindow run
    on_actionNew_N_triggered();
}

void KAVIMainWindow::appendToLog(bool prefixed, const QString &text, QColor textColor)
{
    ui.logTextEdit->setTextColor(textColor);

    if (prefixed )
    {
        ui.logTextEdit->append(text);
    }
}

void KAVIMainWindow::crash()
{
    QFile file("CRASHDUMP");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream dump(&file);

    dump << ui.logTextEdit->toPlainText();
    file.close();
    this->close();
}

void KAVIMainWindow::on_actionNew_N_triggered()
{
    if ( !askForSave("New domain") )
        return;

    initDomain(QDomDocument());

    // because the new domain does not define any operators and problems yet
    // and operators and problems must depend the definition
    ui.actionTab->setEnabled(false);
    ui.taskTab->setEnabled(false);
}

void KAVIMainWindow::on_actionOpen_O_triggered()
{
    if ( !askForSave("Open domain") )
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Planning domains (*.xml)"));

    if ( fileName.isEmpty() )
        return;

    QFile domainFile(fileName);

    if ( !domainFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::critical(this, tr("Error"), tr("Can't open file %1").arg(fileName));
        return;
    }

    domainData = dataConvertor->readFromXML(domainFile);

    initDomain(domainData);

    ui.tabWidget->setCurrentIndex(TAB_INDEX_DEFINITION);
    centerContents(ui.definitionEdit);
}

void KAVIMainWindow::on_actionSave_As_A_triggered()
{
    // save to domainData
    globalSave();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Planning domains (*.xml)"));

    if ( fileName.isEmpty() )
        return;

    QFile file(fileName);

    if ( !file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::critical(this, tr("Error"), tr("Can't open file %1").arg(fileName));
        return;
    }

    dataConvertor->writeToXML(domainData, file);

    file.close();

    domainChanged = false;
}

void KAVIMainWindow::on_actionExit_X_triggered()
{
    if ( !askForSave("Open domain") )
        return;

    qApp->quit();
}

void KAVIMainWindow::on_actionDomain_PDDL_triggered()
{
    // save to domainData
    globalSave();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export domain"), ".", tr("Planning domains (*.pddl)"));

    if ( fileName.isEmpty() )
        return;

    QFile file(fileName);

    if ( !file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::critical(this, tr("Error"), tr("Can't open file %1").arg(fileName));
        return;
    }

    dataConvertor->writeDomainToPDDL(domainData, file);

    file.close();
}

void KAVIMainWindow::on_actionProblem_PDDL_triggered()
{
    // save to domainData
    globalSave();
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Export problems"), ".",
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if ( dirName.isEmpty() )
        return;

    dataConvertor->writeProblemsToPDDL(domainData, dirName);
}

void KAVIMainWindow::on_actionEdit_Properties_P_triggered()
{
    QDomElement domainEl = domainData.documentElement();
    Q_ASSERT(!domainEl.isNull());

    // init the domain descriotion dialog with the domainData
    DomainDescDialog dialog(domainEl, this);

    // if the domain descriotion dialog was accepted, set domainChanged = true;
    domainChanged = dialog.exec();
}

void KAVIMainWindow::on_actionDomain_Summary_S_triggered()
{
    // clear the log
    ui.logTextEdit->clear();

    // save to domainData
    globalSave();

    QDomElement definitionDiagram = definitionDocument.firstChildElement("diagram");

    // print the summary of classes defined currently
    NodeStructure tempNode;
    tempNode.setData(nodeType, NST_CLASS);

    QList<QDomElement> definedClasses = selectMatchingElementList(definitionDiagram, tempNode);

    if (definedClasses.isEmpty())
        qDebug() << "@No classes defined.";
    else
    {
        qDebug() << "@Defined classes:";

        foreach(QDomElement actClass, definedClasses)
        {
            qDebug() << "@" << subelementTagValue(actClass, "label");
        }
    }


    // print the summary of predicates defined currently
    tempNode.setData(nodeType, NST_PREDICATE);

    QList<QDomElement> definedPredicates = selectMatchingElementList(definitionDiagram, tempNode);

    // specified the count of invalid predicates
    int invalidPredCnt = 0;

    QSet<QString> processed;
    if (definedPredicates.isEmpty())
        qDebug() << "@No predicates defined.";
    else
    {
        qDebug() << "@Defined predicates:";

        foreach(QDomElement actPred, definedPredicates)
        {
            QString predName = subelementTagValue(actPred, "label");

            // handle the overloaded predicate, because it have been processed before
            if (processed.contains(predName))
                continue;

            // all the overloaded predicate have equal count of arguments
            if(checkPredicateDefinition(actPred, false))
            {
                qDebug() << "@" << predName;
                processed.insert(predName);
            }
            else
            {
                qWarning() << "@" << predName;
                ++invalidPredCnt;
                processed.insert(predName);
            }
        }
    }


    // print the summary of operators defined currently

    // the list of correct operators' name
    QStringList correctOper;
    // the list of incorrect operators' name
    QStringList incorrectOper;

    brokenDiagrams(domainData, definitionDiagram, "operators", "action", correctOper, incorrectOper);

    if (correctOper.size() > 0)
    {
        qDebug() << "@Defined operators:";
        foreach(QString op, correctOper)
        {
            qDebug() << "@" << op;
        }
    }

    if ( incorrectOper.size() > 0 )
    {
        qDebug() << "@Incorrect operators:";
        foreach(QString op, incorrectOper)
        {
            qWarning() << "@" << op;
        }
    }

    if ( (correctOper.size() + incorrectOper.size()) == 0)
        qDebug() << "@No operators defined.";



    // print the summary of problems defined currently

    // the list of correct problems' name
    QStringList correctProb;

    // the list of incorrect problems' name
    QStringList incorrectProb;

    brokenDiagrams(domainData, definitionDiagram, "problems", "task", correctProb, incorrectProb);

    if (correctProb.size() > 0)
    {
        qDebug() << "@Defined problems:";
        foreach(QString prob, correctProb)
        {
            qDebug() << "@" << prob;
        }
    }

    if ( incorrectProb.size() > 0)
    {
        qDebug() << "@Incorrect problems:";

        foreach(QString prob, incorrectProb)
        {
            qWarning() << "@" << prob;
        }
    }

    if ((correctProb.size() + incorrectProb.size()) == 0)
        qDebug() << "@No problems defined.";

    qDebug() << "@---------Summary---------";
    qDebug() << "@#classes: " << definedClasses.count();

    if (invalidPredCnt > 0)
        qWarning() << "@#predicates: " << (processed.count() - invalidPredCnt) << "/" << invalidPredCnt
               << " (valid/invalid)";
    else
        qDebug() << "@#predicates: " << processed.count();

    if (incorrectOper.size() > 0)
        qWarning() << "@#operators: " << correctOper.size() << "+" << incorrectOper.count() << "(correct + incorrect)";
    else
        qDebug() << "@#operators: " << correctOper.size();

    if (incorrectProb.size() > 0)
        qWarning() << "@#problems: " << correctProb.size() << "+" << incorrectProb.count() << "(correct + incorrect)";
    else
        qDebug() << "@#problems: " << correctProb.size();
}

void KAVIMainWindow::on_actionClear_Log_C_triggered()
{
    ui.logTextEdit->clear();
}

void KAVIMainWindow::on_actionAbout_A_triggered()
{

}

void KAVIMainWindow::on_actionAddEllipseNode_toggled(bool arg1)
{
    if (arg1)
        globalSetEditMode(AddEllipse);
}

void KAVIMainWindow::on_actionAddRectNode_toggled(bool arg1)
{
    if (arg1)
        globalSetEditMode(AddRect);
}

void KAVIMainWindow::on_actionAddEdge_toggled(bool arg1)
{
    if (arg1)
        globalSetEditMode(AddEdge);
}

void KAVIMainWindow::on_actionDelete_toggled(bool arg1)
{
    if (arg1)
        globalSetEditMode(Delete);
}

void KAVIMainWindow::on_actionCreateAction_triggered()
{
    bool ok;
    QString newOperatorName = QInputDialog::getText(this, tr("Create new operator"),
            tr("Name:"), QLineEdit::Normal, QString("operator"), &ok);

    if (!ok)
        return;

    if (newOperatorName.isEmpty())
    {
        QMessageBox::information(this, tr("KAVI"), tr("Operator name can't be empty."));
        return;
    }

    if ( !nameChecker.exactMatch(newOperatorName) )
    {
        QMessageBox::warning(this, tr("KAVI"),
        tr("Name has wrong format.\n - only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n - must start with letter"));
        return;
    }

    if ( ui.actionSelector->findText(newOperatorName) >= 0)
    {
        QMessageBox::warning(this, tr("KAVI"), tr("Operator name must be unique."));
        return;
    }

    // get the operators element from domain
    QDomElement operatorsElement = findDomainSubelement("operators");
    Q_ASSERT(!operatorsElement.isNull());

    // create the action element for the new action in domain withour diagram element
    QDomElement newActionElement = domainData.createElement("action");
    newActionElement.setAttribute("name", newOperatorName);

    operatorsElement.appendChild(newActionElement);

    registerSubelement(newActionElement, ui.actionSelector);

    int index = ui.actionSelector->findText(newOperatorName);

    ui.actionSelector->setCurrentIndex(index);

    ui.actionTab->setEnabled(true);
    ui.actionDeleteAction->setEnabled(true);
    ui.actionCheckDiagram->setEnabled(true);

    domainChanged = true;
}

void KAVIMainWindow::on_actionDeleteAction_triggered()
{
    // get the operators element from domain
    QDomElement operatorsElement = findDomainSubelement("operators");
    Q_ASSERT(!operatorsElement.isNull());

    int actionCount = ui.actionSelector->count();
    if ( actionCount < 1 )
        return;

    QDomElement actionElement;
    actionElement = findSubelementAttr(operatorsElement, "action", "name", ui.actionSelector->currentText());

    // remove the action element from the domain
    operatorsElement.removeChild(actionElement);

    recentActionName = QString();

    if ( actionCount == 1)
    {
        ui.actionSelector->clear();
        ui.actionTab->setEnabled(false);
        ui.actionDeleteAction->setEnabled(false);
        ui.actionCheckDiagram->setEnabled(false);
    }
    else
    {
        int index = ui.actionSelector->currentIndex();

        ui.actionSelector->removeItem(index);
    }

    domainChanged = true;
}

void KAVIMainWindow::on_actionCreateProblem_triggered()
{
    bool ok;
    QString newTaskName = QInputDialog::getText(this, tr("Create new task"),
            tr("Task name:"), QLineEdit::Normal, QString("problem"), &ok);

    if (!ok)
        return;

    if (newTaskName.isEmpty())
    {
        QMessageBox::information(this, tr("KAVI"), tr("Task name can't be empty."));
        return;
    }

    if ( !nameChecker.exactMatch(newTaskName) )
    {
        QMessageBox::warning(this, tr("KAVI"),
        tr("Name has wrong format.\n - only letters and digits are allowed\n- max lenght is limited"));
        return;
    }

    if ( ui.taskSelector->findText(newTaskName) >= 0)
    {
        QMessageBox::warning(this, tr("KAVI"), tr("Task name must be unique."));
        return;
    }

    // get the problems element from domain
    QDomElement problemsElement = findDomainSubelement("problems");
    Q_ASSERT(!problemsElement.isNull());

    // create the task element for the new task in domain withour diagram element
    QDomElement newTaskElement = domainData.createElement("task");
    newTaskElement.setAttribute("name", newTaskName);

    problemsElement.appendChild(newTaskElement);

    registerSubelement(newTaskElement, ui.taskSelector);

    int index = ui.taskSelector->findText(newTaskName);

    ui.taskSelector->setCurrentIndex(index);

    ui.taskTab->setEnabled(true);
    ui.actionDeleteProblem->setEnabled(true);
    ui.actionCheckDiagram->setEnabled(true);

    domainChanged = true;
}

void KAVIMainWindow::on_actionDeleteProblem_triggered()
{
    // get the problems element from domain
    QDomElement problemsElement = findDomainSubelement("problems");

    int taskCount = ui.taskSelector->count();

    if ( taskCount < 1 )
        return;

    QDomElement taskElement;
    taskElement = findSubelementAttr(problemsElement, "task", "name", ui.taskSelector->currentText());

    // remove the task element from the domain
    problemsElement.removeChild(taskElement);

    recentTaskName = QString();

    if ( taskCount == 1 )
    {
        ui.taskSelector->clear();
        ui.taskTab->setEnabled(false);
        ui.actionDeleteProblem->setEnabled(false);
        ui.actionCheckDiagram->setEnabled(false);
    }
    else
    {
        int index = ui.taskSelector->currentIndex();

        ui.taskSelector->removeItem(index);
    }
    domainChanged = true;
}

void KAVIMainWindow::on_actionCheckDiagram_triggered()
{
    ui.logTextEdit->clear();

    int currentTab = ui.tabWidget->currentIndex();

    QDomElement definitionDiagram = definitionDocument.firstChildElement("diagram");
    if (currentTab == TAB_INDEX_DEFINITION)
    {
        qDebug() << "@Checking definition.";
        checkDefinition(definitionDiagram, true);
        return;
    }

    if (currentTab == TAB_INDEX_OPERATORS)
    {
        qDebug() << "@Checking operator" << recentActionName << ".";
        QDomElement actionDiagram = actionDocument.firstChildElement("diagram");
        checkDependentDiagram(actionDiagram, definitionDiagram, NST_VARIABLE, true);
        return;
    }

    if (currentTab == TAB_INDEX_PROBLEMS)
    {
        qDebug() << "@Checking problem" << recentTaskName << ".";
        QDomElement taskDiagram = taskDocument.firstChildElement("diagram");
        checkDependentDiagram(taskDiagram, definitionDiagram, NST_OBJECT, true);
        return;
    }

    Q_ASSERT(false);
}

void KAVIMainWindow::on_actionSavePNG_triggered()
{
    qDebug() << "@Saving Image";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images (*.png)"));

    if ( fileName.isEmpty() )
        return;

    int activeTab = ui.tabWidget->currentIndex();

    EditWidget *activeDiagram;

    switch(activeTab)
    {
    case TAB_INDEX_DEFINITION:
        activeDiagram = ui.definitionEdit;
        break;
    case TAB_INDEX_OPERATORS:
        activeDiagram = ui.actionEdit;
        break;
    case TAB_INDEX_PROBLEMS:
        activeDiagram = ui.taskEdit;
        break;
    default:
        activeDiagram = ui.definitionEdit;
    }

    QGraphicsScene *diagramScene = activeDiagram->scene();

    QRectF boundingRect = diagramScene->itemsBoundingRect();
    boundingRect.setWidth(boundingRect.width() + 1);
    boundingRect.setHeight(boundingRect.height() + 1);

    QSizeF diagramSize(boundingRect.width() + 2 * IMAGE_MARGIN, boundingRect.height() + 2 * IMAGE_MARGIN);
    QImage diagramImage(diagramSize.toSize(), QImage::Format_RGB32);

    diagramImage.fill(qRgb(255,255,255));

    QPainter diagramPainter(&diagramImage);

    const QRectF targetRect(IMAGE_MARGIN, IMAGE_MARGIN, boundingRect.width(), boundingRect.height());
    diagramScene->render(&diagramPainter, targetRect, boundingRect, Qt::KeepAspectRatio);

    diagramImage.save(fileName, "PNG", 5);
}

void KAVIMainWindow::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case TAB_INDEX_DEFINITION:
        ui.actionCreateAction->setEnabled(false);
        ui.actionDeleteAction->setEnabled(false);
        ui.actionCreateProblem->setEnabled(false);
        ui.actionDeleteProblem->setEnabled(false);

        ui.actionCheckDiagram->setEnabled(true);

        centerContents(ui.definitionEdit);
        break;
    case TAB_INDEX_OPERATORS:
        ui.actionCreateAction->setEnabled(true);

        ui.actionDeleteAction->setEnabled(ui.actionSelector->count() > 0);
        ui.actionCheckDiagram->setEnabled(ui.actionSelector->count() > 0);

        ui.actionCreateProblem->setEnabled(false);
        ui.actionDeleteProblem->setEnabled(false);

        centerContents(ui.actionEdit);
        break;
    case TAB_INDEX_PROBLEMS:
        ui.actionCreateAction->setEnabled(false);
        ui.actionDeleteAction->setEnabled(false);

        ui.actionCreateProblem->setEnabled(true);

        ui.actionDeleteProblem->setEnabled(ui.taskSelector->count() > 0);
        ui.actionCheckDiagram->setEnabled(ui.taskSelector->count() > 0);

        centerContents(ui.taskEdit);
        break;
    default:

        Q_ASSERT(false);
    }

    if ( ui.definitionEdit->wasChanged() )
    {
        saveDefinition();
        ui.definitionEdit->setChanged(false);
    }

    if ( ui.actionEdit->wasChanged() )
    {
        saveDiagram("action", ui.actionSelector->currentText(), "operators", actionDocument);
        ui.actionEdit->setChanged(false);
    }

    if ( ui.taskEdit->wasChanged() )
    {
        saveDiagram("task", ui.taskSelector->currentText(), "problems", taskDocument);
        ui.taskEdit->setChanged(false);
    }

    resetPropertyEditor();
}

void KAVIMainWindow::on_definitionEdit_sceneChanged(int changeCode)
{
    qDebug() << "$KAVIMainWindow::on_definitionEdit_sceneChanged";

    bool checkOperators = false;
    bool checkProblems = false;

    switch(changeCode)
    {
    case RectNodeAdded:
        qDebug() << "@Class defined.";
        reloadClassTree();
        break;
    case EllipseNodeAdded:
        qDebug() << "@Predicate defined.";
        break;
    case AssocEdgeAdded:
        qDebug() << "@Predicate argument defined.";
        updatePropertyEditor();
        break;
    case RectNodeDeleted:
        qDebug() << "@Class deleted.";
        if (ui.actionSelector->count() > 0)
            checkOperators = true;
        if (ui.taskSelector->count() > 0)
            checkProblems = true;
        reloadClassTree();
        resetPropertyEditor();
        break;
    case EllipseNodeDeleted:
        qDebug() << "@Predicate deleted.";
        if (ui.actionSelector->count() > 0)
            checkOperators = true;
        if (ui.taskSelector->count() > 0)
            checkProblems = true;
        resetPropertyEditor();
        break;
    case AssocEdgeDeleted:
        qDebug() << "@Predicate argument removed.";
        qWarning() << "@Domain should be checked.";
        updatePropertyEditor();
        break;
    case AssocEdgeReconnected:
        qDebug() << "@Predicate argument redefined.";
        if (ui.actionSelector->count() > 0)
            checkOperators = true;
        if (ui.taskSelector->count() > 0)
            checkProblems = true;
        updatePropertyEditor();
        break;
    case InherEdgeAdded:
        qDebug() << "@Class inheritance defined.";
        reloadClassTree();
        break;
    case InherEdgeDeleted:
        qDebug() << "@Class inheritance removed.";
        qWarning() << "@Domain should be checked.";
        reloadClassTree();
        break;
    case InherEdgeReconnected:
        qDebug() << "@Class inheritance redefined.";
        if (ui.actionSelector->count() > 0)
            checkOperators = true;
        if (ui.taskSelector->count() > 0)
            checkProblems = true;
        qWarning() << "@Domain should be checked.";
        reloadClassTree();
        break;
    case ArgumentOrderChanged:
        qDebug() << "@Argument order changed.";
        if (ui.actionSelector->count() > 0)
            checkOperators = true;
        if (ui.taskSelector->count() > 0)
            checkProblems = true;
    default:
        qDebug() << "$changeCode =" << changeCode;
    }

    if (checkOperators)
    {
        QStringList correctOper;
        QStringList incorrectOper;

        QDomElement definitionDiagram = definitionDocument.firstChildElement("diagram");
        Q_ASSERT(!definitionDiagram.isNull());

        brokenDiagrams(domainData, definitionDiagram, "operators", "action", correctOper, incorrectOper);

        if (incorrectOper.count() > 0)
        {
            qWarning() << "@Following operators are now inconsistent:";
            foreach(QString op, incorrectOper)
                qWarning() << "@" <<op;
        }
    }

    if (checkProblems)
    {
        QStringList correctProb;
        QStringList incorrectProb;

        QDomElement definitionDiagram = definitionDocument.firstChildElement("diagram");
        Q_ASSERT(!definitionDiagram.isNull());

        brokenDiagrams(domainData, definitionDiagram, "problems", "task", correctProb, incorrectProb);

        if (incorrectProb.count() > 0)
        {
            qWarning() << "@Following problems are now inconsistent:";
            foreach(QString prob, incorrectProb)
                qWarning() << "@" << prob;
        }
    }

    domainChanged = true;
}

void KAVIMainWindow::on_definitionEdit_updateInfoPanel(QWidget *infoWidget)
{
    QWidget * oldWidget = ui.propertyEditorDock->widget();
    ui.propertyEditorDock->setWidget(infoWidget);
    infoWidget->setParent(ui.propertyEditorDock);
    infoWidget->show();

    if (oldWidget != NULL)
        delete oldWidget;
    else
        qWarning() << "$KAVIMainWindow::on_definitionEdit_updateInfoPanel : oldWidget == NULL";
}

void KAVIMainWindow::on_actionSelector_currentIndexChanged(const QString &arg1)
{
    if (arg1.isNull())
    {
        qDebug() << "$on_actionSelector_currentIndexChanged : action with empty name selected";
        ui.actionEdit->reset();
        actionDocument.clear();
        actionDocument.appendChild(actionDocument.createElement("diagram"));
        return;
    }

    if ( !recentActionName.isEmpty() )
        saveDiagram("action", recentActionName, "operators", actionDocument);

    QDomElement operatorsElement = findDomainSubelement("operators");
    QDomElement demandedAction = findSubelementAttr(operatorsElement, "action", "name", arg1);

    updateDocument(demandedAction, actionDocument);

    qDebug() << "@Loading operator:" << arg1;
    ui.actionEdit->loadXMLdata(actionDocument);

    recentActionName = arg1;

    centerContents(ui.actionEdit);

    resetPropertyEditor();
}

void KAVIMainWindow::on_actionEdit_sceneChanged(int changeCode)
{
    qDebug() << "$KAVIMainWindow::on_actionEdit_sceneChanged";

    bool runCheck = false;
    switch(changeCode)
    {
    case RectNodeAdded:
        qDebug() << "@Variable defined.";
        break;
    case EllipseNodeAdded:
        qDebug() << "@Predicate added.";
        break;
    case AssocEdgeAdded:
        qDebug() << "@Association defined.";
        updatePropertyEditor();
        break;
    case RectNodeDeleted:
        qDebug() << "@Variable deleted.";
        resetPropertyEditor();
        break;
    case EllipseNodeDeleted:
        qDebug() << "@Predicate deleted.";
        resetPropertyEditor();
        break;
    case AssocEdgeDeleted:
        qDebug() << "@Association deleted.";
        updatePropertyEditor();
        break;
    case AssocEdgeReconnected:
        qDebug() << "@Association changed.";
        updatePropertyEditor();
        break;
    case ArgumentOrderChanged:
        qDebug() << "@Argument order changed.";
        runCheck = true;
        break;
    default:
        qDebug() << "$changeCode =" << changeCode;
    }

    if (runCheck)
    {
        QDomElement definitionDiagram = definitionDocument.firstChildElement("diagram");
        Q_ASSERT(!definitionDiagram.isNull());

        QDomElement actionDiagram = actionDocument.firstChildElement("diagram");
        Q_ASSERT(!actionDiagram.isNull());

        if( !checkDependentDiagram(actionDiagram, definitionDiagram, NST_VARIABLE, false) )
            qWarning() << "@Predicate is inconsistent.";
    }

    domainChanged = true;
}

void KAVIMainWindow::on_actionEdit_updateInfoPanel(QWidget *infoWidget)
{
    QWidget * oldWidget = ui.propertyEditorDock->widget();
    ui.propertyEditorDock->setWidget(infoWidget);
    infoWidget->setParent(ui.propertyEditorDock);
    infoWidget->show();

    delete oldWidget;
}

void KAVIMainWindow::on_taskSelector_currentIndexChanged(const QString &arg1)
{
    if ( arg1.isNull() )
    {
        ui.taskEdit->reset();
        taskDocument.clear();
        taskDocument.appendChild(taskDocument.createElement("diagram"));
        return;
    }

    if ( !recentTaskName.isEmpty() )
        saveDiagram("task", recentTaskName, "problems", taskDocument);

    QDomElement problemsElement = findDomainSubelement("problems");
    QDomElement demandedTask = findSubelementAttr(problemsElement, "task", "name", arg1);

    updateDocument(demandedTask, taskDocument);

    qDebug() << "@Loading problem:" << arg1;
    ui.taskEdit->loadXMLdata(taskDocument);

    recentTaskName = arg1;

    centerContents(ui.taskEdit);

    resetPropertyEditor();
}

void KAVIMainWindow::on_goalCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui.taskEdit->setTaskState(GoalState);
        ui.taskEdit->setPredicatesVisible(true, GoalState);
        ui.actionAddEllipseNode->setEnabled(true);
    }
    else
    {
        ui.taskEdit->setPredicatesVisible(false, GoalState);

        if ( ui.initCheckBox->isChecked() )
            ui.taskEdit->setTaskState(InitState);
        else
        {
            ui.actionAddEllipseNode->setEnabled(false);
            ui.taskEdit->setTaskState(NoState);
        }
    }
}

void KAVIMainWindow::on_initCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui.taskEdit->setTaskState(InitState);
        ui.taskEdit->setPredicatesVisible(true, InitState);
        ui.actionAddEllipseNode->setEnabled(true);
    }
    else
    {
        ui.taskEdit->setPredicatesVisible(false, InitState);

        if ( ui.goalCheckBox->isChecked() )
            ui.taskEdit->setTaskState(GoalState);
        else
        {
            ui.actionAddEllipseNode->setEnabled(false);
            ui.taskEdit->setTaskState(NoState);
        }
    }
}

void KAVIMainWindow::on_taskEdit_sceneChanged(int changeCode)
{
    qDebug() << "$KAVIMainWindow::on_taskEdit_sceneChanged";

    bool runCheck = false;
    switch(changeCode)
    {
    case RectNodeAdded:
        qDebug() << "@Object defined.";
        break;
    case EllipseNodeAdded:
        qDebug() << "@Predicate added.";
        break;
    case AssocEdgeAdded:
        qDebug() << "@Association defined.";
        updatePropertyEditor();
        break;
    case RectNodeDeleted:
        qDebug() << "@Object deleted.";
        resetPropertyEditor();
        break;
    case EllipseNodeDeleted:
        qDebug() << "@Predicate deleted.";
        resetPropertyEditor();
        break;
    case AssocEdgeDeleted:
        qDebug() << "@Association deleted.";
        updatePropertyEditor();
        break;
    case AssocEdgeReconnected:
        qDebug() << "@Association changed.";
        updatePropertyEditor();
        break;
    case ArgumentOrderChanged:
        qDebug() << "@Argument order changed.";
        runCheck = true;
        break;
    case PredicateStateChanged:
        qDebug() << "@Predicate State changed.";
        runCheck = true;
        on_initCheckBox_toggled(ui.initCheckBox->isChecked());
        on_goalCheckBox_toggled(ui.goalCheckBox->isChecked());
        break;
    default:
        qDebug() << "$changeCode =" << changeCode;
    }

    if (runCheck)
    {
        QDomElement definitionDiagram = definitionDocument.firstChildElement("diagram");
        Q_ASSERT(!definitionDiagram.isNull());

        QDomElement taskDiagram = taskDocument.firstChildElement("diagram");
        Q_ASSERT(!taskDiagram.isNull());

        if( !checkDependentDiagram(taskDiagram, definitionDiagram, NST_VARIABLE, false) )
            qWarning() << "@Predicate is inconsistent.";
    }
    domainChanged = true;
}

void KAVIMainWindow::on_taskEdit_updateInfoPanel(QWidget *infoWidget)
{
    QWidget * oldWidget = ui.propertyEditorDock->widget();
    ui.propertyEditorDock->setWidget(infoWidget);
    infoWidget->setParent(ui.propertyEditorDock);
    infoWidget->show();

    delete oldWidget;
}

void KAVIMainWindow::on_classTreeView_clicked(const QModelIndex &index)
{
    int clicked = ui.classTreeView->model()->data(index, Qt::UserRole).toInt();
    qDebug() << "$KAVIMainWindow::on_classTreeView_clicked : ID =" << clicked;
}

void KAVIMainWindow::updateDocument(QDomElement &rootEl, QDomDocument &updatedDoc)
{
    if ( !updatedDoc.isNull() )
    {
        updatedDoc.clear();
        qDebug() << "$KAVIMainWindow::updateDocument : clearing old data";
    }

    if ( rootEl.isNull() )
    {
        qDebug() << "$KAVIMainWindow::updateDocument : rootEl is empty";
        return;
    }

    QDomElement targetDiagram = rootEl.firstChildElement("diagram");

    QDomNode docDiagram;

    if ( targetDiagram.isNull() )
    {
        qDebug() << "$KAVIMainWindow::updateDocument : creating new diagram element";
        targetDiagram = domainData.createElement("diagram");
        rootEl.appendChild(targetDiagram);

        docDiagram = updatedDoc.createElement("diagram");
    }
    else
    {
        docDiagram = updatedDoc.importNode(targetDiagram, true);
    }

    updatedDoc.appendChild(docDiagram);
}

QDomDocument KAVIMainWindow::createEmptyDomain()
{
    QDomDocument result;

    QDomElement rootEl = result.createElement("domain");

    result.appendChild(rootEl);

    rootEl.appendChild(result.createElement("definition"));
    rootEl.appendChild(result.createElement("operators"));
    rootEl.appendChild(result.createElement("problems"));

    return result;
}

void KAVIMainWindow::initDomain(QDomDocument domainDocument)
{
    if ( !domainDocument.isNull() )
    {
        domainData = domainDocument;
    }
    else
    {
        domainData = createEmptyDomain();
    }

    QDomElement definitionElement = findDomainSubelement("definition");
    initDefinition(definitionElement.firstChildElement("diagram"));

    recentActionName = QString();
    initOperators();

    recentTaskName = QString();
    initProblems();

    resetPropertyEditor();

    domainChanged = false;
}

QDomElement KAVIMainWindow::findDomainSubelement(const QString &tagName)
{
    QDomElement domainEl = domainData.documentElement();

    return domainEl.firstChildElement(tagName);
}

void KAVIMainWindow::initDefinition(QDomElement diagramElement)
{
    definitionDocument.clear();

    if ( diagramElement.isNull() )
    {
        qDebug() << "$KAVIMainWindow::initDefinition : creating empty diagram in definitionDocument";
        definitionDocument.appendChild(definitionDocument.createElement("diagram"));
    }
    else
    {
        qDebug() << "$KAVIMainWindow::initDefinition : loading diagram to definitionDocument";
        definitionDocument.appendChild(definitionDocument.importNode(diagramElement, true));
    }

    // load the definition document to the definitionEdit, including DataWidget and DiagramWidget
    ui.definitionEdit->loadXMLdata(definitionDocument);

    // set the dependent definition for actionEdit and taskEdit
    ui.actionEdit->setDefinition(ui.definitionEdit->xmlDataPointer());
    ui.taskEdit->setDefinition(ui.definitionEdit->xmlDataPointer());

    reloadClassTree();

    ui.definitionEdit->setChanged(false);
}

void KAVIMainWindow::initOperators()
{
    actionDocument.clear();
    ui.actionSelector->clear();

    QDomElement operatorsElement = findDomainSubelement("operators");

    QDomElement actionEl = operatorsElement.firstChildElement("action");

    while ( !actionEl.isNull() )
    {
        registerSubelement(actionEl, ui.actionSelector);

        actionEl = actionEl.nextSiblingElement("action");
    }

    recentActionName = ui.actionSelector->currentText();

    if ( recentActionName.isNull() )
    {
        qDebug() << "@No operator defined in this domain.";
        ui.actionTab->setEnabled(false);
    }
    else
    {
        qDebug() << "@Count of defined operators in this domain:" << ui.actionSelector->count();
        ui.actionTab->setEnabled(true);
    }
}

void KAVIMainWindow::initProblems()
{
    taskDocument.clear();
    ui.taskSelector->clear();

    QDomElement problemsElement = findDomainSubelement("problems");

    QDomElement taskEl = problemsElement.firstChildElement("task");

    while ( !taskEl.isNull() )
    {
        registerSubelement(taskEl,ui.taskSelector);

        taskEl = taskEl.nextSiblingElement("task");
    }

    recentTaskName = ui.taskSelector->currentText();

    if ( recentTaskName.isNull() )
    {
        qDebug() << "@No problem defined in this domain";
        ui.taskTab->setEnabled(false);
    }
    else
    {
        qDebug() << "@Count of defined problems in this domain:" << ui.taskSelector->count();
        ui.taskTab->setEnabled(true);
    }
}

bool KAVIMainWindow::askForSave(const QString &text)
{
    if ( domainChanged )
    {
        QMessageBox::StandardButton answer = QMessageBox::question(this, text, tr("Do you want to save current domain?"),
                QMessageBox::Save | QMessageBox::No |QMessageBox::Cancel, QMessageBox::Save);

        if ( answer == QMessageBox::Save )
        {
            on_actionSave_As_A_triggered();
            return true;
        }

        if ( answer == QMessageBox::Cancel)
            return false;

        if ( answer == QMessageBox::No )
            return true;
    }

    return true;
}

void KAVIMainWindow::globalSave()
{
    if ( ui.definitionEdit->wasChanged() )
    {
        saveDefinition();
        ui.definitionEdit->saveKB();
        ui.definitionEdit->setChanged(false);
    }

    if ( ui.actionEdit->wasChanged() )
    {
        saveDiagram("action", ui.actionSelector->currentText(), "operators", actionDocument);
        ui.actionEdit->setChanged(false);
    }

    if ( ui.taskEdit->wasChanged() )
    {
        saveDiagram("task", ui.taskSelector->currentText(), "problems", taskDocument);
        ui.taskEdit->setChanged(false);
    }
}

void KAVIMainWindow::saveDefinition()
{
    // get the old definition from the domain
    QDomElement definitionElement = findDomainSubelement("definition");
    Q_ASSERT(!definitionElement.isNull());

    // get the new definition diagram from definitionDocument
    QDomNode definitionDiagram = definitionDocument.firstChildElement("diagram");
    Q_ASSERT(!definitionDiagram.isNull());

    // first import the new definition diagram to the domain
    QDomNode newDiagram = domainData.importNode(definitionDiagram, true);

    // get the old definition diagram from domain
    QDomNode oldDiagram = definitionElement.firstChildElement("diagram");

    if ( oldDiagram.isNull() )
        oldDiagram = addSubelement(definitionElement, "diagram");

    // replace the old definition diagram with the new definition diagram
    definitionElement.replaceChild(newDiagram, oldDiagram);

    // reset changed for definitionEdit
    ui.definitionEdit->setChanged(false);
}

void KAVIMainWindow::saveDiagram(const QString &parentType, const QString &parentName, const QString &sectionName, const QDomDocument &diagramDocument)
{ 
    // get the new diagram from associated document
    QDomNode savedDiagram = diagramDocument.firstChildElement("diagram");
    Q_ASSERT(!savedDiagram.isNull());

    // first import the new diagram to the domain
    QDomNode newDiagram = domainData.importNode(savedDiagram, true);

    // get the old diagram from domain
    QDomElement sectionElement = findDomainSubelement(sectionName);
    QDomElement parentElement = findSubelementAttr(sectionElement, parentType, "name", parentName);
    QDomNode oldDiagram = parentElement.firstChildElement("diagram");

    if ( oldDiagram.isNull() )
        oldDiagram = addSubelement(parentElement, "diagram");

    // replace the old diagram with the new diagram
    parentElement.replaceChild(newDiagram, oldDiagram);
}

void KAVIMainWindow::globalSetEditMode(EditMode mode)
{
    ui.definitionEdit->setEditMode(mode);
    ui.actionEdit->setEditMode(mode);
    ui.taskEdit->setEditMode(mode);
}

void KAVIMainWindow::registerSubelement(QDomElement sub, QComboBox *listWidget)
{
    Q_ASSERT(!sub.isNull());

    QString subName = sub.attribute("name");
    if ( !subName.isNull() )
    {
        if ( listWidget->findText(subName) < 0 )
        {
            if ( sub.firstChildElement("diagram").isNull() )
                addSubelement(sub, "diagram");

            listWidget->addItem(subName);
            qDebug() << "@" << subName << "registered";
        }
        else
            qWarning() << "$KAVIMainWindow::registerSubelement :" << subName << "is not unique name.";
    }
}

void KAVIMainWindow::reloadClassTree()
{
    if ( classTreeModel != NULL)
        delete classTreeModel;

    QDomElement diagramElement = definitionDocument.firstChildElement("diagram");

    Q_ASSERT(!diagramElement.isNull());

    qDebug() << "$KAVIMainWindow::reloadClassTree : definition diagram";
    GraphClass graphData(diagramElement);
    graphData.init(NST_CLASS, DEP_INHERITANCE, IGRAPH_DIRECTED);

    classTreeModel = new TreeModel(&graphData, this);
    ui.classTreeView->setModel(classTreeModel);
}

void KAVIMainWindow::updatePropertyEditor()
{
    InfoPanel * infoWidget = static_cast<InfoPanel*>(ui.propertyEditorDock->widget());
    infoWidget->updateContent();
}

void KAVIMainWindow::resetPropertyEditor()
{
    qDebug() << "$KAVIMainWindow::resetPropertyEditor";
    InfoPanel * infoWidget = new InfoPanel(ui.propertyEditorDock);

    QWidget * oldWidget = ui.propertyEditorDock->widget();
    ui.propertyEditorDock->setWidget(infoWidget);
    infoWidget->show();

    if ( oldWidget != NULL )
        delete oldWidget;
    else
        qWarning() << "$KAVIMainWindow::resetPropertyEditor : oldWidget == NULL";
}

void KAVIMainWindow::centerContents(QGraphicsView *view)
{
    QRectF	boundingRect = view->scene()->itemsBoundingRect();
    view->centerOn(boundingRect.center());
}
