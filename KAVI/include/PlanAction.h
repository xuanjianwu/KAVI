/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : This class implements an action from plan. Save the states before and after the
 *          action, and the action's positive effects, negative effects of the world state
*/

#ifndef PLANACTION_H
#define PLANACTION_H

#include "KAVIBase.h"

class PlanAction{
public:
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

private:
    QString formula;
    double time;
    int id;

    QSet<QString> positivePreconditions;
    QSet<QString> negativePreconditions;
    QSet<QString> positiveEffects;
    QSet<QString> negativeEffects;

    QMap<QString, bool> repairAdvice;

    QMap<int, QSet<QString> > positivePreconditionsDependers;
    QMap<int, QSet<QString> > negativePreconditionsDependers;

    QMap<int, QSet<QString> > positiveEffectsDependers;
    QMap<int, QSet<QString> > negativeEffectsDependers;
};

#endif // PLANACTION_H
