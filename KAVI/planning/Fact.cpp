#include "Fact.h"


Fact::Fact(QString formula, int id)
{
    setFormula(formula);
    setId(id);
}

QString Fact::getFormula() const
{
    return formula;
}

void Fact::setFormula(const QString &value)
{
    formula = value;
}

int Fact::getId() const
{
    return id;
}

void Fact::setId(int value)
{
    id = value;
}
