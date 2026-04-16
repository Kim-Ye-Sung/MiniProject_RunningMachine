#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <vector>

#include <QPixmap>
#include <QDate>

#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    // LoginButton을 눌렀을때 실행될 내용.
    // Qt에서 on_버튼객체이름_clicked라고 함수명을 지으면 알아서 그 버튼객체에 이 함수를 바인딩해준다.
    void on_LoginButton_clicked();

    void on_StartButton_clicked();

    void on_Speed4Button_clicked();

    void on_Speed8Button_clicked();

    void on_Speed12Button_clicked();

    void SpeedDownButton_Push();

    void SpeedUpButton_Push();

    void on_EndButton_clicked();

    void on_SaveButton_clicked();

    void on_RefuseButton_clicked();

    void on_ExitButton_MainMenuUI_clicked();

    void on_ExitButton_LoginUI_clicked();

    void on_LogoutButton_clicked();

    void on_LoginYesButton_clicked();

    void on_LoginNoButton_clicked();

    void on_RecordButton_clicked();

    void on_InquiryButton_clicked();

    void on_BackButton_clicked();

    void on_ShowPWButton_clicked();

private :
    std::unique_ptr<QTimer> Timer;  // 시간, 거리, 칼로리등을 계산할때 사용하는 타이머

    std::unique_ptr<class timeCalculator> TimeCal_Obj;
    std::unique_ptr<class speedCalculator> SpeedCal_Obj;
    std::unique_ptr<class distanceCalculator> DistanceCal_Obj;
    std::unique_ptr<class calorieCalculator> CalorieCal_Obj;

    std::unique_ptr<class db_Connector> DBC_Obj;

    std::vector<class Calculator*> Calculators;

    std::unique_ptr<QTimer> SpeedUpButtonTimer;
    std::unique_ptr<QTimer> SpeedDownButtonTimer;

private:
    Ui::MainWindow *ui;

    int UpdateCycle = 200;  // 시간, 거리, 칼로리 등등 계산기들의 계산 주기.   200 = 0.2초

    void UpdateScreen();

    QString ChangeTimeText(double TimeValue);
    QString ChangeSpeedText(double SpeedValue);
    QString ChangeDistanceText(double DistanceValue);
    QString ChangeCalorieText(double CalorieValue);

    bool ID_ConditionCheck(QString ID, QString PW);
    void LoginRecheck();
    void LoginSuccess();

    void ClearLoginScreen();

    void SettingRecordTable();

    QDate DefaultDate;

    bool ShowPw = false;

private:
    QPixmap RunSpriteSheet;   // 전체 스프라이트 시트
    std::unique_ptr<QTimer> RunAnimTimer;     // 애니메이션용 타이머

    int CurrentFrame = 0;     // 현재 프레임 번호
    int TotalFrames = 8;      // 총 프레임 수

    int FrameWidth = 0;       // 한 칸 너비
    int FrameHeight = 0;      // 한 칸 높이

    double minSpeed = 0.0;    // 속도계산기의 최저 속도
    double maxSpeed = 15.0;   // 속도계산기의 최대 속도

    int maxInterval = 300; // 런닝머신의 속도가 제일 느릴때 전환할 애니메이션 전환속도
    int minInterval = 10;  // 런닝머신의 속도가 제일 빠를때 전환할 애니메이션 전환속도

    QElapsedTimer AnimElapsed;
    int AnimAccumulator = 0;
    int AnimTickInterval = 16;   // 16ms ~= 60fps 느낌

    void SetupSpriteSheet();

    void UpdateRunAnimation();

    void ChangeFrame();

    int AdjustRunAnimSpeed();
};
#endif // MAINWINDOW_H
