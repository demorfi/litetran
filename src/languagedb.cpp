#include "languagedb.h"
#include <QFile>
#include <QTextStream>

LanguageDB::LanguageDB(QObject *parent) :
    QObject(parent)
{
    QFile file(":/languages.csv");
    if(!file.open(QFile::ReadOnly | QFile::Text))
        qFatal("Cannot read language database!");

    QTextStream in(&file);

    unsigned int counter = 0;

    while (!in.atEnd()) {
        const QString line = in.readLine();

        QStringList result = line.split(",");

        if(result.size() != 2 )
            qFatal("Database error: invalid entry in line %d" + counter);

        const QString code = result.first();
        const QString name = result.last();

        if(code.isEmpty() || name.isEmpty())
            qFatal("Database error: invalid value in line %d", counter);

        lang_map.insert(name, code);

        counter++;
    }
    file.close();
}