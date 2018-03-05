#include "PlanValidator.h"

using namespace XMLUtils;

PlanValidator::PlanValidator()
{
    KAVIRunMode = Debug;

    initEnvironment();
}

PlanValidator::PlanValidator(QDomElement chosenValidator, QString domainFile, QString problemFile, QString planFile)
{
    KAVIRunMode = Debug;

    this->chosenValidator = chosenValidator;
    this->domainFile = domainFile;
    this->problemFile = problemFile;
    this->planFile = planFile;

    initEnvironment();
}

void PlanValidator::buildDependenceBetweenPlanAction()
{
    if (this->plan->getPlanSize() > 0)
    {
        for (int i = 0; i < this->plan->getPlanSize(); i++)
        {
            PlanAction currentAction = this->plan->getAction(i);

            QMap<int, QSet<QString> > positivePreconditionsDependers;
            QMap<int, QSet<QString> > negativePreconditionsDependers;

            QMap<int, QSet<QString> > positiveEffectsDependers;
            QMap<int, QSet<QString> > negativeEffectsDependers;

            // build the positive preconditions dependers

            QSet<QString> discardedPreconditions;
            for (int j = i-1; j >= 0; j--)
            {
                PlanAction previousAction = this->plan->getAction(j);

                foreach (QString positivePrecondition, currentAction.getPositivePreconditions()) {
                    if (previousAction.getNegativeEffects().contains(positivePrecondition))
                    {
                        discardedPreconditions.insert(positivePrecondition);
                        continue;
                    }

                    if (!discardedPreconditions.contains(positivePrecondition) && previousAction.getPositiveEffects().contains(positivePrecondition))
                    {
                        if (!positivePreconditionsDependers.contains(j))
                        {
                            QSet<QString> tmpSet;
                            tmpSet.insert(positivePrecondition);
                            positivePreconditionsDependers.insert(j, tmpSet);
                        }
                        else
                        {
                            QSet<QString> tmpSet = positivePreconditionsDependers.value(j);
                            tmpSet.insert(positivePrecondition);
                            positivePreconditionsDependers.insert(j, tmpSet);
                        }
                        //break;
                    }
                }
            }


            // build the negative preconditions dependers

            discardedPreconditions.clear();
            for (int j = i-1; j >= 0; j--)
            {
                PlanAction previousAction = this->plan->getAction(j);

                foreach (QString negativePrecondition, currentAction.getNegativePreconditions()) {
                    if (previousAction.getPositiveEffects().contains(negativePrecondition))
                    {
                        discardedPreconditions.insert(negativePrecondition);
                        continue;
                    }

                    if (!discardedPreconditions.contains(negativePrecondition) && previousAction.getNegativeEffects().contains(negativePrecondition))
                    {
                        if (!negativePreconditionsDependers.contains(j))
                        {
                            QSet<QString> tmpSet;
                            tmpSet.insert(negativePrecondition);
                            negativePreconditionsDependers.insert(j, tmpSet);
                        }
                        else
                        {
                            QSet<QString> tmpSet = negativePreconditionsDependers.value(j);
                            tmpSet.insert(negativePrecondition);
                            negativePreconditionsDependers.insert(j, tmpSet);
                        }
                        //break;
                    }
                }
            }


            // build the positive effects dependers

            QSet<QString> discardedEffects;
            for (int j = i+1; j < this->plan->getPlanSize(); j++)
            {
                PlanAction nextAction = this->plan->getAction(j);

                foreach (QString positiveEffect, currentAction.getPositiveEffects()) {
                    if (!discardedEffects.contains(positiveEffect) && nextAction.getPositivePreconditions().contains(positiveEffect))
                    {
                        if (!positiveEffectsDependers.contains(j))
                        {
                            QSet<QString> tmpSet;
                            tmpSet.insert(positiveEffect);
                            positiveEffectsDependers.insert(j, tmpSet);
                        }
                        else
                        {
                            QSet<QString> tmpSet = positiveEffectsDependers.value(j);
                            tmpSet.insert(positiveEffect);
                            positiveEffectsDependers.insert(j, tmpSet);
                        }
                        //break;
                    }

                    if (nextAction.getNegativeEffects().contains(positiveEffect))
                    {
                        discardedEffects.insert(positiveEffect);
                    }
                }
            }


            // build the negative effects dependers

            discardedEffects.clear();
            for (int j = i+1; j < this->plan->getPlanSize(); j++)
            {
                PlanAction nextAction = this->plan->getAction(j);

                foreach (QString negativeEffect, currentAction.getNegativeEffects()) {
                    if (!discardedEffects.contains(negativeEffect) && nextAction.getNegativePreconditions().contains(negativeEffect))
                    {
                        if (!negativeEffectsDependers.contains(j))
                        {
                            QSet<QString> tmpSet;
                            tmpSet.insert(negativeEffect);
                            negativeEffectsDependers.insert(j, tmpSet);
                        }
                        else
                        {
                            QSet<QString> tmpSet = negativeEffectsDependers.value(j);
                            tmpSet.insert(negativeEffect);
                            negativeEffectsDependers.insert(j, tmpSet);
                        }
                        //break;
                    }

                    if (nextAction.getPositiveEffects().contains(negativeEffect))
                    {
                        discardedEffects.insert(negativeEffect);
                    }
                }
            }


            currentAction.setPositivePreconditionsDependers(positivePreconditionsDependers);
            currentAction.setNegativePreconditionsDependers(negativePreconditionsDependers);
            currentAction.setPositiveEffectsDependers(positiveEffectsDependers);
            currentAction.setNegativeEffectsDependers(negativeEffectsDependers);

            this->plan->insertAction(currentAction.getId(), currentAction);
        }
    }
}

