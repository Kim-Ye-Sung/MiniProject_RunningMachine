#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timecalculator.h"
#include "speedcalculator.h"
#include "distancecalculator.h"
#include "caloriecalculator.h"
#include "db_connector.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <QApplication> // 프로그램 종료 함수를 사용하기 위한 라이브러리 추가
#include <QRegularExpression> // 로그인할때 로그인 아이디의 조건을 확인하기 위한 라이브러리 추가


// #include <QDir>
// #include <QFileInfo>
#include <QDebug>   // 디버그 로그 찍기 위한 라이브러리


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->LoginYesButton->hide();
    ui->LoginNoButton->hide();

    ui->stackedWidget->setCurrentWidget(ui->Login_UI);

    SpeedCal_Obj = std::make_unique<speedCalculator>();
    Calculators.push_back(SpeedCal_Obj.get());
    TimeCal_Obj = std::make_unique<timeCalculator>();
    Calculators.push_back(TimeCal_Obj.get());
    DistanceCal_Obj= std::make_unique<distanceCalculator>();
    Calculators.push_back(DistanceCal_Obj.get());
    CalorieCal_Obj = std::make_unique<calorieCalculator>();
    Calculators.push_back(CalorieCal_Obj.get());

    DBC_Obj = std::make_unique<db_Connector>();
    /*qDebug() << "연결 결과 : " <<*/ DBC_Obj->Connect();

    Timer = std::make_unique<QTimer>();
    connect(Timer.get(), &QTimer::timeout, this, &MainWindow::UpdateScreen);

    SpeedUpButtonTimer =std::make_unique<QTimer>();
    SpeedDownButtonTimer = std::make_unique<QTimer>();

    connect(ui->SpeedUpButton, &QPushButton::pressed, this, [=]()
             {  SpeedUpButtonTimer->start(100);
    });

    connect(ui->SpeedUpButton, &QPushButton::released, this, [=]()
            {   SpeedUpButtonTimer->stop();

    });

    connect(SpeedUpButtonTimer.get(), &QTimer::timeout, this, &MainWindow::SpeedUpButton_Push);

    connect(ui->SpeedDownButton, &QPushButton::pressed, this, [=]()
            {  SpeedDownButtonTimer->start(100);
            });

    connect(ui->SpeedDownButton, &QPushButton::released, this, [=]()
            {   SpeedDownButtonTimer->stop();

            });

    connect(SpeedDownButtonTimer.get(), &QTimer::timeout, this, &MainWindow::SpeedDownButton_Push);

    SettingRecordTable();

    DefaultDate = ui->StartDateEdit->date();

    ui->PWLineEditor->setEchoMode(QLineEdit::Password); // 비밀번호 입력란의 입력을 안보이게 하기.


    // SpeedCal_Obj->SetSpeed(4.0); // 여기 변경
    // SetupRunAnimation();        // 여기 변경
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ID_ConditionCheck(QString ID, QString PW)      // 로그인 ID 조건을 만족하는지 확인하는 함수
{
    QRegularExpression regex("^[A-Za-z0-9]{1,10}$");    // 영어와 숫자로만 이루어진 최대 10자리의 조건

    if (!regex.match(ID).hasMatch())
    {
        return false;  // 조건에 맞지 않으면 false를 반환
    }

    if(!regex.match(PW).hasMatch())
    {
        return false;
    }

    return true;
}

void MainWindow::LoginSuccess()
{
    ClearLoginScreen();

    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);
}

