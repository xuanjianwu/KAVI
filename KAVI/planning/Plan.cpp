#include "Plan.h"


Plan::Plan()
{
    actionId = 0;
}

void Plan::addAction(PlanAction action)
{
    int newId = actionId++;
    action.setId(newId);
    this->actions.insert(newId, action);
}

void Plan::insertAction(int actionId, PlanAction action)
{
    this->actions.insert(actionId, action);
}

bool Plan::removeAction(int actionId)
{
    if (this->actions.remove(actionId) == 1) {
        return true;
    }
    else
    {
        return false;
    }
}

int Plan::getActionId(PlanAction action) const
{
    //return this->actions.key(action);
    return action.getId();
}

PlanAction Plan::getAction(int actionId) const
{
    return this->actions.value(actionId);
}

int Plan::getActionId() const
{
    return actionId;
}

void Plan::setActionId(int value)
{
    actionId = value;
}

int Plan::getPlanSize() const
{
    return this->actions.size();
}

QMap<int, PlanAction> Plan::getActions() const
{
    return actions;
}

void Plan::setActions(const QMap<int, PlanAction> &value)
{
    actions = value;
}
