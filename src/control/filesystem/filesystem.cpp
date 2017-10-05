#include "filesystem.h"

FileSystem::FileSystem(QString sessionDirectory) :
    sessionDirectory(sessionDirectory)
{

}

void FileSystem::setSessionName(QString name)
{
    this->sessionName = name;
}

QString FileSystem::getSessionPath() const
{
    return(sessionDirectory + "/" + sessionName);
}

QString FileSystem::getLibrariesPath() const
{
    return(getSessionPath() + "/libraries");
}

QString FileSystem::getArchitecturePath() const
{
    return(getSessionPath() + "/architecture");
}

QString FileSystem::getLevelPath(int level) const
{
    return getSessionPath() + "/blobs/" + QString::number(level);
}

QString FileSystem::getGridsPath(int level) const
{
    return getLevelPath(level) + "/grids";
}

QString FileSystem::getSchemesPath(int level) const
{
    return getLevelPath(level) + "/schemes";
}