void MainWindow::LoginRecheck() // 정말 아이디가 맞는지 재확인하는 함수
{
    DBC_Obj->SetMemberID(ui->LoginLineEditor->text());
    DBC_Obj->SetPassword(ui->PWLineEditor->text());

    if(DBC_Obj->MemberExists() && DBC_Obj->IsPasswordRight())  // ID가 DB에 존재하고 비밀번호도 일치한다면
    {
        ui->LoginCheckLabel->setText("ID : ""<span style='color:red;'>" + DBC_Obj->GetMemberId()+"</span>""가 존재합니다. <br>로그인 하시겠습니까?");

        ui->LoginButton->setEnabled(true);
    }
    else if(DBC_Obj->MemberExists() && !DBC_Obj->IsPasswordRight())  // ID는 존재하지만 비밀번호가 일치하지 않는다면
    {
        ui->LoginCheckLabel->setText("<span style='color:red;'>""비밀번호가 일치하지 않습니다!""</span>");

        ui->LoginButton->setEnabled(true);

        return;
    }
    else
    {
        ui->LoginCheckLabel->setText("ID : ""<span style='color:red;'>" + DBC_Obj->GetMemberId()+"</span>""가 존재하지 않습니다. <br>새롭게 ID를 만드시겠습니까?");

        ui->LoginButton->setEnabled(true);
    }

    ui->LoginYesButton->show();
    ui->LoginNoButton->show();
}

void MainWindow::on_LoginButton_clicked()
{
    ui->LoginButton->setEnabled(false);

    if(!ID_ConditionCheck(ui->LoginLineEditor->text(), ui->PWLineEditor->text())) // 로그인 ID 조건을 만족하지 않으면
    {
        ui->WarningLabel->setText("! ID와 PW 모두 최소 1자리에서 최대 10자리까지 ! <br> !&nbsp;영어와 숫자로만&nbsp;!");   // 주의 문구 나타내기

        ui->LoginButton->setEnabled(true);
        return;
    }

    ui->WarningLabel->clear();

    LoginRecheck(); // 조건을 만족한다면 다시 묻는 함수 실행
}


void MainWindow::on_StartButton_clicked()
{
    for(auto Cal : Calculators)
    {
        Cal->RunStart(SpeedCal_Obj->GetSpeed());
    }

    Timer->start(UpdateCycle);

    ui->SpeedText->setText((ChangeSpeedText(SpeedCal_Obj->GetSpeed())));

    ui->stackedWidget->setCurrentWidget(ui->Running_UI);
}

void MainWindow::UpdateScreen()
{
    for(auto Cal : Calculators)
    {
        Cal->Calculate(UpdateCycle);
    }

    ui->TimeText->setText(ChangeTimeText(TimeCal_Obj->GetRunTime()));
    ui->DistanceText->setText(ChangeDistanceText(DistanceCal_Obj->GetDistance()));
    ui->CalorieText->setText(ChangeCalorieText(CalorieCal_Obj->GetCalorie()));
}

QString MainWindow::ChangeTimeText(double TimeValue)
{
    int hour = (int)TimeValue / 3600;          // 시간
    int min = ((int)TimeValue % 3600) / 60;    // 분
    int sec = (int)TimeValue % 60;             // 초

    std::stringstream TimeText;
    TimeText << std::setw(2) << std::setfill('0') << hour << ":"  // 시간,분,초 단위를 두자리씩 표기하며, 빈자리는 0으로 채워서 나타내기
             << std::setw(2) << std::setfill('0') << min << ":"
             << std::setw(2) << std::setfill('0') << sec;

    return QString::fromStdString(TimeText.str());
}

QString MainWindow::ChangeSpeedText(double SpeedValue)
{
    std::stringstream SpeedText;
    SpeedText << std::fixed		// 고정 소수점
              << std::setprecision(1)   // 소수점 1자리까지 표기
              << std::setw(4)   // . 까지 포함하여 4자리 나타내기
              << std::setfill(' ')  // 비어있는 곳은 0으로 표기
              << SpeedValue << " km/h";

    return QString::fromStdString(SpeedText.str());
}

QString MainWindow::ChangeDistanceText(double DistanceValue)
{
    std::stringstream DistanceText;

    DistanceText << std::fixed           // 고정 소수점
                 << std::setprecision(3) // 소수점 3자리
                 << std::setw(7)         // 전체 폭 (000.000  == .을 포함한7자리)
                 << std::setfill(' ')    // 빈 자리 공백으로 채움
                 << DistanceValue << " km";

    return QString::fromStdString(DistanceText.str());
}

