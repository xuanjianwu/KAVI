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
    KAVIMainWindow(QWidget* parent = 0);

    void appendToLog(bool prefixed, const QString& text, QColor textColor = Qt::black);
    void crash();

public slots:


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
};



#endif // KAVIMAINWINDOW_H
