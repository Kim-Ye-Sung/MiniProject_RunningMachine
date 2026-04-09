#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <vector>

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

private :
    std::unique_ptr<QTimer> Timer;  // 시간, 거리, 칼로리등을 계산할때 사용하는 타이머

    std::unique_ptr<class timeCalculator> TimeCal_Obj;
    std::unique_ptr<class speedCalculator> SpeedCal_Obj;
    std::unique_ptr<class distanceCalculator> DistanceCal_Obj;
    std::unique_ptr<class calorieCalculator> CalorieCal_Obj;

    std::vector<class Calculator*> Calculators;

    std::unique_ptr<QTimer> SpeedUpButtonTimer;
    std::unique_ptr<QTimer> SpeedDownButtonTimer;

private:
    Ui::MainWindow *ui;

    int UpdateCycle = 200;  // 시간, 거리, 칼로리 등등 계산기들의 계산 주기.   200 = 0.2초

    void UpdateScreen();

    void SetTimeText(double TimeValue);
    void SetSpeedText(double SpeedValue);
    void SetDistanceText(double DistanceValue);
    void SetCalorieText(double CalorieValue);
};
#endif // MAINWINDOW_H