QString MainWindow::ChangeCalorieText(double CalorieValue)
{
    std::stringstream CalorieText;
    CalorieText << std::fixed
                << std::setprecision(1)
                << std::setw(6)
                << std::setfill(' ')
                << CalorieValue << " kcal";

    return QString::fromStdString(CalorieText.str());
}
void MainWindow::on_Speed4Button_clicked()
{
    double SpeedValue = 4.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    // UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::on_Speed8Button_clicked()
{
    double SpeedValue = 8.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    // UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::on_Speed12Button_clicked()
{
    double SpeedValue = 12.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    // UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::SpeedDownButton_Push()
{
    SpeedCal_Obj->SpeedDown();
    DistanceCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());
    CalorieCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    // UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::SpeedUpButton_Push()
{
    SpeedCal_Obj->SpeedUp();
    DistanceCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());
    CalorieCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    // UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::on_EndButton_clicked()
{
    Timer->stop(); // 타이머 작동 중지하여 기록이 늘어나지 않게 하기

    ui->ResultTimeText->setText(ChangeTimeText(TimeCal_Obj->GetRunTime()));
    ui->ResultSpeedText->setText(ChangeSpeedText(SpeedCal_Obj->AvrSpeedCalulate(DistanceCal_Obj->GetDistance(), TimeCal_Obj->GetRunTime())));
    ui->ResultDistanceText->setText(ChangeDistanceText(DistanceCal_Obj->GetDistance()));
    ui->ResultCalorieText->setText(ChangeCalorieText(CalorieCal_Obj->GetCalorie()));


    ui->stackedWidget->setCurrentWidget(ui->Result_UI);
}


void MainWindow::on_SaveButton_clicked()
{
    ui->SaveButton->setEnabled(false);

    qDebug() << "저장 결과 : " <<   DBC_Obj->SaveRecord(TimeCal_Obj->GetRunTime(),
                                                       SpeedCal_Obj->GetAvrSpeed(),
                                                       DistanceCal_Obj->GetDistance(),
                                                       CalorieCal_Obj->GetCalorie());

    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);

    ui->SaveButton->setEnabled(true);
}


void MainWindow::on_RefuseButton_clicked()
{
    ui->RefuseButton->setEnabled(false);

    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);

    ui->RefuseButton->setEnabled(true);
}

void MainWindow::on_ExitButton_MainMenuUI_clicked()
{
    QApplication::quit();   // 프로그램 종료
}


void MainWindow::on_ExitButton_LoginUI_clicked()
{
    QApplication::quit();
}

void MainWindow::on_LogoutButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Login_UI);
}

void MainWindow::on_LoginYesButton_clicked()
{
    if(DBC_Obj->MemberExists())
    {
        LoginSuccess();
    }
    else
    {
        DBC_Obj->InsertMember();
        LoginSuccess();
    }
}

void MainWindow::on_LoginNoButton_clicked()
{
    ClearLoginScreen();
}

void MainWindow::ClearLoginScreen()
{
    ui->WarningLabel->clear();
    ui->LoginLineEditor->clear();
    ui->PWLineEditor->clear();
    ui->LoginCheckLabel->clear();

    ui->LoginYesButton->hide();
    ui->LoginNoButton->hide();

    if(ShowPw)
    {
        on_ShowPWButton_clicked();
    }
}


void MainWindow::on_RecordButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Record_UI);
}

void MainWindow::on_InquiryButton_clicked()
{
    ui->InquiryButton->setEnabled(false);

    QDate StartDate = ui->StartDateEdit->date();
    QDate EndDate = ui->EndDateEdit->date();

    if(StartDate > EndDate)
    {
        ui->DateWarningLabel->setText("! 시작 날짜가 종료 날짜보다 늦을 수 없습니다 !");

        ui->InquiryButton->setEnabled(true);
        return;
    }

    ui->DateWarningLabel->clear();

    DBC_Obj->InquiryRecord(*(ui->RecordTable), StartDate.toString("yyyy-MM-dd"), EndDate.toString("yyyy-MM-dd"));

    if(ui->RecordTable->rowCount() == 0)
    {
        ui->DateWarningLabel->setText("! 조건에 맞는 데이터가 하나도 없습니다 !");
    }

    ui->InquiryButton->setEnabled(true);
}

