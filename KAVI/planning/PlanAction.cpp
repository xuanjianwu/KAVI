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

QSet<QString> PlanAction::getPositivePreconditions() const
{
    return positivePreconditions;
}

void PlanAction::setPositivePreconditions(const QSet<QString> &value)
{
    positivePreconditions = value;
}

QSet<QString> PlanAction::getNegativePreconditions() const
{
    return negativePreconditions;
}

void PlanAction::setNegativePreconditions(const QSet<QString> &value)
{
    negativePreconditions = value;
}

QMap<QString, bool> PlanAction::getRepairAdvice() const
{
    return repairAdvice;
}

void PlanAction::setRepairAdvice(const QMap<QString, bool> &value)
{
    repairAdvice = value;
}

void PlanAction::addRepairAdvice(const QString precondition, const bool advice)
{
    repairAdvice.insert(precondition, advice);
}

int PlanAction::getRepairAdviceSize() const
{
    return repairAdvice.size();
}

QMap<int, QSet<QString> > PlanAction::getPositivePreconditionsDependers() const
{
    return positivePreconditionsDependers;
}

void PlanAction::setPositivePreconditionsDependers(const QMap<int, QSet<QString> > &value)
{
    positivePreconditionsDependers = value;
}

QMap<int, QSet<QString> > PlanAction::getNegativePreconditionsDependers() const
{
    return negativePreconditionsDependers;
}

void PlanAction::setNegativePreconditionsDependers(const QMap<int, QSet<QString> > &value)
{
    negativePreconditionsDependers = value;
}

QMap<int, QSet<QString> > PlanAction::getPositiveEffectsDependers() const
{
    return positiveEffectsDependers;
}

void PlanAction::setPositiveEffectsDependers(const QMap<int, QSet<QString> > &value)
{
    positiveEffectsDependers = value;
}

QMap<int, QSet<QString> > PlanAction::getNegativeEffectsDependers() const
{
    return negativeEffectsDependers;
}

void PlanAction::setNegativeEffectsDependers(const QMap<int, QSet<QString> > &value)
{
    negativeEffectsDependers = value;
}

QString PlanAction::getActioName() const
{
    return actioName;
}

void PlanAction::setActioName(const QString &value)
{
    actioName = value;
}

QMap<QString, QString> PlanAction::getArgumentTypePair() const
{
    return argumentTypePair;
}

void PlanAction::setArgumentTypePair(const QMap<QString, QString> &value)
{
    argumentTypePair = value;
}