void PlanValidator::initEnvironment()
{
    initPlan();
    stateHistory = new StateHistory();

    initDomainActions();
}

void PlanValidator::initPlan()
{
    plan = new Plan();
}

void PlanValidator::setInitPlanAction(PlanAction &action)
{
    QFile file(problemFile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        return;
    }
    QString content(file.readAll());
    file.seek(0);
    file.close();
    action.setFormula("(init)");

    splitProblemInitToString(content, problemInit);
    appendEffectsToInitPlanAction(action, problemInit);
}

void PlanValidator::appendEffectsToInitPlanAction(PlanAction &action, QString initString)
{
    // select all predicates including positive and negative
    QRegExp predicateChecker;
    predicateChecker.setPattern(QString("\\([^()]*\\)"));
    int pos = 0;
    QStringList predicateList;
    while ((pos = predicateChecker.indexIn(initString, pos)) != -1)
    {
        predicateList.append(predicateChecker.capturedTexts());
        pos += predicateChecker.matchedLength();
    }

    // select negative predicates
    QRegExp negativePredicateChecker;
    negativePredicateChecker.setPattern(QString("not[\\s]*\\([^()]*\\)"));
    pos = 0;
    QStringList negativePredicateList;
    while ((pos = negativePredicateChecker.indexIn(initString, pos)) != -1)
    {
        negativePredicateList.append(negativePredicateChecker.capturedTexts());
        pos += negativePredicateChecker.matchedLength();
    }
    pos = 0;
    QStringList negativePurePredicateList;
    for (int i = 0; i < negativePredicateList.size(); i++)
    {
        QString tmpStr = negativePredicateList.at(i);
        predicateChecker.indexIn(tmpStr, pos);
        negativePurePredicateList.append(predicateChecker.cap(0).simplified().toLower());
    }
    // select positive predicates
    QStringList positivePurePredicateList;
    foreach (QString predicate, predicateList) {
        if (!negativePurePredicateList.contains(predicate))
        {
            // remove the "?" before parameters
            //predicate = predicate.replace("?", "");
            positivePurePredicateList.append(predicate.toLower());
        }
    }
    action.setPositiveEffects(positivePurePredicateList.toSet());
    action.setNegativeEffects(negativePurePredicateList.toSet());
}

void PlanValidator::setGoalPlanAction(PlanAction &action)
{
    QFile file(problemFile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        return;
    }
    QString content(file.readAll());
    file.seek(0);
    file.close();
    action.setFormula("(goal)");

    splitProblemGoalToString(content, problemGoal);
    appendPreconditionsToGoalPlanAction(action, problemGoal);
}