void MainWindow::SettingRecordTable()
{
    ui->RecordTable->setColumnCount(5);
    ui->RecordTable->setHorizontalHeaderLabels(
        {"날짜", "운동 시간", "평균 속도 (km/h)", "거리 (km)", "칼로리(kcal)"}
        );

    ui->RecordTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 수정 불가
    ui->RecordTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 행 단위 선택
    ui->RecordTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->RecordTable->horizontalHeader()->setStretchLastSection(true);
    ui->RecordTable->verticalHeader()->setVisible(false);

    ui->RecordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_BackButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);

    ui->RecordTable->setRowCount(0);

    ui->DateWarningLabel->clear();

    ui->StartDateEdit->setDate(DefaultDate);
    ui->EndDateEdit->setDate(DefaultDate);
}

void MainWindow::on_ShowPWButton_clicked()
{
    if(!ShowPw)
    {
        ui->PWLineEditor->setEchoMode(QLineEdit::Normal);
        ShowPw = true;
        ui->ShowPWButton->setText("Hide PW");
    }
    else
    {
        ui->PWLineEditor->setEchoMode(QLineEdit::Password);
        ShowPw = false;
        ui->ShowPWButton->setText("Show PW");
    }
}

// void MainWindow::SetupRunAnimation()
// {
//     qDebug() << "현재 작업 폴더:" << QDir::currentPath();

//     QString path = "../../images/WomanRun.png";
//     qDebug() << "확인할 이미지 경로:" << QFileInfo(path).absoluteFilePath();
//     qDebug() << "파일 존재 여부:" << QFileInfo::exists(path);

//     RunSpriteSheet.load(path);

//     if (RunSpriteSheet.isNull())
//     {
//         qDebug() << "이미지 로드 실패!";
//         return;
//     }
//     else
//     {
//         qDebug() << "이미지 로드 성공!";
//     }

//     FrameWidth = RunSpriteSheet.width() / Columns;
//     FrameHeight = RunSpriteSheet.height() / Rows;

//     RunAnimTimer = new QTimer(this);
//     connect(RunAnimTimer, &QTimer::timeout, this, &MainWindow::UpdateRunAnimation);

//     UpdateRunAnimation();
//     UpdateRunAnimationSpeed();
// }

// void MainWindow::UpdateRunAnimation()
// {
//     if (RunSpriteSheet.isNull())
//         return;

//     int row = CurrentRunFrame / Columns;
//     int col = CurrentRunFrame % Columns;

//     QPixmap frame = RunSpriteSheet.copy(col * FrameWidth,
//                                         row * FrameHeight,
//                                         FrameWidth,
//                                         FrameHeight);

//     ui->RunnerLabel->setPixmap(frame);
//     ui->RunnerLabel->setScaledContents(true);

//     CurrentRunFrame++;
//     if (CurrentRunFrame >= TotalFrames)
//         CurrentRunFrame = 0;
// }

// void MainWindow::UpdateRunAnimationSpeed()
// {
//     double speed = SpeedCal_Obj->GetSpeed();

//     // 0이면 멈춤
//     if (speed <= 0.0)
//     {
//         RunAnimTimer->stop();
//         return;
//     }

//     // 속도를 타이머 간격으로 변환
//     int interval = 150 - (speed * 7);
//     // speed=0 → 150ms (느림)
//     // speed=15 → 45ms (빠름)

//     if (interval < 30)
//         interval = 30;  // 최소 제한

//     RunAnimTimer->start(interval);  // interval의 숫자가 낮을수록 빠르게 재생됨
// }

