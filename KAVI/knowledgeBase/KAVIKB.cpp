#include "KAVIKB.h"


KAVIKB::KAVIKB()
{
    KAVIRunMode = Debug;
}

KAVIKB::~KAVIKB()
{
   classes.clear();
   predicates.clear();
}


bool KAVIKB::createFile(QString filePath, QString fileName)
{
    QDir tempDir;

    // absolute path of the application's current directory
    QString currentDir = tempDir.currentPath();
    //qDebug() << "@" << tempDir.currentPath() << fileName;

    // if filePath not exists, create it
    if(!tempDir.exists(filePath))
    {
        tempDir.mkpath(filePath);
    }

    QFile *tempFile = new QFile;

    // Sets the application's current working directory to filePath
    tempDir.setCurrent(filePath);
    //qDebug() << "@" << tempDir.currentPath() << fileName;

    // if the expected file exists, return true
    if(tempFile->exists(fileName))
    {
        // restore the application's working directory
        tempDir.setCurrent(currentDir);
        return true;
    }
    // if the expected file not exists, create it
    tempFile->setFileName(fileName);

    if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "@Error: can not open file: " << tempFile->fileName() << " in path: " << filePath;

        // restore the application's working directory
        tempDir.setCurrent(currentDir);
        return false;
    }
    tempFile->close();
    // restore the application's working directory
    tempDir.setCurrent(currentDir);
    //qDebug() << "@" << tempDir.currentPath() << fileName;
    return true;
}
