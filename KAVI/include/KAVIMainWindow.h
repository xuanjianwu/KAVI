/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : main window for KAVI
*/

#ifndef KAVIMAINWINDOW_H
#define KAVIMAINWINDOW_H

#include "KAVIBase.h"
#include "ui_KAVIMainWindow.h"



struct NodeStructure;
struct EdgeStructure;

class TreeModel;
class Convertor;
class QRegExp;

class KAVIMainWindow: public QMainWindow {
    Q_OBJECT

public:
    /*
    * construct function, init the MainWindow
    * @params:
    *       parent - parent object
    * @return: N/A
    */
    KAVIMainWindow(QWidget* parent = 0);

    /*
    * append the messageto the log widget in MainWindow, message with prefix '$' will
    * be not appended to logTextEdit
    * @params:
    *       prefixed  - specify that if message contains the expected prefix or not
    *       text      - the expected text to append
    *       textColor - color of the text
    * @return: void
    */
    void appendToLog(bool prefixed, const QString& text, QColor textColor = Qt::black);

    /*
    * handle the application's crash, write the message in log to CRASHDUMP
    * @params: N/A
    * @return: void
    */
    void crash();

public slots:
    // menubar: File

    void on_actionNew_N_triggered();

    void on_actionOpen_O_triggered();

    void on_actionSave_As_A_triggered();

    void on_actionExit_X_triggered();

    void on_actionDomain_PDDL_triggered();

    void on_actionProblem_PDDL_triggered();


    // menubar: Options

    void on_actionEdit_Properties_P_triggered();

    /*
    * first, globalSave(), summary the classes in the definitionDocument, the same as the domainData.
    * then, summary the predicates in the definitionDocument. then, summary all the operators and problems
    * in the domainData with checking the definitionDocument: checking all the class instances and predicate
    * instances.
    * The difference between on_actionDomain_Summary_S_triggered and on_actionCheckDiagram_triggered:
    * checkDiagram only check the current diagram in actionDocument or taskDocument with the definitionDocument.
    * but, domainSummaru will save all the actions and tasks in actionTab and taskTab to the domainData, then
    * broken them into single diagram, finally, check the single diagram with the definitionDocument like the
    * checkDiagram do.
    * @params: N/A
    * @return: void
    */
    void on_actionDomain_Summary_S_triggered();

    void on_actionClear_Log_C_triggered();


    // menubar: Help

    void on_actionAbout_A_triggered();


    // menubar : Developer
    void on_actionKnowledge_Base_K_triggered();


    // toolbar: edit configure

    void on_actionAddEllipseNode_toggled(bool arg1);

    void on_actionAddRectNode_toggled(bool arg1);

    void on_actionAddEdge_toggled(bool arg1);

    void on_actionDelete_toggled(bool arg1);

    void on_actionCreateAction_triggered();

    void on_actionDeleteAction_triggered();

    void on_actionCreateProblem_triggered();

    void on_actionDeleteProblem_triggered();

    // toolbar: utilities

    /*
    * in definition tab, check the definition itself, classes, and overloaded predicates' argument count.
    * in action tab, check the current diagram in actionDocument with the definitionDocument
    * in task tab, chekc the current diagram in taskDocument with the definitionDocument
    * @params: N/A
    * @return: void
    */
    void on_actionCheckDiagram_triggered();

    void on_actionSavePNG_triggered();


    // tabWidget

    void on_tabWidget_currentChanged(int index);

    // tabWidget: definitionTab

    void on_definitionEdit_sceneChanged(int changeCode);

    void on_definitionEdit_updateInfoPanel(QWidget *infoWidget);

    // tagWidget: actionTab

    /*
    * handle the update of actionDocument according to the domainData.
    * load actionDocument to actionEdit
    * @params:
    *       arg1 - the current selected text
    * @return: void
    */
    void on_actionSelector_currentIndexChanged(const QString &arg1);

    void on_actionEdit_sceneChanged(int changeCode);

    void on_actionEdit_updateInfoPanel(QWidget *infoWidget);

    // tagWidget: taskTab

    /*
    * handle the update of taskDocument according to the domainData.
    * load taskDocument to taskEdit
    * @params:
    *       arg1 - the current selected text
    * @return: void
    */
    void on_taskSelector_currentIndexChanged(const QString &arg1);

    void on_goalCheckBox_toggled(bool checked);

    void on_initCheckBox_toggled(bool checked);

    void on_taskEdit_sceneChanged(int changeCode);

    void on_taskEdit_updateInfoPanel(QWidget *infoWidget);


