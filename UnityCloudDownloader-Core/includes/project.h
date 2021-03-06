#ifndef UCD_PROJECT_H
#define UCD_PROJECT_H

#pragma once

#include "unityclouddownloader-core_global.h"
#include "buildtarget.h"

#include <QString>
#include <QVector>
#include <QUuid>

namespace ucd
{

typedef QVector<BuildTarget> BuildTargetList;

class UCD_SHARED_EXPORT Project
{
public:
    Project();
    Project(const Project &other) = default;
    Project(Project &&other) noexcept;
    ~Project() = default;

    Project& operator=(const Project &other);
    Project& operator=(Project &&other) noexcept;

    const QString& name() const { return m_name; }
    const QUuid& id() const { return m_id; }
    const QUuid& profileId() const { return m_profileId; }
    const QString& cloudId() const { return m_cloudId; }
    const QString& organisationId() const { return m_orgId; }
    const QString& iconPath() const { return m_iconPath; }
    const BuildTargetList& buildTargets() const { return m_buildTargets; }

    void setName(const QString &name);
    void setId(const QUuid &id);
    void setProfileId(const QUuid &profileId);
    void setCloudId(const QString &cloudId);
    void setOrganisationId(const QString &id);
    void setIconPath(const QString &iconPath);
    void setBuildTargets(const BuildTargetList &buildTargets);

private:
    QUuid m_id;
    QUuid m_profileId;
    QString m_cloudId;
    QString m_name;
    QString m_orgId;
    QString m_iconPath;
    BuildTargetList m_buildTargets;
};

}

QDataStream &operator<<(QDataStream &out, const ucd::BuildTargetList &value);
QDataStream &operator>>(QDataStream &in, ucd::BuildTargetList &dest);

QDataStream &operator<<(QDataStream &out, const ucd::Project &value);
QDataStream &operator>>(QDataStream &in, ucd::Project &dest);

Q_DECLARE_METATYPE(ucd::BuildTargetList)
Q_DECLARE_METATYPE(ucd::Project)

#endif // UCD_PROJECT_H