void PlanValidator::appendPreconditionsToGoalPlanAction(PlanAction &action, QString goalString)
{
    // select all predicates including positive and negative
    QRegExp predicateChecker;
    predicateChecker.setPattern(QString("\\([^()]*\\)"));
    int pos = 0;
    QStringList predicateList;
    while ((pos = predicateChecker.indexIn(goalString, pos)) != -1)
    {
        predicateList.append(predicateChecker.capturedTexts());
        pos += predicateChecker.matchedLength();
    }

    // select negative predicates
    QRegExp negativePredicateChecker;
    negativePredicateChecker.setPattern(QString("not[\\s]*\\([^()]*\\)"));
    pos = 0;
    QStringList negativePredicateList;
    while ((pos = negativePredicateChecker.indexIn(goalString, pos)) != -1)
    {
        negativePredicateList.append(negativePredicateChecker.capturedTexts());
        pos += negativePredicateChecker.matchedLength();
    }
    pos = 0;
    QStringList negativePurePredicateList;
    for (int i = 0; i < negativePredicateList.size(); i++)
    {
        QString tmpStr = negativePredicateList.at(i);
        predicateChecker.indexIn(tmpStr, pos);
        negativePurePredicateList.append(predicateChecker.cap(0).simplified().toLower());
    }
    // select positive predicates
    QStringList positivePurePredicateList;
    foreach (QString predicate, predicateList) {
        if (!negativePurePredicateList.contains(predicate))
        {
            // remove the "?" before parameters
            //predicate = predicate.replace("?", "");
            positivePurePredicateList.append(predicate.toLower());
        }
    }
    action.setPositivePreconditions(positivePurePredicateList.toSet());
    action.setNegativePreconditions(negativePurePredicateList.toSet());
}

void PlanValidator::initDomainActions()
{
    QFile file(domainFile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        return;
    }
    QString content(file.readAll());
    file.seek(0);
    file.close();

    //content.replace("\n", " ");
    splitDomainActionsToString(content, domainActions);
}

void PlanValidator::appendPreconditionsToPlanAction(PlanAction &planAction, QString domainAction)
{
    // this method append the preconditions of the matched domain action to the plan action


    // get the action name and action parameters of the plan action
    QString actionName;
    QStringList actionParameters;

    QString actionFormula = planAction.getFormula();
    actionFormula.remove(actionFormula.indexOf("("), 1);
    actionFormula.remove(actionFormula.indexOf(")"), 1);
    actionFormula = actionFormula.simplified();
    QStringList actionItems = actionFormula.split(" ");
    for (int i = 0; i < actionItems.size(); i++)
    {
        if (i == 0)
        {
            actionName = actionItems.at(i);
        }
        else
        {
            actionParameters.append(actionItems.at(i));
        }
    }


    int pos = 0;
    // replace the arguments of domain action with the parameters of plan action(replace variable with object)
    QRegExp parametersChecker;
    parametersChecker.setPattern(QString("\\:parameters[\\s]*\\([^()]*\\)"));
    parametersChecker.indexIn(domainAction);
    QString parameters = parametersChecker.cap(0);
    parameters = parameters.simplified();

    parametersChecker.setPattern("\\([^()]*\\)");
    parametersChecker.indexIn(parameters);
    parameters = parametersChecker.cap(0);
    parameters = parameters.simplified();

    QStringList argumentsList;
    QStringList argumentNameList;
    parameters.remove(parameters.indexOf("("), 1);
    parameters.remove(parameters.indexOf(")"), 1);
    argumentsList = parameters.split("?", QString::SkipEmptyParts);
    foreach (QString argument, argumentsList) {
        argument = argument.simplified();
        if (!argument.isEmpty())
        {
            QStringList argumentItems = argument.split(" ", QString::SkipEmptyParts);
            QString argumentName = argumentItems.at(0);
            argumentNameList.append(argumentName.simplified());
        }
    }
    for (int i = 0; i < argumentNameList.size(); i++)
    {
        QRegExp argumentNameChecker;
        QString pattern("[\\s]+");
        pattern.append(argumentNameList.at(i));
        pattern.append("[\\s]+");
        argumentNameChecker.setPattern(pattern);

        domainAction.replace(argumentNameList.at(i), actionParameters.at(i));
    }

    // split the positive predicates and negative predicates from preconditions
    QRegExp preconditionChecker;
    preconditionChecker.setPattern(QString("[\\s]*\\:precondition[\\s\\S]*[\\s]*\\:"));
    preconditionChecker.indexIn(domainAction);
    QString precondition = preconditionChecker.cap(0);
    precondition.remove(precondition.lastIndexOf(":"), 1);
    precondition = precondition.simplified();

    // select all predicates including positive and negative
    QRegExp predicateChecker;
    predicateChecker.setPattern(QString("\\([^()]*\\)"));
    pos = 0;
    QStringList predicateList;
    while ((pos = predicateChecker.indexIn(precondition, pos)) != -1)
    {
        predicateList.append(predicateChecker.capturedTexts());
        pos += predicateChecker.matchedLength();
    }

    // select negative predicates
    QRegExp negativePredicateChecker;
    negativePredicateChecker.setPattern(QString("not[\\s]*\\([^()]*\\)"));
    pos = 0;
    QStringList negativePredicateList;
    while ((pos = negativePredicateChecker.indexIn(precondition, pos)) != -1)
    {
        negativePredicateList.append(negativePredicateChecker.capturedTexts());
        pos += negativePredicateChecker.matchedLength();
    }
    pos = 0;
    QStringList negativePurePredicateList;
    for (int i = 0; i < negativePredicateList.size(); i++)
    {
        QString tmpStr = negativePredicateList.at(i);
        predicateChecker.indexIn(tmpStr, pos);
        negativePurePredicateList.append(predicateChecker.cap(0).simplified());
    }

    // select positive predicates
    QStringList positivePurePredicateList;
    foreach (QString predicate, predicateList) {
        if (!negativePurePredicateList.contains(predicate))
        {
            // remove the "?" before parameters
            predicate = predicate.replace("?", "");
            positivePurePredicateList.append(predicate.toLower());
        }
    }
    QStringList negativePurePredicateList2;
    for (int i = 0; i < negativePurePredicateList.size(); i++)
    {
        QString tmpStr = negativePurePredicateList.at(i);
        // remove the "?" before parameters
        tmpStr.replace("?", "");
        negativePurePredicateList2.append(tmpStr.toLower());
    }

    // append the positive predicates and negative predicates to plan action preconditions
    planAction.setPositivePreconditions(positivePurePredicateList.toSet());
    planAction.setNegativePreconditions(negativePurePredicateList2.toSet());
}

