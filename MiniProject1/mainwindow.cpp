#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timecalculator.h"
#include "speedcalculator.h"
#include "distancecalculator.h"
#include "caloriecalculator.h"
#include <sstream>
#include <iomanip>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->Login_UI);

    SpeedCal_Obj = std::make_unique<speedCalculator>();
    Calculators.push_back(SpeedCal_Obj.get());
    TimeCal_Obj = std::make_unique<timeCalculator>();
    Calculators.push_back(TimeCal_Obj.get());
    DistanceCal_Obj= std::make_unique<distanceCalculator>();
    Calculators.push_back(DistanceCal_Obj.get());
    CalorieCal_Obj = std::make_unique<calorieCalculator>();
    Calculators.push_back(CalorieCal_Obj.get());

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoginButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);
}


void MainWindow::on_StartButton_clicked()
{
    for(auto Cal : Calculators)
    {
        Cal->RunStart(SpeedCal_Obj->GetSpeed());
    }

    Timer->start(UpdateCycle);

    SetSpeedText(SpeedCal_Obj->GetSpeed());

    ui->stackedWidget->setCurrentWidget(ui->Running_UI);
}

void MainWindow::UpdateScreen()
{
    for(auto Cal : Calculators)
    {
        Cal->Calculate(UpdateCycle);
    }

    SetTimeText(TimeCal_Obj->GetRunTime());
    SetDistanceText(DistanceCal_Obj->GetDistance());
    SetCalorieText(CalorieCal_Obj->GetCalorie());
}

void MainWindow::SetTimeText(double TimeValue)
{
    int hour = (int)TimeValue / 3600;          // 시간
    int min = ((int)TimeValue % 3600) / 60;    // 분
    int sec = (int)TimeValue % 60;             // 초

    std::stringstream TimeText;
    TimeText << std::setw(2) << std::setfill('0') << hour << ":"  // 시간,분,초 단위를 두자리씩 표기하며, 빈자리는 0으로 채워서 나타내기
             << std::setw(2) << std::setfill('0') << min << ":"
             << std::setw(2) << std::setfill('0') << sec;

    ui->TimeText->setText(QString::fromStdString(TimeText.str()));
}

void MainWindow::SetSpeedText(double SpeedValue)
{
    std::stringstream SpeedText;
    SpeedText << std::fixed		// 고정 소수점
              << std::setprecision(1)   // 소수점 1자리까지 표기
              << std::setw(4)   // . 까지 포함하여 4자리 나타내기
              << std::setfill(' ')  // 비어있는 곳은 0으로 표기
              << SpeedValue << " km/h";

    ui->SpeedText->setText(QString::fromStdString(SpeedText.str()));
}

void MainWindow::SetDistanceText(double DistanceValue)
{
    std::stringstream DistanceText;

    DistanceText << std::fixed           // 고정 소수점
                 << std::setprecision(3) // 소수점 2자리
                 << std::setw(7)         // 전체 폭 (000.00  == .을 포함한6자리)
                 << std::setfill(' ')    // 빈 자리 0으로 채움
                 << DistanceValue << " km";

    ui->DistanceText->setText(QString::fromStdString(DistanceText.str()));
}

void MainWindow::SetCalorieText(double CalorieValue)
{
    std::stringstream CalorieText;
    CalorieText << std::fixed
                << std::setprecision(1)
                << std::setw(6)
                << std::setfill(' ')
                << CalorieValue << " kcal";

    ui->CalorieText->setText(QString::fromStdString(CalorieText.str()));
}
void MainWindow::on_Speed4Button_clicked()
{
    double SpeedValue = 4.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    SetSpeedText(SpeedCal_Obj->GetSpeed());
}


void MainWindow::on_Speed8Button_clicked()
{
    double SpeedValue = 8.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    SetSpeedText(SpeedCal_Obj->GetSpeed());
}


void MainWindow::on_Speed12Button_clicked()
{
    double SpeedValue = 12.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    SetSpeedText(SpeedCal_Obj->GetSpeed());
}


void MainWindow::SpeedDownButton_Push()
{
    SpeedCal_Obj->SpeedDown();
    DistanceCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());
    CalorieCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());

    SetSpeedText(SpeedCal_Obj->GetSpeed());
}


void MainWindow::SpeedUpButton_Push()
{
    SpeedCal_Obj->SpeedUp();
    DistanceCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());
    CalorieCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());

    SetSpeedText(SpeedCal_Obj->GetSpeed());
}


void MainWindow::on_EndButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Result_UI);
}

