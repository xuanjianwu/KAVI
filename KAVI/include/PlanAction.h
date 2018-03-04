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

private:
    QString formula;
    double time;
    int id;

    QSet<QString> positiveEffects;
    QSet<QString> negativeEffects;
};

#endif // PLANACTION_H
