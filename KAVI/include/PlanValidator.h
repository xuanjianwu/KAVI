/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : the class handle the validation of the plan
*/

#ifndef PLANVALIDATOR_H
#define PLANVALIDATOR_H

#include "KAVIBase.h"
#include "XMLUtils.h"

class PlanValidator {
public:
    PlanValidator();
    PlanValidator(QDomElement chosenValidator, QString domainFile, QString problemFile, QString planFile);

    QString getContentsAsString(QFile& file);
    QStringList getContentsAsStringList(QFile& file);

    QStringList getValidatorOutput(QDomElement chosenValidator, QString domain, QString problem, QString plan, QStringList& consoleOutput);

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

private:
    double time = 0;
    QString toolMessage;
    QProcess* process;
    bool gotError;
    bool readyReadOutput;
    bool normalExit;

    QDomElement chosenValidator;
    QString domainFile;
    QString problemFile;
    QString planFile;

    QString domainName;
    QString problemName;

    // the running mode of KAVI: Debug or Release
    RunMode KAVIRunMode;
};

#endif // PLANVALIDATOR_H
