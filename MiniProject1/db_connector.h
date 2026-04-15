// // #pragma once

// // class db_Connector
// // {
// // public:
// //     db_Connector();
// // };

// #include <QString>

// #pragma once
// class db_Connector
// {
// private:
//     QString MemberID;

// public:
//     inline void SetMemberID(QString MemberID){this->MemberID = MemberID;}

//     inline QString GetMemberId() const {return MemberID;}

//     db_Connector();

//     bool Connect();

//     bool MemberExists();

//     bool SaveRecord(double runTime, double avgSpeed, double distance, double calorie);

//     void InsertMemberID();

//     void InquiryRecord(class QTableWidget& RecordTable, QString StartDate, QString EndDate);
// };

#pragma once

#include <QString>
#include <QJsonArray>

class db_Connector
{
private:
    QString MemberID;
    QString Password;
    QString BaseUrl = "https://men-aims-mae-grown.trycloudflare.com";

public:
    inline void SetMemberID(QString MemberID) { this->MemberID = MemberID; }
    inline QString GetMemberId() const { return MemberID; }

    inline void SetPassword(QString Password) {this->Password = Password;}

    db_Connector();

    bool Connect();

    bool MemberExists();
    bool IsPasswordRight();

    bool SaveRecord(double runTime, double avgSpeed, double distance, double calorie);

    void InsertMember();

    void InquiryRecord(class QTableWidget& RecordTable, QString StartDate, QString EndDate);
};
