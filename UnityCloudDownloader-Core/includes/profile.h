#ifndef UCD_PROFILE_H
#define UCD_PROFILE_H

#pragma once

#include "unityclouddownloader-core_global.h"
#include "project.h"

#include <QString>
#include <QUuid>
#include <QVector>

namespace ucd
{

typedef QVector<Project> ProjectList;

class UCD_SHARED_EXPORT Profile
{
public:
    Profile();
    Profile(const Profile &other);
    Profile(Profile &&other);
    ~Profile();

    Profile& operator=(const Profile &other);
    Profile& operator=(Profile &&other);

    const QUuid& uuid() const { return m_uuid; }
    const QString& name() const { return m_name; }
    const QString& apiKey() const { return m_apiKey; }
    const QString& rootPath() const { return m_rootPath; }
    const ProjectList projects() const { return m_projects; }

    void setUuid(const QUuid &uuid);
    void setName(const QString &name);
    void setApiKey(const QString &apiKey);
    void setRootPath(const QString &rootPath);
    void setProjects(const ProjectList &projects);

private:
    QUuid m_uuid;
    QString m_name;
    QString m_apiKey;
    QString m_rootPath;
    ProjectList m_projects;
};

}

QDataStream &operator<<(QDataStream &out, const ucd::ProjectList &value);
QDataStream &operator>>(QDataStream &in, ucd::ProjectList &dest);

QDataStream &operator<<(QDataStream &out, const ucd::Profile &value);
QDataStream &operator>>(QDataStream &in, ucd::Profile &dest);

Q_DECLARE_METATYPE(ucd::ProjectList)
Q_DECLARE_METATYPE(ucd::Profile)

#endif // UCD_PROFILE_H