void PlanValidator::appendEffectsToPlanAction(PlanAction &planAction, QString domainAction)
{
    // this method append the effects of the matched domain action to the plan action


    // get the action name and action parameters of the plan action
    QString actionName;
    QStringList actionParameters;

    QString actionFormula = planAction.getFormula();
    actionFormula.remove(actionFormula.indexOf("("), 1);
    actionFormula.remove(actionFormula.indexOf(")"), 1);
    actionFormula = actionFormula.simplified();
    QStringList actionItems = actionFormula.split(" ");
    for (int i = 0; i < actionItems.size(); i++)
    {
        if (i == 0)
        {
            actionName = actionItems.at(i);
        }
        else
        {
            actionParameters.append(actionItems.at(i));
        }
    }


    int pos = 0;
    // replace the arguments of domain action with the parameters of plan action(replace variable with object)
    QRegExp parametersChecker;
    parametersChecker.setPattern(QString("\\:parameters[\\s]*\\([^()]*\\)"));
    parametersChecker.indexIn(domainAction);
    QString parameters = parametersChecker.cap(0);
    parameters = parameters.simplified();

    parametersChecker.setPattern("\\([^()]*\\)");
    parametersChecker.indexIn(parameters);
    parameters = parametersChecker.cap(0);
    parameters = parameters.simplified();

    QStringList argumentsList;
    QStringList argumentNameList;
    parameters.remove(parameters.indexOf("("), 1);
    parameters.remove(parameters.indexOf(")"), 1);
    argumentsList = parameters.split("?", QString::SkipEmptyParts);
    foreach (QString argument, argumentsList) {
        argument = argument.simplified();
        if (!argument.isEmpty())
        {
            QStringList argumentItems = argument.split(" ", QString::SkipEmptyParts);
            QString argumentName = argumentItems.at(0);
            argumentNameList.append(argumentName.simplified());
        }
    }
    for (int i = 0; i < argumentNameList.size(); i++)
    {
        domainAction.replace(argumentNameList.at(i), actionParameters.at(i));
    }

    // split the positive effects and negative effects from effect
    QRegExp effectChecker;
    effectChecker.setPattern(QString("[\\s]*\\:effect[\\s\\S]*"));
    effectChecker.indexIn(domainAction);
    QString effect = effectChecker.cap(0);
    //effect.remove(effect.lastIndexOf(":"), 1);
    effect = effect.simplified();

    // select all predicates including positive and negative predicates
    QRegExp predicateChecker;
    predicateChecker.setPattern(QString("\\([^()]*\\)"));
    pos = 0;
    QStringList predicateList;
    while ((pos = predicateChecker.indexIn(effect, pos)) != -1)
    {
        predicateList.append(predicateChecker.capturedTexts());
        pos += predicateChecker.matchedLength();
    }

    // select negative predicates
    QRegExp negativePredicateChecker;
    negativePredicateChecker.setPattern(QString("not[\\s]*\\([^()]*\\)"));
    pos = 0;
    QStringList negativePredicateList;
    while ((pos = negativePredicateChecker.indexIn(effect, pos)) != -1)
    {
        negativePredicateList.append(negativePredicateChecker.capturedTexts());
        pos += negativePredicateChecker.matchedLength();
    }
    pos = 0;
    QStringList negativePurePredicateList;
    for (int i = 0; i < negativePredicateList.size(); i++)
    {
        QString tmpStr = negativePredicateList.at(i);
        predicateChecker.indexIn(tmpStr, pos);
        negativePurePredicateList.append(predicateChecker.cap(0).simplified());
    }

    // select positive predicates
    QStringList positivePurePredicateList;
    foreach (QString predicate, predicateList) {
        if (!negativePurePredicateList.contains(predicate))
        {
            // remove the "?" before parameters
            predicate = predicate.replace("?", "");
            positivePurePredicateList.append(predicate.toLower());
        }
    }
    QStringList negativePurePredicateList2;
    for (int i = 0; i < negativePurePredicateList.size(); i++)
    {
        QString tmpStr = negativePurePredicateList.at(i);
        // remove the "?" before parameters
        tmpStr.replace("?", "");
        negativePurePredicateList2.append(tmpStr.toLower());
    }

    // append the positive predicates and negative predicates to plan action effects
    planAction.setPositiveEffects(positivePurePredicateList.toSet());
    planAction.setNegativeEffects(negativePurePredicateList2.toSet());
}

