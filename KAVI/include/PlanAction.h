/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : This class implements an action from plan. Save the action's preconditions, effects and so on.
*/

#ifndef PLANACTION_H
#define PLANACTION_H

#include "KAVIBase.h"

class PlanAction{
public:
    /*
     * construct function
     * @params: N/A
     * @return: N/A
    */
    PlanAction();

    QString getFormula() const;
    void setFormula(const QString &value);

    double getTime() const;
    void setTime(double value);

    int getId() const;
    void setId(int value);

    QSet<QString> getPositiveEffects() const;
    void setPositiveEffects(const QSet<QString> &value);

    QSet<QString> getNegativeEffects() const;
    void setNegativeEffects(const QSet<QString> &value);

    QSet<QString> getPositivePreconditions() const;
    void setPositivePreconditions(const QSet<QString> &value);

    QSet<QString> getNegativePreconditions() const;
    void setNegativePreconditions(const QSet<QString> &value);

    QMap<QString, bool> getRepairAdvice() const;
    void setRepairAdvice(const QMap<QString, bool> &value);

    /*
     * add repair advice to the action
     * @params:
     *      precondition - the target precondition
     *      advice       - the bool advice for the precondition
     * @return: N/A
    */
    void addRepairAdvice(const QString precondition, const bool advice);
    int getRepairAdviceSize() const;

    QMap<int, QSet<QString> > getPositivePreconditionsDependers() const;
    void setPositivePreconditionsDependers(const QMap<int, QSet<QString> > &value);

    QMap<int, QSet<QString> > getNegativePreconditionsDependers() const;
    void setNegativePreconditionsDependers(const QMap<int, QSet<QString> > &value);

    QMap<int, QSet<QString> > getPositiveEffectsDependers() const;
    void setPositiveEffectsDependers(const QMap<int, QSet<QString> > &value);

    QMap<int, QSet<QString> > getNegativeEffectsDependers() const;
    void setNegativeEffectsDependers(const QMap<int, QSet<QString> > &value);

    QString getActioName() const;
    void setActioName(const QString &value);

    QMap<QString, QString> getArgumentTypePair() const;
    void setArgumentTypePair(const QMap<QString, QString> &value);

private:
    // the formula of the action
    QString formula;

    // the time of the action
    double time;

    // action's id
    int id;

    // action's name
    QString actioName;

    // action's [argument, type] pairs
    QMap<QString, QString> argumentTypePair;

    // positive preconditions
    QSet<QString> positivePreconditions;

    // negative preconditions
    QSet<QString> negativePreconditions;

    // positive effects
    QSet<QString> positiveEffects;

    // negative effects
    QSet<QString> negativeEffects;

    // repair advice against the action
    QMap<QString, bool> repairAdvice;

    // positive preconditions dependers
    QMap<int, QSet<QString> > positivePreconditionsDependers;

    // negative preconditions dependers
    QMap<int, QSet<QString> > negativePreconditionsDependers;

    // positive effects dependers
    QMap<int, QSet<QString> > positiveEffectsDependers;

    // negative effects dependers
    QMap<int, QSet<QString> > negativeEffectsDependers;
};

#endif // PLANACTION_H
