# MiniProject_RunningMachine
부경대IoT 미니프로젝트 과제


# Qt 사용법

## UI 관련
1. 기본적으로 UI 배치는 mainwindow.ui 라는 파일에서 작업한다.

2. 


## 화면 전환 관련
1. 기본적으로 화면은 하나지만 여러개로 작업하고 싶다면 stackedWidget이라는 콘테이너 객체를 화면에 드래그해서 넣는다.
    - 그러면 ObjectInspector창에서 stackedWidget 오브젝트를 마우스 우클릭하고 InsertPage를 통해 여러개의 페이지를 만들수 있다.
    - 이러면 페이지 하나당 내가 전환하고픈 화면 하나인 것이다.

2. 화면을 전환하는 방법
    - cpp파일에서 UI::MainWindow 객체의 포인터를 사용해서 전환한다. 페이지가 몇번 인덱스인지 기억하기 힘드므로 `2번 방법을 추천`한다.
        - ex_1    ui->stackedWidget->SetCurrentIndex(전환하고픈 화면의 인덱스);
        - ex_2    ui->stackedWidget->SetCurrentWidget(ui->전환하고픈 페이지 객체의 이름);

    **작동을 하면 기본적으로 내가 ui에서 작업하던 화면으로 시작을 해버리니, MainWidow 생성자에서 화면을 초기화해주는 것이 좋다.**
    


## 버튼 관련
1. 버튼 누름 관련 함수
    - 게임엔진처럼 버튼을 눌렀을때 실행할 함수를 연결할수있다.
        - 기본적으로 Qt에서 제공하는 기능이며, mainwindow.ui에서 실행하고자 할 버튼을 우클릭 하고 "Go to slot..."을 누르면 원하는 함수를 바인딩할수있다.
        - 바인딩할 함수를 클릭하면 mainwindow.h와 cpp파일에 함수가 생긴다. 보통 함수명은 "on_버튼객체명_clicked()"와 같이 생성되며, 이런식의 함수 이름으로 되어있다면 Qt에서 자동으로 그 버튼에 이 함수를 바인딩해준다.

## 꾸미기 관련
#### `객체의 Property에서 "styleSheet" 부분에 입력한다.`
1. 테두리1(테두리 전체 색칠)
    - 테두리가 없는 객체들이 많다. 이것을 해결하기 위해서는 그 객체의 Property에서 "styleSheet" 부분에 "border: 1px solid black;" 라는 것을 입력하면 검은색의 테두리가 생긴다.

2. 테두리2(부분 테두리 색칠)
    - 테두리 하나하나만 하고 싶을때 사용하는 코드이다. 예를들어 Frame이 사각형인데, 각 방향으로 원하는 곳만 색칠하고 싶다면 방향을 입력해야한다.
        - ex_1  border-right: 2px solid black;  // 사각형중에 오른쪽 부분을 검은색으로 칠한다.
        - ex_2  border-left:none;    // 사각형중 왼쪽은 칠하지 않는다.

3. 테두리3(각진 부분 없애기)
    - Framer같은 거라던가 사각형의 모서리 부분이 각지지 않고 약간 둥글수 있다. 그때 각지게 만들어주고 싶다면 다음과 같이 입력한다.
        - ex_1 border-radius: 0px;

4. 특정 객체만 적용하기
    - Frame안에 자식 객체들이 있으면 내가 Frame에 "border: 1px solid black;"으로 색을 칠하면 안에 있는 객체들까지 전부 동일하게 적용된다. 이때 Frame만 적용하고 싶다면 다음과 같이 입력한다. 
        - ex_1 #프레임객체명{border: 2px solid black;}  // 이러면 프레임객체명의 객체만 적용된다. 반드시 중괄호로 감싸줘야하는걸 잊지말자.

5. 색깔 변경
    - background-color: #3498db;  이렇게 색깔 코드를 입력하면 버튼이라던가 라벨등의 백그라운드 색깔이 변경된다.
    - color: white; 이것은 버튼이나 라벨등 안에 있는 글자의 색깔을 변경하는 코드다.

6. 버튼 모양 둥글게(원형)
    - 기본적으로 버튼등은 사각형 모양으로 시작한다. 그것을 완전 원형으로 변경하려면 다음의 예제처럼 하면 된다.
        - ex_1  width: 50px; height: 50px; border-radius: 25px;   /* 크기의 절반 */ 
        - 이처럼 가로, 세로의 길이를 동일하게하고 모서리부분을 그것의 절반으로 설정하면 완전한 타원형이 된다.






# 데이터베이스 연동하는 방법
1. 시작 메뉴에서 QT라고 입력하면 Qt전용 콘솔앱이 나온다. 그것을 실행한다.

2. 콘솔 입력
    - cd /d 
    - C:\SourceBank\MiniProject_RunningMachine\MiniProject1\build\Desktop_Qt_6_11_0_MinGW_64_bit-Debug
    - 이렇게 실행을 하면 Qt 관련 라이브러리와 플러그인을 실행 폴더로 복사해 준다.

3. 확인용 코드로 확인
    ```cpp
    #include "db_connector.h"

    #include <QSqlDatabase>
    #include <QSqlQuery>
    #include <QSqlError>
    #include <QSqlDriver>
    #include <QVariant>
    #include <QDebug>
    #include <QCoreApplication>

    db_Connector::db_Connector()
    {
        qDebug() << "실행 폴더:" << QCoreApplication::applicationDirPath();
        qDebug() << "라이브러리 경로들:" << QCoreApplication::libraryPaths();
        qDebug() << "사용 가능한 드라이버 목록:" << QSqlDatabase::drivers();
    }
    ```
    위와같은 방식으로 확인했을때, 사용 가능한 드라이버 목록에 "QODBC"가 나온다면 사용 가능한 상태이다.

4. ODBC를 사용해서 MySql에 사용하기 위해 MySQL 사이트에서 설치 파일 다운로드하고 설치
    - 주소 : https://dev.mysql.com/downloads/connector/odbc/?utm_source=chatgpt.com 에서 Windows (x86, 64-bit), MSI Installer 다운로드 후 설치한다.

5. ODBC 설치확인
    - 시작메뉴에서 `ODBC 데이터 원본` 이라고 검색한다. 
        - `ODBC 데이터 원본(64비트)`라는게 보인다면 실행한다.
        - `드라이버`탭에 들어간다.
        - `MySQL ODBC 9.6 Unicode Driver`가 있다면 설치확인 완료이다.

6. db와 연결
```cpp
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
```

7. db에 저장
```cpp
    // 저장할때 입력한 멤버ID가 DB에 없으면 멤버 ID를 생성하여 저장
    bool db_Connector::EnsureMemberExists(int memberId) 
    {
        QSqlQuery query;
        query.prepare("INSERT IGNORE INTO Member (member_id) VALUES (:member_id)");
        query.bindValue(":member_id", memberId);

        if (!query.exec())
        {
            qDebug() << "Member 등록 실패:" << query.lastError().text();
            return false;
        }

        return true;
    }

    // DB에 저장
    bool db_Connector::SaveRecord(int memberId, double runTime, double avgSpeed, double distance, double calorie)
    {
        if (!EnsureMemberExists(memberId))  
        {
            return false;
        }

        QSqlQuery query;
        query.prepare(
            "INSERT INTO RunningRecord "
            "(member_id, run_time, avg_speed, distance, calorie) "
            "VALUES (:member_id, :run_time, :avg_speed, :distance, :calorie)"
            );

        query.bindValue(":member_id", memberId);
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
```