void PlanValidator::matchPlanActionWithDomain(PlanAction &action)
{
    // this method match the plan action with the domain file

    QString actionName;
    QStringList actionParameters;

    QString actionFormula = action.getFormula();
    actionFormula.remove(actionFormula.indexOf("("), 1);
    actionFormula.remove(actionFormula.indexOf(")"), 1);
    actionFormula = actionFormula.simplified();
    QStringList actionItems = actionFormula.split(" ");
    for (int i = 0; i < actionItems.size(); i++)
    {
        if (i == 0)
        {
            actionName = actionItems.at(i);
        }
        else
        {
            actionParameters.append(actionItems.at(i));
        }
    }

    QString matchedDomainAction;
    selectMatchedActionFromDomainActions(actionName, domainActions, matchedDomainAction);

    appendPreconditionsToPlanAction(action, matchedDomainAction);
    appendEffectsToPlanAction(action, matchedDomainAction);
}

void PlanValidator::selectMatchedActionFromDomainActions(QString actionName, QStringList& domainActions, QString& targetDomainAction)
{
    QRegExp actionNameChecker;
    QString pattern("[\\s]+");
    pattern.append(actionName);
    pattern.append("[\\s]+");
    actionNameChecker.setPattern(pattern);
    foreach (QString action, domainActions) {
        if (action.contains(actionNameChecker))
        {
            QRegExp tmpChecker;
            tmpChecker.setPattern(QString("\\([\\s]*\\:action"));
            if ((action.indexOf(actionName) > action.indexOf(tmpChecker)))
            {
                tmpChecker.setPattern(QString("[\\s]*\\:parameters"));
                if (action.indexOf(actionName) < action.indexOf(tmpChecker))
                {
                    targetDomainAction = action;
                }
            }
        }
    }
}

