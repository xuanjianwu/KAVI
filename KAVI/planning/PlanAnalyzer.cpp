#include "PlanAnalyzer.h"

using namespace XMLUtils;

QString PlanAnalyzer::generateHTMLSinglePlanReport(QDomElement planners, QDomElement xmlPlan)
{
    QDomElement dateElement = xmlPlan.firstChildElement("datetime");
    QString dateTime = getStrValue(dateElement);

    // head
    QString info = "<html>\n";
    info += "<head>\n";
    //info += "<style type=\"text/css\">\n";
    //info += "</style>\n";
    info += "</head>\n";
    info += "<body>\n";

    info.append("<TABLE width='100%' border='0' align='center'>").append("<TR><TD bgcolor='#4169E1'><font size=6 face=arial color='#FFFFFF'>").append("<b>Introduction</b></font></TD></TR>\n");

    QDomElement domainNameElement = xmlPlan.firstChildElement("domain");
    QDomElement problemNameElement = xmlPlan.firstChildElement("problem");
    QDomElement planvalidityElement = xmlPlan.firstChildElement("validity");
    QDomElement validatorElement = xmlPlan.firstChildElement("validator");

    QDomElement toolMessageElement = xmlPlan.firstChildElement("toolInformation").firstChildElement("message");
    QString toolMessage = getStrValue(toolMessageElement);

    // domain and problem
    if (!domainNameElement.isNull() && !problemNameElement.isNull())
    {
        QString domainName = getStrValue(domainNameElement);
        QString problemName = getStrValue(problemNameElement);

        info += "<TR><TD><font size=4 face=arial><b>Domain: </b>"+ domainName +
                        "</font></TD></TR>\n" +
                        "<TR><TD><font size=4 face=arial><b>Problem: </b>"+ problemName +
                        "</font></TD></TR>\n" +
                        "<TR><TD><font size=4 face=arial><b>Date/Time: </b>"+ dateTime +
                        "</font></TD></TR>\n";
        /*
        if ( !planvalidityElement.isNull() && !validatorElement.isNull())
        {
            QString validityString;
            QDomElement validatorNameElement = validatorElement.firstChildElement("name");
            QDomElement validatorVersionElement = validatorElement.firstChildElement("version");
            QString validatorName = getStrValue(validatorNameElement) + " " + getStrValue(validatorVersionElement);

            if (!getStrAttribute(validatorElement, "id").trimmed().isEmpty())
            {
                QDomElement validatorLinkElement = validatorElement.firstChildElement("link");
                QString link = getStrValue(validatorLinkElement);
                if (!link.trimmed().isEmpty())
                {
                    //validatorName += ": " += link;
                    validatorName.append(": ").append(link);
                }
            }

            QString isValid = getStrAttribute(planvalidityElement, "isValid");
            if (isValid.contains("true"))
            {
                validityString = "<font color='green'><strong>Valid plan.</strong></font> (validated by "+validatorName+")";
            }
            else if (isValid.contains("false"))
            {
                validityString = "<font color='red'><strong>Invalid plan. "+getStrValue(planvalidityElement)+".</strong></font> (validated by "+validatorName+")";
            }
            else
            {
                validityString = "<font color='blue'><strong>Unknown. Plan not validated. "+getStrValue(planvalidityElement)+"</strong>";
            }

            //check if this is a empty plan
            if (xmlPlan.firstChildElement("plan").childNodes().size() < 1)
            {
                validityString = "<font color='red'><strong>Empty plan. Goal not satisfied.</strong>";
            }
            info += "<TR><TD><font size=3 face=arial><b>Plan validity: </b>"+ validityString+
                       "</font></TD></TR>\n";
        }
        */
    }

    info += "<TR><TD><font size=4 face=arial><b>KAVI message:<br></b>"+
                    toolMessage.replace("\n", "<br>") +"</font><p></TD></TR>\n";

    // planner
    QDomElement plannerElement = xmlPlan.firstChildElement("planner");
    int targetId = getIntAttribute(plannerElement, "id");

    QDomElement KAVIPlanners = planners;
    QDomElement KAVIPlannersElement = KAVIPlanners.firstChildElement("planners");

    QDomElement settingsPlanner = KAVIPlannersElement.firstChildElement("planner");
    while (!settingsPlanner.isNull())
    {
        int plannerId = getIntAttribute(settingsPlanner, "id");
        if (plannerId == targetId)
        {
            break;
        }
        settingsPlanner = settingsPlanner.nextSiblingElement("planner");
    }


    if ( !settingsPlanner.isNull())
    {
        QDomElement nameElement = settingsPlanner.firstChildElement("name");
        QDomElement versionElement = settingsPlanner.firstChildElement("version");
        QDomElement authorElement = settingsPlanner.firstChildElement("author");
        QDomElement institutionElement = settingsPlanner.firstChildElement("institution");
        QDomElement linkELement = settingsPlanner.firstChildElement("link");
        QDomElement descriptionElement = settingsPlanner.firstChildElement("description");

        info.append("<TR><TD bgcolor='#4169E1'><font size=6 face=arial color='#FFFFFF'><b>Planner</b></font></TD></TR>\n");
        info.append("<TR><TD><font size=4 face=arial><b>Name: </b>").append(getStrValue(nameElement)).append("</font></TD></TR>\n");
        info.append("<TR><TD><font size=4 face=arial><b>Version: </b>").append(getStrValue(versionElement)).append("</font></TD></TR>\n");
        info.append("<TR><TD><font size=4 face=arial><b>Author(s): </b>").append(getStrValue(authorElement)).append("</font></TD></TR>\n");
        info.append("<TR><TD><font size=4 face=arial><b>Institution(s): </b>").append(getStrValue(institutionElement)).append("</font></TD></TR>\n");
        //info.append("<TR><TD><font size=4 face=arial><b>Link: </b>").append(getStrValue(linkELement)).append("</font></TD></TR>\n");
        info.append("<TR><TD><font size=4 face=arial><b>Link: </b>").append("<a href=\"").append(getStrValue(linkELement)).append("\" target=\"_blank\">").append(getStrValue(linkELement)).append("</a>").append("</font></TD></TR>\n");

        info.append("<TR><TD><font size=4 face=arial><b>Description: </b>").append(getStrValue(descriptionElement)).append("</font><p></TD></TR>\n");
    }

    // statistics
    QDomElement statisticsElement = xmlPlan.firstChildElement("statistics");

    info.append("<TR><TD bgcolor='#4169E1'><font size=6 face=arial color='#FFFFFF'><b>Statistics</b>").append("</TD></TR>\n");

    if (!statisticsElement.firstChildElement("toolTime").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Tool total time: </b>").append(statisticsElement.firstChildElement("toolTime").text().trimmed()).append("</font></TD></TR>\n");
    }
    if (!statisticsElement.firstChildElement("time").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Planner time: </b>").append(statisticsElement.firstChildElement("time").text().trimmed()).append("</font></TD></TR>\n");
    }
    if (! statisticsElement.firstChildElement("parsingTime").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Parsing time: </b>").append(statisticsElement.firstChildElement("parsingTime").text().trimmed()).append("</font></TD></TR>\n");
    }

    if (! statisticsElement.firstChildElement("nrActions").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Number of actions: </b>").append(statisticsElement.firstChildElement("nrActions").text().trimmed()).append("</font></TD></TR>\n");
    }

    //info.append("<TR><TD><font size=4 face=arial><b>Number of actions: </b>").append(xmlPlan.firstChildElement("plan").childNodes().size()).append("</font></TD></TR>\n");

    if (! statisticsElement.firstChildElement("makeSpan").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Makespan: </b>").append(statisticsElement.firstChildElement("makeSpan").text().trimmed()).append("</font></TD></TR>");
    }
    if (! statisticsElement.firstChildElement("metricValue").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Metric value: </b>").append(statisticsElement.firstChildElement("metricValue").text().trimmed()).append("</font></TD></TR>\n");
    }
    if (! statisticsElement.firstChildElement("planningTechnique").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Planning technique: </b>").append(statisticsElement.firstChildElement("planningTechnique").text().trimmed()).append("</font></TD></TR>\n");
    }
    if (! statisticsElement.firstChildElement("additional").text().trimmed().isEmpty())
    {
        info.append("<TR><TD><font size=4 face=arial><b>Additional: </b>").append(statisticsElement.firstChildElement("additional").text().trimmed().replace("\n", "<br>")).append("</font></TD></TR>\n");
    }

    info += "<p>";
    // plan
    info += "<TR><TD bgcolor='#4169E1'><font size=6 face=arial color='#FFFFFF'><b>Plan</b></TD></TR>\n";

    QDomNodeList actions = xmlPlan.firstChildElement("plan").childNodes();

    if (actions.size() > 0)
    {
        for (int i = 0; i < actions.size(); i++)
        {
            QDomElement action = actions.at(i).toElement();
            // build up the action string
            // start time
            QString actionStr = action.firstChildElement("startTime").text().trimmed() + " : ";

            // action name
            actionStr += "(" + action.attribute("id") + " ";

            // action parameters
            QDomElement parameters = action.firstChildElement("parameters");
            QDomElement parameter = parameters.firstChildElement("parameter");
            while (!parameter.isNull())
            {
                actionStr += parameter.attribute("id");

                parameter = parameter.nextSiblingElement("parameter");
                if (!parameter.isNull())
                {
                    actionStr += " ";
                }
            }
            actionStr += ")";

            // action duration
            QString duration = action.firstChildElement("duration").text().trimmed();
            if (!duration.isEmpty())
            {
                actionStr += " [" + duration + "]";
            }

            if (i < actions.size() - 1)
            {
                info += "<TR><TD><font size=4 face=arial>"+ actionStr +"</font></TD></TR>\n";
            }
            else
            {
                info += "<TR><TD><font size=4 face=arial>"+ actionStr +"</font><p></TD></TR>\n";
            }
        }
    }
    else
    {
        info += "<TR><TD><font size=4 face=arial>No plan found.</font><p></TD></TR>\n";
    }

    // planner console output
    if ( !plannerElement.isNull() && !plannerElement.firstChildElement("consoleOutput").isNull())
    {

        info.append("<TR><TD bgcolor='#4169E1'><font size=6 face=arial color='#FFFFFF'>").append("<b>Planner Console Output</b></TD></TR>\n").append("<TR><TD><font size=4 face=arial>");
        info.append(plannerElement.firstChildElement("consoleOutput").text().trimmed().replace("\n", "<br>")).append("</font><p></TD></TR>\n");
    }
    info += "</TABLE>\n";

    info += "</body>\n";
    info += "</html>\n";

    return info;
}
