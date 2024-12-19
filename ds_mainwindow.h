#ifndef DS_MAINWINDOW_H
#define DS_MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QMessageBox>
#include <random>
#include <ctime>

namespace Ui {
class DS_MainWindow;
}

class DS_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DS_MainWindow(QWidget *parent = 0);
    ~DS_MainWindow();

private slots:
    void Init_Image();
    void Show_Field();

    void Run_Game();
    void Stop_Game();
    void Game_Tick();

    void Init_StartPos();

    void Move_Pos();
    void Move_Pos(int* X, int* Y, int D, int S);
    void Draw_Point();

    int Effect_Random();

    QPoint Pos_Random();

    void G_Spawn();

    void on_action_Quit_triggered();

    void on_action_Play_Pause_triggered(bool checked);

    void on_spinBox_Tick_valueChanged(int arg1);

    void on_spinBox_Size_X_valueChanged(int arg1);

    void on_spinBox_Size_Y_valueChanged(int arg1);

    void on_action_R_Left_triggered();

    void on_action_R_Right_triggered();

    void on_action_B_Left_triggered();

    void on_action_B_Right_triggered();

    void on_spinBox_Shift_valueChanged(int arg1);

    void on_spinBox_Spawn_valueChanged(int arg1);

private:
    Ui::DS_MainWindow *ui;

    int             Width;
    int             Height;
    int             X_Max;
    int             Y_Max;
    QImage          QI_Field;
    QPixmap         PX_Field;
    QGraphicsScene  SC_Field;

    bool            play = false;

    int             tick_count = 0;
    QTimer          timer;
    int             tick;

    int             R_Direction = UU;
    int             B_Direction = UU;
    bool            R_Act = false;
    bool            B_Act = false;

    int             R_Score = 0;
    int             B_Score = 0;

    int             R_Pos_X;
    int             R_Pos_Y;
    int             B_Pos_X;
    int             B_Pos_Y;

    bool            R_Do = false;
    bool            B_Do = false;
    bool            G_Do = false;
    int             R_Tick_Count = 0;
    int             B_Tick_Count = 0;
    int             G_Tick_Count = 0;
    int             R_Skip_Ticks = 10;
    int             B_Skip_Ticks = 10;
    int             G_Skip_Ticks = 200;

    int             R_Shift = 1;
    int             B_Shift = 1;



    static const int
    UU  = 0,
    UR  = 1,
    RR  = 2,
    DR  = 3,
    DD  = 4,
    DL  = 5,
    LL  = 6,
    UL  = 7;
    static const int
    c_SKIP  = 0,
    c_CLEAR = 1,
    c_MULTI = 2;
};

#endif // DS_MAINWINDOW_H