void PlanValidator::splitDomainActionsToString(QString domainFile, QStringList &actions)
{
    // this function split the plain text of domain file for all the actions as string, such as
    /*
  ( :action load-truck
    :parameters
         (?obj - package
          ?truck - truck
          ?loc - location)
    :precondition
        (and 	(at ?truck ?loc)
            (at ?obj ?loc))
    :effect
        (and 	(not (at ?obj ?loc))
            (in ?obj ?truck)))
     */

    QRegExp actionChecker;
    //actionChecker.setPattern(QString("\\([\\s]*\\:action[\\s\\S]*?[\\s]*\\:parameters[\\s\\S]*?[\\s]*\\:precondition[\\s\\S]*?[\\s]*\\:effect[\\s\\S]*?"));
    actionChecker.setPattern(QString("\\([\\s]*\\:action[\\s\\S]*[\\s]*\\:parameters[\\s\\S]*[\\s]*\\:precondition[\\s\\S]*[\\s]*\\:effect[\\s\\S]*"));
    int pos = actionChecker.indexIn(domainFile);
    QString actionsString = actionChecker.cap(0);
    //QString str2 = actionChecker.cap(1);
    //QString str3 = actionChecker.cap(3);
    //actions = actionChecker.capturedTexts();
    actionsString.remove(actionsString.lastIndexOf(")"), actionsString.size()-actionsString.lastIndexOf(")"));
    QRegExp tmpChecker;
    tmpChecker.setPattern(QString("\\([\\s]*\\:action"));
    QStringList tmpList = actionsString.split(tmpChecker);

    foreach (QString action, tmpList) {
        if (!action.isEmpty())
        {
            action.prepend("(:action ");
            actions.append(action.simplified());
        }
    }
}

void PlanValidator::splitProblemInitToString(QString problemFile, QString &initString)
{
    // this function split the plain text of problem file for the init as string, such as
    /*
    (:init
        (holding R1 B2)
        (clear B1)
    )
     */

    QString tmpString = QString(problemFile.split("(:init").at(1));
    tmpString = QString(tmpString.split("(:goal").at(0));
    tmpString.prepend("(:init ");
    initString = tmpString.simplified();
}

void PlanValidator::splitProblemGoalToString(QString problemFile, QString &goalString)
{
    // this function split the plain text of problem file for the goal as string, such as
    /*
    (:goal
        (and
            (empty R1)
            (clear B2)
            (on B2 B1)
        )
    )
     */

    QString tmpString = QString(problemFile.split("(:goal").at(1));
    tmpString.prepend("(:goal ");
    goalString = tmpString.simplified();
}

QString PlanValidator::getContentsAsString(QFile &file)
{
    QString res;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        res.append(str).append("\n");
    }
    file.seek(0);
    return res;
}

QStringList PlanValidator::getContentsAsStringList(QFile &file)
{
    QStringList res;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        res.append(str);
    }
    file.seek(0);
    return res;
}

