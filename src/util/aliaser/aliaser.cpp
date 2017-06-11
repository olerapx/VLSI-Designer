#include "aliaser.h"

Aliaser::Aliaser(int length)
{
    this->length = length;

    hasAvailableAlias = true;
}

void Aliaser::generate(Scheme *scheme)
{
    QMap<QString, QString> generatedAliases;

    for(SchemeElement& element: scheme->getElements())
    {
        QString key = element.getLibraryId() + ":" + element.getElementId();

        if(generatedAliases.contains(key))
        {
            element.setAlias(generatedAliases[key]);
            continue;
        }

        QString alias = nextAlias();
        generatedAliases.insert(key, alias);

        element.setAlias(alias);
    }
}


QString Aliaser::nextAlias()
{
    if(!hasAvailableAlias)
        return "";

    if(lastGeneratedAlias.isEmpty())
    {
        lastGeneratedAlias = getFirstAlias();
        return lastGeneratedAlias;
    }

    shiftAlias();

    if(lastGeneratedAlias == getLastAlias())
        hasAvailableAlias = false;

    return lastGeneratedAlias;
}


QString Aliaser::getFirstAlias()
{
    QString res;

    for(int i=0; i<length; i++)
        res.push_back(allowedCharacters[0]);

    return res;
}

QString Aliaser::getLastAlias()
{
    QString res;

    int size = allowedCharacters.size();

    for(int i=0; i<length; i++)
        res.push_back(allowedCharacters[size - 1]);

    return res;
}

void Aliaser::shiftAlias()
{
    int size = lastGeneratedAlias.size();

    for(int i=size-1; i>=0; i--)
    {
        QChar aliasChar = lastGeneratedAlias[i];
        int index = allowedCharacters.indexOf(aliasChar);

        if(index == allowedCharacters.size() - 1)
        {
            lastGeneratedAlias[i] = allowedCharacters[0];
            continue;
        }
        lastGeneratedAlias[i] = allowedCharacters[index + 1];

        return;
    }
}
