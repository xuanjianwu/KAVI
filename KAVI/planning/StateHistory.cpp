#include "StateHistory.h"

StateHistory::StateHistory()
{

}

QList<State> StateHistory::getStatesList() const
{
    return statesList;
}

void StateHistory::setStatesList(const QList<State> &value)
{
    statesList = value;
}
