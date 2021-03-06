#include "builddao.h"

#include "build.h"

#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

namespace ucd
{

BuildDao::BuildDao(const QSqlDatabase &database)
    : m_db(database)
{}

void BuildDao::init()
{
    QSqlQuery query(m_db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS Builds ("
               "buildNumber INT, "
               "buildTargetId TEXT, "
               "status TINYINT, "
               "name TEXT, "
               "createTime DATETIME, "
               "iconPath TEXT, "
               "artifactName TEXT, "
               "artifactSize BIGINT, "
               "artifactPath TEXT, "
               "manualDownload BOOLEAN, "
               "PRIMARY KEY(buildNumber, buildTargetId))"))
    {
        auto error = query.lastError().text().toUtf8();
        qFatal("%s", error.data());
        throw std::runtime_error(error);
    }
}

bool BuildDao::hasBuild(const Build &build)
{
    return hasBuild(BuildRef(build));
}

bool BuildDao::hasBuild(const BuildRef &buildRef)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) WHERE EXISTS(SELECT 1 FROM Builds WHERE "
                  "buildNumber = :buildNumber "
                  "AND buildTargetId = :buildTargetId)");
    query.bindValue(":buildNumber", buildRef.buildNumber());
    query.bindValue(":buildTargetId", buildRef.buildTargetId().toString());
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
    if (query.next())
    {
        return query.value(0).toInt() != 0;
    }
    return false;
}

void BuildDao::addBuild(const Build &build, bool orReplace)
{
    if (build.buildTargetId().isNull())
    {
        const char error[] = "Trying to add a build with a null build target.";
        qCritical("%s", error);
        throw std::runtime_error(error);
    }
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
                          "INSERT %1INTO Builds ("
                          "buildNumber, buildTargetId, status, name, "
                          "createTime, iconPath, artifactName, artifactSize, "
                          "artifactPath, manualDownload)"
                          "VALUES (:buildNumber, :buildTargetId, :status, :name, "
                          ":createTime, :iconPath, :artifactName, :artifactSize, "
                          ":artifactPath, :manualDownload)").arg(orReplace ? QStringLiteral("OR REPLACE ") : QStringLiteral("")));
    query.bindValue(":buildNumber", build.id());
    query.bindValue(":buildTargetId", build.buildTargetId());
    query.bindValue(":status", build.status());
    query.bindValue(":name", build.name());
    query.bindValue(":createTime", build.createTime());
    query.bindValue(":iconPath", build.iconPath());
    query.bindValue(":artifactName", build.artifactName());
    query.bindValue(":artifactSize", build.artifactSize());
    query.bindValue(":artifactPath", build.artifactPath());
    query.bindValue(":manualDownload", build.manualDownload());
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
}

void BuildDao::updateBuild(const Build &build)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Builds SET "
                  "status = :status, "
                  "iconPath = :iconPath, "
                  "artifactName = :artifactName, "
                  "artifactSize = :artifactSize, "
                  "artifactPath = :artifactPath, "
                  "manualDownload = :manualDownload "
                  "WHERE buildNumber = :buildNumber "
                  "AND buildTargetId = :buildTargetId");
    query.bindValue(":status", build.status());
    query.bindValue(":iconPath", build.iconPath());
    query.bindValue(":artifactName", build.artifactName());
    query.bindValue(":artifactSize", build.artifactSize());
    query.bindValue(":artifactPath", build.artifactPath());
    query.bindValue(":manualDownload", build.manualDownload());
    query.bindValue(":buildNumber", build.id());
    query.bindValue(":buildTargetId", build.buildTargetId().toString());
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
}

void BuildDao::partialUpdate(const Build &build)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Builds SET "
                  "status = :status, "
                  "iconPath = :iconPath, "
                  "artifactName = :artifactName, "
                  "artifactSize = :artifactSize, "
                  "artifactPath = :artifactPath "
                  "WHERE buildNumber = :buildNumber "
                  "AND buildTargetId = :buildTargetId");
    query.bindValue(":status", build.status());
    query.bindValue(":iconPath", build.iconPath());
    query.bindValue(":artifactName", build.artifactName());
    query.bindValue(":artifactSize", build.artifactSize());
    query.bindValue(":artifactPath", build.artifactPath());
    query.bindValue(":buildNumber", build.id());
    query.bindValue(":buildTargetId", build.buildTargetId().toString());
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
}

void BuildDao::removeBuild(const Build &build)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Builds "
                  "WHERE buildNumber = :buildNumber "
                  "AND buildTargetId = :buildTargetId");
    query.bindValue(":buildNumber", build.id());
    query.bindValue(":buildTargetId", build.buildTargetId().toString());
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
}

QVector<Build> BuildDao::builds(const QUuid &buildTargetId)
{
    QVector<Build> builds;
    QSqlQuery query(m_db);
    if (buildTargetId.isNull())
    {
        query.exec("SELECT * FROM Builds");
    }
    else
    {
        query.prepare("SELECT * FROM Builds WHERE buildTargetId = :buildTargetId ORDER BY buildNumber DESC");
        query.bindValue(":buildTargetId", buildTargetId.toString());
        query.exec();
    }
    while (query.next())
    {
        Build build;
        build.setId(query.value("buildNumber").toInt());
        build.setBuildTargetId(query.value("buildTargetId").toString());
        build.setStatus(query.value("status").toInt());
        build.setName(query.value("name").toString());
        build.setCreateTime(query.value("createTime").toDateTime());
        build.setIconPath(query.value("iconPath").toString());
        build.setArtifactName(query.value("artifactName").toString());
        build.setArtifactSize(query.value("artifactSize").toLongLong());
        build.setArtifactPath(query.value("artifactPath").toString());
        build.setManualDownload(query.value("manualDownload").toBool());
        builds.append(std::move(build));
    }

    return builds;
}

Build BuildDao::build(const QUuid &buildTargetId, int buildNumber)
{
    Build build;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Builds WHERE buildTargetId = :buildTargetId AND buildNumber = :buildNumber");
    query.bindValue(":buildTargetId", buildTargetId.toString());
    query.bindValue(":buildNumber", buildNumber);
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
    else if (query.next())
    {
        build.setId(query.value("buildNumber").toInt());
        build.setBuildTargetId(query.value("buildTargetId").toString());
        build.setStatus(query.value("status").toInt());
        build.setName(query.value("name").toString());
        build.setCreateTime(query.value("createTime").toDateTime());
        build.setIconPath(query.value("iconPath").toString());
        build.setArtifactName(query.value("artifactName").toString());
        build.setArtifactSize(query.value("artifactSize").toLongLong());
        build.setArtifactPath(query.value("artifactPath").toString());
        build.setManualDownload(query.value("manualDownload").toBool());
    }

    return build;
}

void BuildDao::removeBuilds(const QUuid &buildTargetId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Builds WHERE buildTargetId = :buildTargetId");
    query.bindValue(":buildTargetId", buildTargetId.toString());
    if (!query.exec())
    {
        auto error = query.lastError().text().toUtf8();
        qCritical("%s", error.data());
        throw std::runtime_error(error);
    }
}

} // namespace ucd
