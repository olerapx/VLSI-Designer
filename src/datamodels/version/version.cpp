#include "version.h"

Version::Version() :
    major(0),
    minor(0),
    build(0),
    revision(0)
{

}

Version::Version(int major, int minor, int build, int revision)
{
    checkNumber(major);
    checkNumber(minor);
    checkNumber(build);
    checkNumber(revision);

    this->major = major;
    this->minor = minor;
    this->build = build;
    this->revision = revision;
}

void Version::checkNumber(int number)
{
    if(number < 0)
        throw IllegalArgumentException(QObject::tr("The version number cannot be negative, passed: %1.")
                                       .arg(QString::number(number)));
}

Version::Version(QString version)
{
    major = minor = build = revision = 0;

    QList<QString> list = version.split('.');
    for(int i=list.size(); i<3; i++)
        list.append("");

    major = list[0].toInt();
    minor = list[1].toInt();

    list = list[2].split('-');
    if(list.size() < 2)
        list.append("");

    build = list[0].toInt();
    revision = list[1].toInt();
}

QString Version::toString() const
{
    QString res;

    if(revision != 0)
        res = QString("-%1").arg(QString::number(revision));

    if(build != 0 || revision != 0)
        res.prepend(QString(".%1").arg(QString::number(build)));

    if(minor != 0 || build != 0 || revision != 0)
        res.prepend(QString(".%1").arg(QString::number(minor)));

    res.prepend(QString("%1").arg(QString::number(major)));

    return res;
}

bool Version::operator ==(const Version& other) const
{
    if(major == other.major && minor == other.minor &&
            build == other.build && revision == other.revision)
        return true;

    return false;
}

bool Version::operator !=(const Version& other) const
{
    if(major != other.major || minor != other.minor ||
            build != other.build || revision != other.revision)
        return true;

    return false;
}

bool Version::operator >(const Version& other) const
{
    if(major != other.major)
        return (major > other.major);

    if(minor != other.minor)
        return (minor > other.minor);

    if(build != other.build)
        return (build > other.build);

    return (revision > other.revision);
}

bool Version::operator <(const Version& other) const
{
    if(major != other.major)
        return (major < other.major);

    if(minor != other.minor)
        return (minor < other.minor);

    if(build != other.build)
        return (build < other.build);

    return (revision < other.revision);
}
