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
    if ( !baseFile.open(QFile::ReadOnly | QFile::Text) )
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
                predicates.append(xmlStream.readElementText());
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

    QString fileName = baseFile.fileName();
    QFile::remove(baseFile.fileName());
    QFile newBaseFile(fileName);

    if ( !newBaseFile.open(QFile::WriteOnly | QFile::Text) )
    {
        qDebug()<< "@Error: cannot open file: " << newBaseFile.fileName();
        return false;
    }
    QXmlStreamWriter xmlStream;
    xmlStream.setDevice(&newBaseFile);

    xmlStream.setAutoFormatting(true);
    xmlStream.writeStartDocument();
    xmlStream.writeStartElement("predicates");
    foreach (QString preSign, predicates) {
        xmlStream.writeTextElement("predicate", preSign);
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
}

bool KAVIPredicateKB::readBasetoCache()
{
    cachedPredicates.clear();
    cachedPredicates = predicates;
}

void KAVIPredicateKB::testAddPredicates(QStringList list)
{
    foreach (QString str, list) {
        if (! cachedPredicates.contains(str))
        {
            cachedPredicates.append(str);
        }
    }
    refreshCachetoBase();
}

void KAVIPredicateKB::testDeletePredicates(QStringList list)
{
    foreach (QString str, list) {
        if (cachedPredicates.contains(str))
        {
            cachedPredicates.removeAt(cachedPredicates.indexOf(str));
        }
    }
    refreshCachetoBase()
}
