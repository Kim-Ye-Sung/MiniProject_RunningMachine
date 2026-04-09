#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timecalculator.h"
#include "speedcalculator.h"
#include "distancecalculator.h"
#include "caloriecalculator.h"
#include <sstream>
#include <iomanip>
#include <memory>

#include <QDir>
#include <QFileInfo>
#include <QDebug>


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


    SpeedCal_Obj->SetSpeed(4.0); // 여기 변경
    SetupRunAnimation();        // 여기 변경
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
                 << std::setprecision(3) // 소수점 2자리
                 << std::setw(7)         // 전체 폭 (000.00  == .을 포함한6자리)
                 << std::setfill(' ')    // 빈 자리 0으로 채움
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

    UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::on_Speed8Button_clicked()
{
    double SpeedValue = 8.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::on_Speed12Button_clicked()
{
    double SpeedValue = 12.0f;
    SpeedCal_Obj->SetSpeed(SpeedValue);
    DistanceCal_Obj->SetCurrentSpeed(SpeedValue);
    CalorieCal_Obj->SetCurrentSpeed(SpeedValue);

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::SpeedDownButton_Push()
{
    SpeedCal_Obj->SpeedDown();
    DistanceCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());
    CalorieCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::SpeedUpButton_Push()
{
    SpeedCal_Obj->SpeedUp();
    DistanceCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());
    CalorieCal_Obj->SetCurrentSpeed(SpeedCal_Obj->GetSpeed());

    ui->SpeedText->setText(ChangeSpeedText(SpeedCal_Obj->GetSpeed()));

    UpdateRunAnimationSpeed();  // 여기 변경
}


void MainWindow::on_EndButton_clicked()
{
    ui->ResultTimeText->setText(ChangeTimeText(TimeCal_Obj->GetRunTime()));
    ui->ResultSpeedText->setText(ChangeSpeedText(SpeedCal_Obj->AvrSpeedCalulate(DistanceCal_Obj->GetDistance(), TimeCal_Obj->GetRunTime())));
    ui->ResultDistanceText->setText(ChangeDistanceText(DistanceCal_Obj->GetDistance()));
    ui->ResultCalorieText->setText(ChangeCalorieText(CalorieCal_Obj->GetCalorie()));

    ui->stackedWidget->setCurrentWidget(ui->Result_UI);
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenu_UI);
}

void MainWindow::SetupRunAnimation()
{
    qDebug() << "현재 작업 폴더:" << QDir::currentPath();

    QString path = "../../images/WomanRun.png";
    qDebug() << "확인할 이미지 경로:" << QFileInfo(path).absoluteFilePath();
    qDebug() << "파일 존재 여부:" << QFileInfo::exists(path);

    RunSpriteSheet.load(path);

    if (RunSpriteSheet.isNull())
    {
        qDebug() << "이미지 로드 실패!";
        return;
    }
    else
    {
        qDebug() << "이미지 로드 성공!";
    }

    FrameWidth = RunSpriteSheet.width() / Columns;
    FrameHeight = RunSpriteSheet.height() / Rows;

    RunAnimTimer = new QTimer(this);
    connect(RunAnimTimer, &QTimer::timeout, this, &MainWindow::UpdateRunAnimation);

    UpdateRunAnimation();
    UpdateRunAnimationSpeed();
}

void MainWindow::UpdateRunAnimation()
{
    if (RunSpriteSheet.isNull())
        return;

    int row = CurrentRunFrame / Columns;
    int col = CurrentRunFrame % Columns;

    QPixmap frame = RunSpriteSheet.copy(col * FrameWidth,
                                        row * FrameHeight,
                                        FrameWidth,
                                        FrameHeight);

    ui->RunnerLabel->setPixmap(frame);
    ui->RunnerLabel->setScaledContents(true);

    CurrentRunFrame++;
    if (CurrentRunFrame >= TotalFrames)
        CurrentRunFrame = 0;
}

void MainWindow::UpdateRunAnimationSpeed()
{
    double speed = SpeedCal_Obj->GetSpeed();

    // 0이면 멈춤
    if (speed <= 0.0)
    {
        RunAnimTimer->stop();
        return;
    }

    // 속도를 타이머 간격으로 변환
    int interval = 150 - (speed * 7);
    // speed=0 → 150ms (느림)
    // speed=15 → 45ms (빠름)

    if (interval < 30)
        interval = 30;  // 최소 제한

    RunAnimTimer->start(interval);  // interval의 숫자가 낮을수록 빠르게 재생됨
}