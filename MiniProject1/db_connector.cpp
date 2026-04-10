// #include "db_connector.h"

// #include <QSqlDatabase>
// #include <QSqlQuery>
// #include <QSqlError>
// #include <QSqlDriver>
// #include <QVariant>
// #include <QDebug>
// #include <QCoreApplication>

// db_Connector::db_Connector()
// {
//     qDebug() << "실행 폴더:" << QCoreApplication::applicationDirPath();
//     qDebug() << "라이브러리 경로들:" << QCoreApplication::libraryPaths();
//     qDebug() << "사용 가능한 드라이버 목록:" << QSqlDatabase::drivers();
// }


#include "db_connector.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

db_Connector::db_Connector()
{

}

bool db_Connector::Connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName(
        "DRIVER={MySQL ODBC 9.6 Unicode Driver};"
        "SERVER=127.0.0.1;"
        "DATABASE=RunRecordDB;"
        "USER=KYS;"
        "PASSWORD=KYS123456;"
        "PORT=3306;"
        "OPTION=3;"
        );

    if (!db.open())
    {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }

    qDebug() << "DB 연결 성공";
    return true;
}

bool db_Connector::EnsureMemberExists() // 멤버ID가 DB에 있는지 확인하는 함수. 멤버ID가 DB에 없다면 새롭게 생성한다.
{
    QSqlQuery query;
    query.prepare("INSERT IGNORE INTO Member (member_id) VALUES (:member_id)");
    query.bindValue(":member_id", MemberID);

    if (!query.exec())
    {
        qDebug() << "Member 등록 실패:" << query.lastError().text();
        return false;
    }

    return true;
}

// 기록을 DB에 저장하는 함수
bool db_Connector::SaveRecord(double runTime, double avgSpeed, double distance, double calorie)
{
    if (!EnsureMemberExists()) // 멤버ID를 찾거나 생성하는데, 문제가 생겼으면 기록 저장하지 않고 종료
    {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO RunningRecord "
        "(member_id, run_time, avg_speed, distance, calorie) "
        "VALUES (:member_id, :run_time, :avg_speed, :distance, :calorie)"
        );

    query.bindValue(":member_id", MemberID);
    query.bindValue(":run_time", runTime);
    query.bindValue(":avg_speed", avgSpeed);
    query.bindValue(":distance", distance);
    query.bindValue(":calorie", calorie);

    if (!query.exec())
    {
        qDebug() << "기록 저장 실패:" << query.lastError().text();
        return false;
    }

    qDebug() << "기록 저장 성공";
    return true;
}