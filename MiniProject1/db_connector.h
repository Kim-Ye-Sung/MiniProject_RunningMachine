// #pragma once

// class db_Connector
// {
// public:
//     db_Connector();
// };

#include <QString>

#pragma once
class db_Connector
{
private:
    QString MemberID;

public:
    inline void SetMemberID(QString MemberID){this->MemberID = MemberID;}

    inline QString GetMemberId() const {return MemberID;}

    db_Connector();

    bool Connect();

    bool MemberExists();

    bool SaveRecord(double runTime, double avgSpeed, double distance, double calorie);

    void InsertMemberID();

    void InquiryRecord(class QTableWidget& RecordTable, QString StartDate, QString EndDate);
};
