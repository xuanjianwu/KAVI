#include "PlannerSuggestion.h"



PlannerSuggestion::PlannerSuggestion()
{

}

PlannerSuggestion::~PlannerSuggestion()
{
    suggestedPlanners.clear();
    discardedPlanners.clear();
}

QList<QDomElement> PlannerSuggestion::getSuggestedPlanners()
{
    return suggestedPlanners;
}

void PlannerSuggestion::setSuggestedPlanners(QList<QDomElement> planners)
{
    suggestedPlanners = planners;
}

QList<QDomElement> PlannerSuggestion::getDiscardedPlanners()
{
    return discardedPlanners;
}

void PlannerSuggestion::setDiscardedPlanners(QList<QDomElement> planners)
{
    discardedPlanners = planners;
}

void PlannerSuggestion::initialPlannerSelection(QFile &domainPDDL, QDomElement plannersElement)
{
    suggestedPlanners.clear();
    discardedPlanners.clear();

    QList<QString> pddlRequirements = getPDDLRequirements(domainPDDL);

    QDomNodeList planners = plannersElement.firstChildElement("planners").childNodes();
    for (int i = 0; i < planners.size(); i++) {
        QDomElement plannerElement = planners.at(i).toElement();
        QDomNodeList plannerRequirements = plannerElement.firstChildElement("requirements").childNodes();
        QList<QDomElement> plannerRequirementsElements;

        for (int j = 0; j < plannerRequirements.size(); j++) {
            plannerRequirementsElements.append(plannerRequirements.at(j).toElement());
        }
        if (containsRequirements(plannerRequirementsElements, pddlRequirements) && checkOperatingSystemCompatibility(plannerElement))
        {
            suggestedPlanners.append(plannerElement);
        }
        else
        {
            discardedPlanners.append(plannerElement);
        }
    }
}

bool PlannerSuggestion::containsRequirements(QList<QDomElement> plannerRequirementsElements, QList<QString> pddlRequirements)
{
    bool containsRequirements = true;

    QList<QString> plannerRequirements = getPlannerRequirements(plannerRequirementsElements);

    foreach (QString pddlRequirement, pddlRequirements) {
        bool plannerContainsRequirement = plannerRequirements.contains(pddlRequirement);
        containsRequirements = containsRequirements && plannerContainsRequirement;
        if (!containsRequirements)
        {
            break;
        }
    }
    return containsRequirements;
}

QList<QString> PlannerSuggestion::getPlannerRequirements(QList<QDomElement> plannerRequirementsElements)
{
    QList<QString> res;
    for (int i = 0; i < plannerRequirementsElements.size(); i++)
    {
        QDomElement requirement = plannerRequirementsElements.at(i);
        res.append(requirement.tagName());
    }
    return res;
}

QList<QString> PlannerSuggestion::getPDDLRequirements(QFile &PDDL)
{
    QString requirementsLine;
    QList<QString> res;

    while (!PDDL.atEnd())
    {
        QByteArray line = PDDL.readLine();
        QString str(line);
        if (str.contains("requirements"))
        {
            requirementsLine = str.toLower().simplified();
            break;
        }
    }    

    requirementsLine.remove(requirementsLine.size()-1, 1);
    QStringList itemsList = requirementsLine.split(" ");
    for (int i = 1; i < itemsList.size(); i++)
    {
        QString item = itemsList.at(i);
        res.append(item.remove(0, 1));
    }
    PDDL.seek(0);
    return res;
}

bool PlannerSuggestion::checkOperatingSystemCompatibility(QDomElement planner)
{
    bool compatible = false;
    QString currentOS = getOperatingSystem();

    QDomNodeList supportedOSs = planner.firstChildElement("platform").childNodes();
    for (int i = 0; i < supportedOSs.size(); i++)
    {
        QDomElement supportedOS = supportedOSs.at(i).toElement();
        compatible = compatible || ((supportedOS.tagName().compare(currentOS)) == 0) ;
    }
    return compatible;
}

QString PlannerSuggestion::getOperatingSystem()
{
    QString operatingSystem = QSysInfo::productType().toLower();

    if (operatingSystem.contains("windows"))
    {
        operatingSystem = "windows";
    }
    else if (operatingSystem.contains("osx"))
    {
        operatingSystem = "mac";
    }
    else if (operatingSystem.contains("linux"))
    {
        operatingSystem = "linux";
    }
    return operatingSystem;
}
