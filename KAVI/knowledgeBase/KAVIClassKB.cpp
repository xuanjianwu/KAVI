#include "KAVIClassKB.h"

KAVIClassKB::KAVIClassKB()
{

}

KAVIClassKB::~KAVIClassKB()
{
    this->cachedClasses.clear();
}

bool KAVIClassKB::loadKB(QFile &baseFile)
{
    this->classes.clear();
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
            if (xmlStream.name() == "class")
            {
                classes.append(xmlStream.readElementText());
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

bool KAVIClassKB::saveKB(QFile &baseFile)
{
    refreshCachetoBase();

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
    xmlStream.writeStartElement("classes");
    foreach (QString classSign, classes) {
        xmlStream.writeTextElement("class", classSign);

    }
    xmlStream.writeEndElement();
    xmlStream.writeEndDocument();

    newBaseFile.close();

    return true;
}

bool KAVIClassKB::refreshCachetoBase()
{
    classes.clear();
    classes = cachedClasses;
}

bool KAVIClassKB::readBasetoCache()
{
    cachedClasses.clear();
    cachedClasses = classes;
}

void KAVIClassKB::testAddClasses(QStringList list)
{
    foreach (QString str, list) {
        if (! cachedClasses.contains(str))
        {
            cachedClasses.append(str);
        }
    }
    refreshCachetoBase();
}

void KAVIClassKB::testDeleteClasses(QStringList list)
{
    foreach (QString str, list) {
        if (cachedClasses.contains(str))
        {
            cachedClasses.removeAt(cachedClasses.indexOf(str));
        }
    }
    refreshCachetoBase();
}
