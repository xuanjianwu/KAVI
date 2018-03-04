/*
 * @author: liYc
 * @date  : 2018/3/4
 * @brief : This class implements a plan. For each action we store a unique
 *          action ID - implemented as HashMap<Integer,PlanAction>
*/

#ifndef PLAN_H
#define PLAN_H

#include "KAVIBase.h"
#include "PlanAction.h"

class Plan {
public:
    Plan();

    void addAction(PlanAction action);
    bool removeAction(int actionId);

    int getActionId(PlanAction action) const;
    PlanAction getAction(int actionId) const;

    int getActionId() const;
    void setActionId(int value);

    int getPlanSize() const;

    QMap<int, PlanAction> getActions() const;
    void setActions(const QMap<int, PlanAction> &value);

private:
    int actionId;
    QMap<int, PlanAction> actions;


};

#endif // PLAN_H
