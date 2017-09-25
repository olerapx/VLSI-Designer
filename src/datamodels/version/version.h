#pragma once

#include <QObject>

#include "exception/illegalargumentexception.h"

/**
 * @brief The Version class
 * Represents a version of something.
 *
 * The version is stored in 4 numbers, formatted in string like this:
 * major.minor.build-revision
 * Any missing component in this format is interpreted as 0.
 */
class Version
{
public:
    /**
     * @brief Version
     * Constructs an empty version with all zeroes.
     */
    Version();

    /**
     * @brief Version
     * Constructs the object using the given 4 components.
     * @param major
     * @param minor
     * @param build
     * @param revision
     */
    Version(int major, int minor, int build, int revision);

    /**
     * @brief Version
     * Parses the object from string.
     * @param version
     */
    Version(QString version);

    /**
     * @brief toString
     * Returns a string representing this object in format major.minor.build-revision.
     * @return
     */
    QString toString() const;

    bool operator ==(const Version& other) const;
    bool operator !=(const Version& other) const;
    bool operator >(const Version& other) const;
    bool operator <(const Version& other) const;

private:
    void checkNumber(int number);

    int major;
    int minor;
    int build;
    int revision;
};
