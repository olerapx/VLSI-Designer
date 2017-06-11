#ifndef ALIASER_H
#define ALIASER_H

#include <QMap>

#include "datamodels/scheme/scheme.h"

/**
 * @brief The Aliaser class
 * Generates three-letter aliases for elements in the given scheme for rendering.
 */
class Aliaser
{
public:
    Aliaser(int length);
    void generate(Scheme* scheme);

private:
    const QString allowedCharacters =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    int length;

    QMap<QString, QString> generatedAliases;
    QString lastGeneratedAlias;
    bool hasAvailableAlias;

    QString nextAlias();

    QString getFirstAlias();
    QString getLastAlias();

    void shiftAlias();
};

#endif // ALIASER_H
