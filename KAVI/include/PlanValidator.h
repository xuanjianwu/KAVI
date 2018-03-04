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

    void initEnvironment();
    void initPlan();

    void matchPlanActionWithDomain(PlanAction& action, QString domainFile);

    void splitDomainActionsToString(QString domainFile, QStringList& actions);

    QString getContentsAsString(QFile& file);
    QStringList getContentsAsStringList(QFile& file);

    QStringList getValidatorOutput(QDomElement chosenValidator, QString domain, QString problem, QString plan, QStringList& consoleOutput);

    void parseValidatorOutput(QStringList& consoleOutput);

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

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;

    Plan *plan;
    StateHistory *stateHistory;
};

#endif // PLANVALIDATOR_H