QStringList PlanValidator::getValidatorOutput(QDomElement chosenValidator, QString domain, QString problem, QString plan, QStringList &consoleOutput)
{
    QStringList output;
    QString solutionFile =  "solution.soln";

    // 1. get main validator's parameters and arguments
    QDomElement settings = chosenValidator.firstChildElement("settings");

    QStringList commandArguments;

    // 1.0 get validator execution file
    QString validatorFilePath;
    validatorFilePath.append(getValidatorsPath()).append(getStrValue(settings.firstChildElement("filePath")));
    commandArguments.append(validatorFilePath);

    QFile validatorFile(validatorFilePath);

    bool validatorFileExists = true;
    if (!validatorFile.exists())
    {
        validatorFileExists = false;
    }

    //proceed only if validator file exists
    if (validatorFileExists)
    {

        //GET ALL ARGUMENTS (including domain, problem, plan) in the specified ORDER

        //1.1 Get all arguments
        QList<QDomElement> theArgs;
        QDomNodeList allArgs = settings.firstChildElement("arguments").childNodes();
        for (int i = 0; i < allArgs.size(); i++)
        {
            QDomElement arg = allArgs.at(i).toElement();
            if (getStrValue(arg.firstChildElement("enable")) == "true")
            {
                theArgs.append(arg);
            }
        }

        if (!theArgs.isEmpty())
        {
            for (int i = 0; i < theArgs.size();i ++)
            {
                QDomElement argElement = theArgs[i];
                if (!getStrAttribute(argElement, "parameter").trimmed().isEmpty())
                {
                    commandArguments.append(getStrAttribute(argElement, "parameter"));
                }
                //check if this is a domain argument
                if (argElement.tagName() == "domain")
                {
                    commandArguments.append(domain);
                }
                //check if this is a problem argument
                else if (argElement.tagName() == "problem")
                {
                    commandArguments.append(problem);
                }
                //check if this is a plan argument
                else if (argElement.tagName() == "plan")
                {
                    commandArguments.append(plan);
                }
                //if there is a value for the argument then add to the command
                else if (!getStrValue(argElement.firstChildElement("value")).trimmed().isEmpty())
                {
                    commandArguments.append(getStrValue(argElement.firstChildElement("value")));
                }
            }
        }

        //1.2 Get output arguments
        bool outputFile;
        QDomElement outputElement = settings.firstChildElement("output");
        if (getStrAttribute(outputElement, "hasOutputFile") == "true")
        {
            outputFile = true;
            solutionFile = getStrValue(outputElement.firstChildElement("outputFile").firstChildElement("fileName")).trimmed();
            if (getStrAttribute(outputElement.firstChildElement("outputFile").firstChildElement("argument"), "needArgument") == "true")
            {
                commandArguments.append(getStrAttribute(outputElement.firstChildElement("outputFile").firstChildElement("argument"), "parameter"));
                commandArguments.append(solutionFile);
            }
        }
        else
        {
            outputFile = false;
        }

        //Prepare the command line
        QString program = commandArguments.at(0);
        commandArguments.removeAt(0);
        QStringList arguments = commandArguments;

        this->time = 0;
        //set initial time
        QTime start_time;
        start_time.start();

        gotError = false;
        readyReadOutput = false;
        normalExit = false;

        process = new QProcess();
        //process->setProcessChannelMode(QProcess::SeparateChannels);
        //process->setReadChannel(QProcess::StandardOutput);

        //connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
        //connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(startReadOutput()));
        //connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishProcess(int, QProcess::ExitStatus)));

        //Call the planner
        process->start(program, arguments);
        //process->execute(program, arguments);
        //process->startDetached(program, arguments);
        process->waitForFinished();

        if (!gotError)
        {
            consoleOutput = QString::fromLocal8Bit(process->readAllStandardOutput()).split("\n");

            this->time = start_time.elapsed();

            //The validator does provide a output file
            if (outputFile)
            {

            }
            //The validator does not provide a output file, just the console message
            else
            {
                parseValidatorOutputToPlan(consoleOutput);
                buildDependenceBetweenPlanAction();
            }
        }
    }
}

