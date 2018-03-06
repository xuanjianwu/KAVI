#include "State.h"

State::State()
{
}

QSet<QString> State::getIncludeFacts() const
{
    return includeFacts;
}

void State::setIncludeFacts(const QSet<QString> &value)
{
    includeFacts = value;
}

QSet<QString> State::getExcludeFacts() const
{
    return excludeFacts;
}

void State::setExcludeFacts(const QSet<QString> &value)
{
    excludeFacts = value;
}

QSet<QString> State::getBeforeActionFacts() const
{
    return beforeActionFacts;
}

void State::setBeforeActionFacts(const QSet<QString> &value)
{
    beforeActionFacts = value;
}

QSet<QString> State::getAfterActionFacts() const
{
    return afterActionFacts;
}

void State::setAfterActionFacts(const QSet<QString> &value)
{
    afterActionFacts = value;
}
