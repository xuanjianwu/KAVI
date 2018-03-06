/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : the class handle the validation of the plan
*/

#ifndef PLANVALIDATOR_H
#define PLANVALIDATOR_H

#include "KAVIBase.h"
#include "XMLUtils.h"

#include "StateHistory.h"
#include "Plan.h"

class PlanValidator {
public:
    PlanValidator();
    PlanValidator(QDomElement chosenValidator, QString domainFile, QString problemFile, QString planFile);

    void buildDependenceBetweenPlanAction();

    void initEnvironment();
    void initPlan();
    void initStateHistory();
    void buildStateHistoryFromPlan();

    void setInitPlanAction(PlanAction &action);
    void appendEffectsToInitPlanAction(PlanAction &action, QString initString);

    void setGoalPlanAction(PlanAction &action);
    void appendPreconditionsToGoalPlanAction(PlanAction &action, QString goalString);

    void initDomainActions();

    void appendPreconditionsToPlanAction(PlanAction& planAction, QString domainAction);

    void appendEffectsToPlanAction(PlanAction& planAction, QString domainAction);

    void matchPlanActionWithDomain(PlanAction& action);

    void selectMatchedActionFromDomainActions(QString actionName, QStringList& domainActions, QString &targetDomainAction);

    void splitDomainActionsToString(QString domainFile, QStringList& actions);

    void splitProblemInitToString(QString problemFile, QString &initString);

    void splitProblemGoalToString(QString problemFile, QString &goalString);

    QString getContentsAsString(QFile& file);
    QStringList getContentsAsStringList(QFile& file);

    void getValidatorOutput(QDomElement chosenValidator, QString domain, QString problem, QString plan, QStringList& consoleOutput);

    void parseValidatorOutputToPlan(QStringList& consoleOutput);

    QString getValidatorsPath();

    QString getDomainFile() const;
    void setDomainFile(const QString &value);

    QString getProblemFile() const;
    void setProblemFile(const QString &value);

    QString getPlanFile() const;
    void setPlanFile(const QString &value);

    QString getDomainName() const;
    void setDomainName(const QString &value);

    QString getProblemName() const;
    void setProblemName(const QString &value);

    QDomElement getChosenValidator() const;
    void setChosenValidator(const QDomElement &value);

    void setPlanSize(int value);
    int getPlanSize() const;

    void run();

    Plan *getPlan() const;

    StateHistory *getStateHistory();

    void solveProblem(QDomElement chosenValidator, QString domain, QString problem, QString plan);

    void resetPlanValidator();

    void initPlanValidator(QDomElement chosenValidator, QString domain, QString problem, QString plan);
private:
    double time = 0;
    QString toolMessage;
    QProcess* process;
    bool gotError;
    bool readyReadOutput;
    bool normalExit;

    int planSize;

    QDomElement chosenValidator;
    QString domainFile;
    QString problemFile;
    QString planFile;

    QString domainName;
    QString problemName;

    QString problemInit;
    QString problemGoal;
    QStringList domainActions;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;

    Plan *plan;
    StateHistory *stateHistory;
    bool planSuccess;
};

#endif // PLANVALIDATOR_H
