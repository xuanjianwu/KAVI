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

    /*
     * construct function
     * @params:
     *      chosenValidator - the chosen validator to execute plan validation
     *      domainFile      - domain PDDL file
     *      problemFile     - problem PDDL file
     *      planFile        - plan txt file
     * @return: N/A
    */
    PlanValidator(QDomElement chosenValidator, QString domainFile, QString problemFile, QString planFile);

    /*
     * build the dependence between plan actions
     * @params: N/A
     * @return: N/A
    */
    void buildDependenceBetweenPlanAction();

    /*
     * init environment of plan validation
     * @params: N/A
     * @return: N/A
    */
    void initEnvironment();
    void initPlan();
    void initStateHistory();

    /*
     * build state history from plan
     * @params: N/A
     * @return: N/A
    */
    void buildStateHistoryFromPlan();

    void setInitPlanAction(PlanAction &action);
    void appendEffectsToInitPlanAction(PlanAction &action, QString initString);

    void setGoalPlanAction(PlanAction &action);
    void appendPreconditionsToGoalPlanAction(PlanAction &action, QString goalString);

    void initDomainActions();
    void buildDomainPlanActionsFromDomainActions(QList<PlanAction> &domainPlanActions, QStringList &domainActions);

    void appendPreconditionsToPlanAction(PlanAction& planAction, QString domainAction);

    void appendEffectsToDomainPlanAction(PlanAction& planAction, QString& domainAction);

    void appendEffectsToPlanAction(PlanAction& planAction, QString domainAction);

    void matchPlanActionWithDomain(PlanAction& action);

    void selectMatchedActionFromDomainActions(QString actionName, QStringList& domainActions, QString &targetDomainAction);

    void splitDomainActionsToString(QString domainFile, QStringList& actions);

    void splitProblemInitToString(QString problemFile, QString &initString);

    void splitProblemGoalToString(QString problemFile, QString &goalString);

    QString getContentsAsString(QFile& file);
    QStringList getContentsAsStringList(QFile& file);

    /*
     * execute plan validation, get the output of plan validator
     * @params:
     *      chosenValidator - the chosen validator to execute plan validation
     *      domain          - domain PDDL file
     *      problem         - problem PDDL file
     *      plan            - plan txt file
     *      consoleOutput   - console output of plan validator
     * @return: N/A
    */
    void getValidatorOutput(QDomElement chosenValidator, QString domain, QString problem, QString plan, QStringList& consoleOutput);

    /*
     * parse validator's output to plan
     * @params:
     *      consoleOutput - console output of plan validator
     * @return: N/A
    */
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

    /*
     * solve plan validation
     * @params:
     *      chosenValidator - the chosen validator to execute plan validation
     *      domain          - domain PDDL file
     *      problem         - problem PDDL file
     *      plan            - plan txt file
     * @return: N/A
    */
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

    // the chosen validator's infomation in xml format
    QDomElement chosenValidator;

    // domain PDDL file
    QString domainFile;

    // problem PDDL file
    QString problemFile;

    // plan txt file
    QString planFile;

    // domain's name
    QString domainName;

    // problem's name
    QString problemName;

    QString problemInit;
    QString problemGoal;
    QStringList domainActions;
    QList<PlanAction> domainPlanActions;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;

    // the associated plan
    Plan *plan;

    // the associated state history
    StateHistory *stateHistory;
    bool planSuccess;
};

#endif // PLANVALIDATOR_H
