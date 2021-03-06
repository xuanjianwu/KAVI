#include "KAVIClassKB.h"

KAVIClassKB::KAVIClassKB()
{

}

KAVIClassKB::~KAVIClassKB()
{
    this->cachedClasses.clear();
}

bool KAVIClassKB::loadKB()
{
    this->classes.clear();

    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:       
        filePath.append(upPath).append(KB_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(KB_DIR_RELEASE);
        break;
    default:
        break;
    }

    if (createFile(filePath, CLASSKB_FILE) == false)
    {
        return false;
    }
    QFile baseFile(filePath.append(CLASSKB_FILE));

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
                this->classes.append(xmlStream.readElementText());
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

bool KAVIClassKB::saveKB()
{
    refreshCachetoBase();

    QString filePath;

    QDir tmpDir;
    QString currentPath = tmpDir.currentPath();
    tmpDir.cdUp();
    QString upPath = tmpDir.path();
    tmpDir.setCurrent(currentPath);
    switch (KAVIRunMode) {
    case Debug:
        filePath.append(upPath).append(KB_DIR_DEBUG);
        break;
    case Release:
        filePath.append(currentPath).append(KB_DIR_RELEASE);
        break;
    default:
        break;
    }

    if (createFile(filePath, CLASSKB_FILE) == false)
    {
        return false;
    }
    QFile baseFile(filePath.append(CLASSKB_FILE));

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
    xmlStream.writeStartElement("classes");
    foreach (QString classSign, this->classes) {
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
    return true;
}

bool KAVIClassKB::readBasetoCache()
{
    cachedClasses.clear();
    cachedClasses = classes;
    return true;
}

QStringList KAVIClassKB::getData() const
{
    return cachedClasses;
}

bool KAVIClassKB::addClass(QString className)
{
    if (!cachedClasses.contains(className, Qt::CaseInsensitive))
    {
        cachedClasses.append(className);
        return true;
    }
    else
    {
        return false;
    }
}

bool KAVIClassKB::removeClass(QString className)
{
    if (cachedClasses.contains(className, Qt::CaseSensitive))
    {
        // if case does not match , will it remove? so set CaseSensitive to
        // remove the CaseSensitive matched class
        cachedClasses.removeAt(cachedClasses.indexOf(className));
        return true;
    }
    else
    {
        return false;
    }
}
