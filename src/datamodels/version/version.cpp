#include "version.h"

Version::Version() :
    _major(0),
    _minor(0),
    _build(0),
    _revision(0)
{

}

Version::Version(int major, int minor, int build, int revision)
{
    checkNumber(major);
    checkNumber(minor);
    checkNumber(build);
    checkNumber(revision);

    this->_major = major;
    this->_minor = minor;
    this->_build = build;
    this->_revision = revision;
}

void Version::checkNumber(int number)
{
    if(number < 0)
        throw IllegalArgumentException(QObject::tr("The version number cannot be negative, passed: %1.")
                                       .arg(QString::number(number)));
}

Version::Version(QString version)
{
    _major = _minor = _build = _revision = 0;

    QList<QString> list = version.split('.');
    for(int i=list.size(); i<3; i++)
        list.append("");

    _major = list[0].toInt();
    _minor = list[1].toInt();

    list = list[2].split('-');
    if(list.size() < 2)
        list.append("");

    _build = list[0].toInt();
    _revision = list[1].toInt();
}

QString Version::toString() const
{
    QString res;

    if(_revision != 0)
        res = QString("-%1").arg(QString::number(_revision));

    if(_build != 0 || _revision != 0)
        res.prepend(QString(".%1").arg(QString::number(_build)));

    res.prepend(QString("%1.%2").arg(QString::number(_major), QString::number(_minor)));

    return res;
}

bool Version::operator ==(const Version& other) const
{
    if(_major == other._major && _minor == other._minor &&
            _build == other._build && _revision == other._revision)
        return true;

    return false;
}

bool Version::operator !=(const Version& other) const
{
    if(_major != other._major || _minor != other._minor ||
            _build != other._build || _revision != other._revision)
        return true;

    return false;
}

bool Version::operator >(const Version& other) const
{
    if(_major != other._major)
        return (_major > other._major);

    if(_minor != other._minor)
        return (_minor > other._minor);

    if(_build != other._build)
        return (_build > other._build);

    return (_revision > other._revision);
}

bool Version::operator <(const Version& other) const
{
    if(_major != other._major)
        return (_major < other._major);

    if(_minor != other._minor)
        return (_minor < other._minor);

    if(_build != other._build)
        return (_build < other._build);

    return (_revision < other._revision);
}
