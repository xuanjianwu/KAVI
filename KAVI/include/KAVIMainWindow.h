/*
 * @author: liYc
 * @date  : 2018/1/25
 * @brief : main window for KAVI
*/

#ifndef KAVIMAINWINDOW_H
#define KAVIMAINWINDOW_H

#include "KAVIBase.h"
#include "ui_KAVIMainWindow.h"

KAVI_NS_BEGIN

struct NodeStructure;
struct EdgeStructure;

class TreeModel;
class Convertor;
class QRegExp;

class KAVIMainWindow: public QMainWindow {
    Q_OBJECT

public:
    KAVIMainWindow(QWidget* parent = 0);

    void appendToLog(bool prefixed, const QString& text, QColor textColor = Qt::black);
    void crash();

public slots:
    // Menu File
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();

    void on_actionProperties_triggered();
    void on_actionExportDomain_triggered();
    void on_actionExportProblems_triggered();

    // Menu Tools
    void on_actionClearLog_triggered();
    void on_actionDomainSummary_triggered();

    // Toolbar
    void on_actionAddRectNode_toggled(bool checked);
    void on_actionAddEllipseNode_toggled(bool checked);
    void on_actionAddEdge_toggled(bool checked);
    void on_actionDelete_toggled(bool checked);

    void on_createAction_triggered();
    void on_deleteAction_triggered();

    void on_createProblem_triggered();
    void on_deleteProblem_triggered();

    void on_actionSavePNG_triggered();
    void on_actionCheckDiagram_triggered();

    // Other
    void on_tabWidget_currentChanged(int index);

    void on_actionSelector_currentIndexChanged(const QString& text);
    void on_taskSelector_currentIndexChanged(const QString& text);

    void on_initCheckBox_toggled(bool checked);
    void on_goalCheckBox_toggled(bool checked);

    void on_definitionEdit_sceneChanged(int changeCode);
    void on_actionEdit_sceneChanged(int changeCode);
    void on_taskEdit_sceneChanged(int changeCode);

    void on_definitionEdit_updateInfoPanel(QWidget* infoWidget);
    void on_actionEdit_updateInfoPanel(QWidget* infoWidget);
    void on_taskEdit_updateInfoPanel(QWidget* infoWidget);

    void on_classTreeView_clicked(const QModelIndex& index);

private:
    bool domainChanged;
    QRegExp nameChecker;

    QString recentActionName;
    QString recentTaskName;

    Convertor* dataConvertor;
    QDomDocument domainData;

    QDomDocument definitionDocument;
    QDomDocument actionDocument;
    QDomDocument taskDocument;

    QDomNode currentAction;
    QDomNode currentTask;

    void updateDocument(QDomElement& rootEl, QDomDocument& updatedDoc);
    QDomDocument createEmptyDomain();
    void initDomain(QDomDocument domainDocument);

    QDomElement findDomainSubelement(const QString& tagName);

    void initDefinition(QDomElement diagramElement);
    void initOperators();
    void initProblems();

    bool askForSave(const QString& text);
    void globalSave();
    void saveDefinition();

    void saveDiagram(const QString& parentType, const QString& parentName,
                const QString& sectionName, const QDomDocument& diagramDocument);

    void globalSetEditMode(EditMode mode);

    void registerSubelement(QDomElement sub, QComboBox* listWidget);

    TreeModel* classTreeModel;
    void reloadClassTree();
    void updatePropertyEditor();
    void resetPropertyEditor();
    void centerContents(QGraphicsView* view);

    Ui::MainWindow ui;
}

KAVI_NS_END

#endif // KAVIMAINWINDOW_H
