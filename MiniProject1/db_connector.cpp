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

#include <QTableWidget>
#include <qdatetime.h>

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

bool db_Connector::MemberExists() // 멤버ID가 DB에 있는지 확인하는 함수. 멤버ID가 DB에 없다면 새롭게 생성한다.
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Member WHERE member_id = :member_id");
    query.bindValue(":member_id", MemberID);

    if(!query.exec())   // 쿼리 실행결과가 false이면.. == 무언가 오류가 나서 쿼리가 제대로 실행되지 않았다면
    {
        qDebug() << "멤버 조회 실패 : " << query.lastError().text();
        return false;
    }

    if(query.next())  // 쿼리 조회 결과의 첫 행으로 이동
    {
        int ID_Count = query.value(0).toInt();

        if(ID_Count >=1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// 기록을 DB에 저장하는 함수
bool db_Connector::SaveRecord(double runTime, double avgSpeed, double distance, double calorie)
{
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

void db_Connector::InsertMemberID()
{
    QSqlQuery query;

    query.prepare("INSERT IGNORE INTO Member (member_id) VALUES (:member_id)");
    query.bindValue(":member_id", MemberID);

    if (!query.exec())
    {
        qDebug() << "Member 등록 실패:" << query.lastError().text();
    }
}

void db_Connector::InquiryRecord(QTableWidget& RecordTable, QString StartDate, QString EndDate)
{
    RecordTable.setRowCount(0);

    QSqlQuery query;

    query.prepare(R"(
    SELECT record_date, TIME_FORMAT(SEC_TO_TIME(run_time), '%H:%i:%s'), avg_speed, distance, calorie
    FROM RunningRecord
    WHERE record_date BETWEEN :StartDate AND :EndDate
    AND member_id = :member_id
    ORDER BY record_date DESC
)");

    query.bindValue(":StartDate", StartDate + " 00:00:00");
    query.bindValue(":EndDate", EndDate + " 23:59:59");
    query.bindValue(":member_id", MemberID);

    if(query.exec())
    {
        int row = 0;
        while(query.next())
        {
            RecordTable.insertRow(row);

            RecordTable.setItem(row, 0, new QTableWidgetItem(query.value(0).toDateTime().toString("yyyy-MM-dd hh:mm")));
            // RecordTable.setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble())));
            RecordTable.setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            RecordTable.setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toDouble(), 'f', 1)));
            RecordTable.setItem(row, 3, new QTableWidgetItem(QString::number(query.value(3).toDouble(), 'f', 3)));
            RecordTable.setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toDouble(), 'f', 1)));

            row++;
        }
    }
}