#pragma once

#include <QDir>
#include <QFile>

/**
 * @brief The FileSystem class
 * Encapsulates the paths to session files.
 */
class FileSystem
{
public:
    FileSystem(QString sessionDirectory);

    void setSessionName(QString name);
    QString getSessionName() const { return sessionName; }

    QString getSessionPath() const;

    QString getLibrariesPath() const;
    QString getArchitecturePath() const;

    QString getLevelPath(int level) const;
    QString getGridsPath(int level) const;
    QString getSchemesPath(int level) const;

private:
    QString sessionDirectory;
    QString sessionName;
};
