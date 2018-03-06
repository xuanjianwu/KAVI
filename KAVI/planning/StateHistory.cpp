#include "StateHistory.h"

StateHistory::StateHistory()
{
}

StateHistory::~StateHistory()
{
    clearStateHistory();
}

void StateHistory::initStateHistoryFromPlan(Plan *plan)
{
    clearStateHistory();

    setPlan(plan);

    buildStateHistoryFromPlan();
}

void StateHistory::clearStateHistory()
{
    statesList.clear();
    this->plan = NULL;
}

void StateHistory::setPlan(Plan *plan)
{
    this->plan = plan;
}

void StateHistory::buildStateHistoryFromPlan()
{
    if (this->plan->getPlanSize() <= 0)
    {
        return;
    }
    foreach (PlanAction action, this->plan->getActions()) {
        if (action.getId() == this->plan->getInterruptActionId())
        {
            // when meeting the wrong action, stop build the state history
            break;
        }

        QSet<QString> beforeActionFacts;
        QSet<QString> afterActionFacts;
        QSet<QString> includeFacts;
        QSet<QString> excludeFacts;

        if (statesList.size() ==0 )
        {
            beforeActionFacts.clear();
        }
        else
        {
            State previousState = statesList.last();

            beforeActionFacts = previousState.getAfterActionFacts();

        }

        foreach (QString effect, action.getPositiveEffects()) {
            if (!beforeActionFacts.contains(effect))
            {
                includeFacts.insert(effect);
            }
        }

        foreach (QString effect, action.getNegativeEffects()) {
            if (beforeActionFacts.contains(effect))
            {
                excludeFacts.insert(effect);
            }
        }

        afterActionFacts = beforeActionFacts+includeFacts-excludeFacts;
        State newState;
        newState.setAfterActionFacts(afterActionFacts);
        newState.setBeforeActionFacts(beforeActionFacts);
        newState.setExcludeFacts(excludeFacts);
        newState.setIncludeFacts(includeFacts);

        statesList.append(newState);
    }
}

QList<State> StateHistory::getStatesList() const
{
    return statesList;
}

void StateHistory::setStatesList(const QList<State> &value)
{
    statesList = value;
}
