#ifndef ALIASER_H
#define ALIASER_H

#include <QMap>

#include "datamodels/scheme/scheme.h"

/**
 * @brief The Aliaser class
 * Generates short aliases for elements in the given scheme for rendering.
 */
class Aliaser
{
public:

    /**
     * @brief Aliaser
     * @param length - the number of letters in each alias.
     */
    Aliaser(int length);

    /**
     * @brief generate
     * Performs the generation.
     * @param scheme - the given scheme.
     */
    void generate(Scheme* scheme);

private:
    const QString allowedCharacters =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    int length;

    QString lastGeneratedAlias;
    bool hasAvailableAlias;

    QString nextAlias();

    QString getFirstAlias();
    QString getLastAlias();

    void shiftAlias();
};

#endif // ALIASER_H
