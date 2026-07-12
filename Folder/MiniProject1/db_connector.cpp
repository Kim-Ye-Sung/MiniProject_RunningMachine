#include "db_connector.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QUrl>
#include <QDebug>

db_Connector::db_Connector()
{
}

bool db_Connector::Connect()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(BaseUrl + "/member/exists"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["member_id"] = "test_connection";

    QNetworkReply* reply = manager.post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "서버 연결 실패:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    qDebug() << "서버 연결 성공";
    reply->deleteLater();
    return true;
}

bool db_Connector::MemberExists()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(BaseUrl + "/member/exists"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["member_id"] = MemberID;

    QNetworkReply* reply = manager.post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "회원 존재 확인 실패:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();

    return obj["exists"].toBool();
}

bool db_Connector::IsPasswordRight()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(BaseUrl + "/member/check_password"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["member_id"] = MemberID;
    json["password"] = Password;

    QNetworkReply* reply = manager.post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "비밀번호 확인 실패:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();

    return obj["match"].toBool();
}

void db_Connector::InsertMember()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(BaseUrl + "/member/create"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["member_id"] = MemberID;
    json["password"] = Password;

    QNetworkReply* reply = manager.post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "회원 생성 실패:" << reply->errorString();
    }
    else
    {
        qDebug() << "회원 생성 성공";
    }

    reply->deleteLater();
}

bool db_Connector::SaveRecord(double runTime, double avgSpeed, double distance, double calorie)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(BaseUrl + "/record/save"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["member_id"] = MemberID;
    json["run_time"] = runTime;
    json["avg_speed"] = avgSpeed;
    json["distance"] = distance;
    json["calorie"] = calorie;

    QNetworkReply* reply = manager.post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "기록 저장 실패:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();

    return obj["success"].toBool();
}

void db_Connector::InquiryRecord(QTableWidget& RecordTable, QString StartDate, QString EndDate)
{
    RecordTable.setRowCount(0);

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(BaseUrl + "/record/inquiry"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["member_id"] = MemberID;
    json["start_date"] = StartDate;
    json["end_date"] = EndDate;

    QNetworkReply* reply = manager.post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "기록 조회 실패:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject obj = doc.object();

    if (!obj["success"].toBool())
    {
        qDebug() << "서버 응답 실패";
        return;
    }

    QJsonArray records = obj["records"].toArray();

    for (int row = 0; row < records.size(); ++row)
    {
        QJsonObject record = records[row].toObject();

        RecordTable.insertRow(row);
        RecordTable.setItem(row, 0, new QTableWidgetItem(record["record_date"].toString()));
        RecordTable.setItem(row, 1, new QTableWidgetItem(record["run_time"].toString()));
        RecordTable.setItem(row, 2, new QTableWidgetItem(QString::number(record["avg_speed"].toDouble(), 'f', 1)));
        RecordTable.setItem(row, 3, new QTableWidgetItem(QString::number(record["distance"].toDouble(), 'f', 3)));
        RecordTable.setItem(row, 4, new QTableWidgetItem(QString::number(record["calorie"].toDouble(), 'f', 1)));
    }
}