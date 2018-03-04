#include "State.h"

State::State(QSet<QString> facts)
{
    setFacts(facts);
}

void State::changeState()
{
    foreach (QString fact, excludeFacts) {
        if (facts.contains(fact))
        {
            facts.remove(fact);
        }
    }

    foreach (QString fact, includeFacts) {
        facts.insert(fact);
    }
}

QSet<QString> State::getFacts() const
{
    return facts;
}

void State::setFacts(const QSet<QString> &value)
{
    facts = value;
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