    // class tree view

    void on_classTreeView_clicked(const QModelIndex &index);

private:
    // specified the domain changed or not
    bool domainChanged;

    // the common name checker
    QRegExp nameChecker;

    // the name of recent action
    QString recentActionName;

    // the name of recent task
    QString recentTaskName;

    // the data convertor instance
    Convertor* dataConvertor;

    // the domain data document
    QDomDocument domainData;

    // the current editing definition document
    QDomDocument definitionDocument;

    // the current editing action document
    QDomDocument actionDocument;

    // the current editing task document
    QDomDocument taskDocument;

    // the current action
    QDomNode currentAction;

    // the current task
    QDomNode currentTask;

    void updateDocument(QDomElement& rootEl, QDomDocument& updatedDoc);

    /*
    * create an empty domain document with the basic structure
    * @params: N/A
    * @return: the empty domain document
    */
    QDomDocument createEmptyDomain();

    /*
    * init the current editing domain with specified domainDocument.
    * set the domainData, load definition data to definitionEdit, update definitionDocument,
    * set the dependent definition pointer for the actionEdit and taskEdit.
    * register the operators and problems to the actionSelector and taskSelector, but not now
    * update the actionDocument and taskDocument.
    * reset the Property Editor.
    * reset domainChanged = false.
    * @params:
    *       domainDocument - the specified domain document to init
    * @return: void
    */
    void initDomain(QDomDocument domainDocument);

    /*
    * get the subelement of domain document with the specified tag
    * @params:
    *       tagName - the specified tag name
    * @return: the expected subelement
    */
    QDomElement findDomainSubelement(const QString& tagName);

    /*
    * init the definition depends on the domain, update the definitionDocument,
    * set the DefinitionEdit, DataWidget, DiagramWidget
    * @params:
    *       diagramElement - the specified diagram to init
    * @return: void
    */
    void initDefinition(QDomElement diagramElement);

    /*
    * register the operators to the actionSelector, now not update actionDocument
    * @params: N/A
    * @return: void
    */
    void initOperators();

    /*
    * register the problems to the taskSelector, now not update taskDocument
    * @params: N/A
    * @return: void
    */
    void initProblems();

    /*
    * ask for save current domain, if application is allowed to run next step, return true
    * @params:
    *       text - the text title of messgaebox
    * @return: domain not changed - true
    *          select to save     - true
    *          select no save     - true
    *          select cancel      - false
    */
    bool askForSave(const QString& text);

    /*
    * save the all, including definition, editing action diagram, editing task diagram
    * @params: N/A
    * @return: void
    */
    void globalSave();

    /*
    * save the current definition in definitionDocument to domain: replace the old one directly
    * @params: N/A
    * @return: void
    */
    void saveDefinition();

    /*
    * save the current diagram(action or task) in actionDocument or taskDocument to domain:
    * replace the old one directly
    * @params:
    *       parentType        - the tag name of the diagram's parent element
    *       parentName        - the attribute name of the diagram's parent element
    *       sectionName       - the section name(operators or problems) of the parent
    *       diagramDocument   - the new saved diagram document
    * @return: void
    */
    void saveDiagram(const QString& parentType, const QString& parentName,
                const QString& sectionName, const QDomDocument& diagramDocument);

    /*
    * set the global edit mode for the definitionEdit, actionEdit, taskEdit
    * @params:
    *       mode - the expected edit mode
    * @return: void
    */
    void globalSetEditMode(EditMode mode);

    /*
    * add the subelment(action or task) to the associated document and update the action
    * or task selector combobox
    * @params:
    *       sub        - the specified subelement
    *       listWidget - the specified combobox
    * @return: void
    */
    void registerSubelement(QDomElement sub, QComboBox* listWidget);

    // the tree model for the class tree view
    TreeModel* classTreeModel;

    /*
    * reload the class tree view
    * @params: N/A
    * @return: void
    */
    void reloadClassTree();

    /*
    * update property editor, call the associated InfoPanle.updateContent()
    * @params: N/A
    * @return: void
    */
    void updatePropertyEditor();

    /*
    * reset the property editor
    * @params: N/A
    * @return: void
    */
    void resetPropertyEditor();

    /*
    * center the bounding rect of all items on the scene associated with view.
    * center the center of all tiems in current view.
    * @params:
    *       view - the specified view
    * @return: void
    */
    void centerContents(QGraphicsView* view);

    Ui::MainWindow ui;
};



#endif // KAVIMAINWINDOW_H
