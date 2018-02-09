#include "KAVIPredicateKB.h"

KAVIPredicateKB::KAVIPredicateKB()
{

}

KAVIPredicateKB::~KAVIPredicateKB()
{
    this->cachedPredicates.clear();
}

bool KAVIPredicateKB::loadKB(QFile &baseFile)
{
    this->predicates.clear();
    if ( !baseFile.open(QFile::ReadOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << baseFile.fileName();
        return false;
    }
    QXmlStreamReader xmlStream;
    xmlStream.setDevice(&baseFile);

    while (!xmlStream.atEnd())
    {
        QXmlStreamReader::TokenType type = xmlStream.readNext();

        if (xmlStream.isStartElement())
        {
            if (xmlStream.name() == "predicate")
            {
                this->predicates.append(xmlStream.readElementText());
            }
        }
    }
    if (xmlStream.hasError())
    {
        qDebug() << "@error: " << xmlStream.errorString();
    }
    baseFile.close();

    readBasetoCache();

    return true;
}

bool KAVIPredicateKB::saveKB(QFile &baseFile)
{
    refreshCachetoBase();

    // remove the old file and new one
    QString fileName = baseFile.fileName();
    QFile::remove(baseFile.fileName());
    QFile newBaseFile(fileName);

    if ( !newBaseFile.open(QFile::WriteOnly | QFile::Text ))
    {
        qDebug()<< "@Error: cannot open file: " << newBaseFile.fileName();
        return false;
    }
    QXmlStreamWriter xmlStream;
    xmlStream.setDevice(&newBaseFile);

    xmlStream.setAutoFormatting(true);
    xmlStream.writeStartDocument();
    xmlStream.writeStartElement("predicates");
    foreach (QString predicateSign, this->predicates) {
        xmlStream.writeTextElement("predicate", predicateSign);

    }
    xmlStream.writeEndElement();
    xmlStream.writeEndDocument();

    newBaseFile.close();

    return true;
}

bool KAVIPredicateKB::refreshCachetoBase()
{
    predicates.clear();
    predicates = cachedPredicates;
    return true;
}

bool KAVIPredicateKB::readBasetoCache()
{
    cachedPredicates.clear();
    cachedPredicates = predicates;
    return true;
}

QStringList KAVIPredicateKB::getPredicates() const
{
    return cachedPredicates;
}

bool KAVIPredicateKB::addPredicate(QString predicateSign)
{
    if (!cachedPredicates.contains(predicateSign, Qt::CaseInsensitive))
    {
        cachedPredicates.append(predicateSign);
        return true;
    }
    else
    {
        return false;
    }
}

bool KAVIPredicateKB::removePredicate(QString predicateSign)
{
    if (cachedPredicates.contains(predicateSign, Qt::CaseSensitive))
    {
        // if predicate sign does not match , will it remove? so set CaseSensitive to
        // remove the CaseSensitive matched predicate sign
        cachedPredicates.removeAt(cachedPredicates.indexOf(predicateSign));
        return true;
    }
    else
    {
        return false;
    }
}
