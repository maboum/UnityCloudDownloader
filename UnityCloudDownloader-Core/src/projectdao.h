#ifndef UCD_PROJECTDAO_H
#define UCD_PROJECTDAO_H

#include <QVector>
#include <QSqlDatabase>

class QUuid;

namespace ucd
{

class Project;

class ProjectDao
{
public:
    ProjectDao(QSqlDatabase &&database);
    ProjectDao(const QSqlDatabase &database);
    ~ProjectDao();

    void init();

    void addProject(const Project &project);
    void updateProject(const Project &project);
    void removeProject(const QUuid &projectId);
    QVector<Project> projects(bool includeBuildTargets = false);

private:
    QSqlDatabase m_db;
};

}

#endif // UCD_PROJECTDAO_H
