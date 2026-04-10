// #pragma once

// class db_Connector
// {
// public:
//     db_Connector();
// };

#pragma once
class db_Connector
{
private:
    int MemberID;

public:
    inline void SetMemberID(int MemberID){this->MemberID = MemberID;}

    db_Connector();

    bool Connect();

    bool EnsureMemberExists();

    bool SaveRecord(double runTime, double avgSpeed, double distance, double calorie);
};