void PlanValidator::parseValidatorOutputToPlan(QStringList &consoleOutput)
{
    bool planActioning = false;
    bool planValidationDetails = false;
    bool checkingNextHappening = false;
    bool planRepairAdvice = false;
    bool repairAdvising = false;

    int repairedPlanActionIndex = -1;

    QVector<PlanAction> tmpPlanActions;

    planSuccess = true;
    int lastCheckedPlanActionIndex = -1;
    double lastCheckedPlanActionTime;

    for (int i = 0; i < consoleOutput.size(); i++)
    {
        QString line = consoleOutput.at(i);

        line = line.simplified();
        if (line.isEmpty())
        {
            continue;
        }

        if (line.contains("Plan size:"))
        {
            line = line.remove(0, line.indexOf(":")+1).simplified();
            setPlanSize(line.toInt());
            planActioning = true;
            continue;
        }
        if (line.contains("Plan Validation details"))
        {
            planActioning = false;
            planValidationDetails = true;
        }
        else if (planActioning)
        {
            double actionTime;
            QString actionFormula;

            line = line.remove(line.indexOf(":"), 1).simplified();
            actionTime = line.toDouble();

            i++;

            line = consoleOutput.at(i);
            actionFormula = line.simplified();

            PlanAction addAction;
            addAction.setTime(actionTime);
            addAction.setFormula(actionFormula);

            matchPlanActionWithDomain(addAction);
            tmpPlanActions.append(addAction);

            //plan->addAction(addAction);
        }
        else if (planValidationDetails)
        {
            if (line.contains("Checking next happening"))
            {
                checkingNextHappening = true;

                lastCheckedPlanActionIndex++;

                QString timeStr = line;
                timeStr.remove(0, timeStr.indexOf("(")+1);
                timeStr.remove(timeStr.indexOf(")"), 1);
                timeStr = timeStr.simplified();
                lastCheckedPlanActionTime = QString(timeStr.split(" ").at(1)).toDouble();
            }
            else if (line.contains("Plan failed to execute"))
            {
                planSuccess = false;
                planValidationDetails = false;
            }
            else if (line.contains("Plan executed successfully - checking goal"))
            {
                planSuccess = true;
                planValidationDetails = false;
                break;
            }
        }
        else if (line.contains("Plan Repair Advice:"))
        {
            planRepairAdvice = true;
        }
        else if (planRepairAdvice)
        {
            if (line.contains("at time"))
            {
                repairAdvising = true;
                double repairedPlanActionTime = QString(line.split(" ").last()).toDouble();
                for (int j = 0; j < tmpPlanActions.size(); j++)
                {
                    if (tmpPlanActions[j].getTime() == repairedPlanActionTime)
                    {
                        repairedPlanActionIndex = j;
                    }
                }
            }
            else if (repairAdvising && line.contains("Set"))
            {
                QRegExp repairedPreconditionChecker;
                repairedPreconditionChecker.setPattern(QString("\\([^()]*\\)"));
                repairedPreconditionChecker.indexIn(line);
                QString repairedPrecondition = repairedPreconditionChecker.cap(0);
                bool advice;
                if (line.contains("true"))
                {
                    advice = true;
                }
                else if (line.contains("false"))
                {
                    advice = false;
                }
                //PlanAction(tmpPlanActions.at(repairedPlanActionIndex)).addRepairAdvice(repairedPrecondition, advice);
                tmpPlanActions[repairedPlanActionIndex].addRepairAdvice(repairedPrecondition, advice);
            }
            else if (line.contains("Failed plans:"))
            {
                planRepairAdvice = false;
                break;
            }
        }
    }

    // add all plan action to the plan, additionly construct the init action and goal action
    PlanAction initPlanAction;
    PlanAction goalPlanAction;

    setInitPlanAction(initPlanAction);
    setGoalPlanAction(goalPlanAction);

    this->plan->addAction(initPlanAction);

    if (planSuccess)
    {
        foreach (PlanAction tmpPlanAction, tmpPlanActions) {
            this->plan->addAction(tmpPlanAction);
        }
    }
    else {
        foreach (PlanAction tmpPlanAction, tmpPlanActions) {
            this->plan->addAction(tmpPlanAction);
        }
    }

    this->plan->addAction(goalPlanAction);
}

QString PlanValidator::getValidatorsPath()
{
    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(PLANNERS_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(PLANNERS_DIR_RELEASE);
        break;
    default:
        break;
    }

    return filePath;
}

QString PlanValidator::getDomainFile() const
{
    return domainFile;
}

void PlanValidator::setDomainFile(const QString &value)
{
    domainFile = value;
}

QString PlanValidator::getProblemFile() const
{
    return problemFile;
}

void PlanValidator::setProblemFile(const QString &value)
{
    problemFile = value;
}

QString PlanValidator::getPlanFile() const
{
    return planFile;
}

void PlanValidator::setPlanFile(const QString &value)
{
    planFile = value;
}

QString PlanValidator::getDomainName() const
{
    return domainName;
}

void PlanValidator::setDomainName(const QString &value)
{
    domainName = value;
}

QString PlanValidator::getProblemName() const
{
    return problemName;
}

void PlanValidator::setProblemName(const QString &value)
{
    problemName = value;
}

QDomElement PlanValidator::getChosenValidator() const
{
    return chosenValidator;
}

void PlanValidator::setChosenValidator(const QDomElement &value)
{
    chosenValidator = value;
}

void PlanValidator::setPlanSize(int value)
{
    planSize = value;
}

int PlanValidator::getPlanSize() const
{
    return planSize;
}

void PlanValidator::run()
{
    if (!chosenValidator.isNull() && !domainFile.isNull() && !problemFile.isNull() && !planFile.isNull())
    {
        QStringList consoleOutput;
        getValidatorOutput(chosenValidator, domainFile, problemFile, planFile, consoleOutput);
        PlanAction action;
        //matchPlanActionWithDomain(action);
    }
}
