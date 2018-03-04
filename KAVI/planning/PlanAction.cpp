#include "PlanAction.h"


PlanAction::PlanAction()
{

}

QString PlanAction::getFormula() const
{
    return formula;
}

void PlanAction::setFormula(const QString &value)
{
    formula = value;
}

double PlanAction::getTime() const
{
    return time;
}

void PlanAction::setTime(double value)
{
    time = value;
}

int PlanAction::getId() const
{
    return id;
}

void PlanAction::setId(int value)
{
    id = value;
}

QSet<QString> PlanAction::getPositiveEffects() const
{
    return positiveEffects;
}

void PlanAction::setPositiveEffects(const QSet<QString> &value)
{
    positiveEffects = value;
}

QSet<QString> PlanAction::getNegativeEffects() const
{
    return negativeEffects;
}

void PlanAction::setNegativeEffects(const QSet<QString> &value)
{
    negativeEffects = value;
}
