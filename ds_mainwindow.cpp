#include "ds_mainwindow.h"
#include "ui_ds_mainwindow.h"

DS_MainWindow::DS_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DS_MainWindow)
{
    ui->setupUi(this);

    //view
    ui->graphicsView_Field->setScene(&SC_Field);

    //read ui
    tick = ui->spinBox_Tick->value();
    Width = ui->spinBox_Size_X->value();
    Height = ui->spinBox_Size_Y->value();
    X_Max = Width - 1;
    Y_Max = Height - 1;

    //seed random
    srand(time(NULL));

    //connect
    connect(&timer, SIGNAL(timeout()), this, SLOT(Game_Tick()));

    //Show
    this->showFullScreen();
    Init_Image();
}

DS_MainWindow::~DS_MainWindow()
{
    delete ui;
}

void DS_MainWindow::Init_Image()
{
    QI_Field = QImage(
                Width,
                Height,
                QImage::Format_RGB888);
    QI_Field.fill(Qt::black);

    Show_Field();
}

void DS_MainWindow::Show_Field()
{
    PX_Field = QPixmap::fromImage(QI_Field);

    PX_Field = PX_Field.scaled(
                ui->graphicsView_Field->width()-2,
                ui->graphicsView_Field->height()-2,
                Qt::KeepAspectRatio,
                Qt::FastTransformation);

    SC_Field.clear();
    SC_Field.addPixmap(PX_Field);
    ui->graphicsView_Field->setSceneRect(SC_Field.itemsBoundingRect());
}

void DS_MainWindow::Run_Game()
{
    Init_Image();
    Init_StartPos();
    timer.start(tick);
}

void DS_MainWindow::Stop_Game()
{
    timer.stop();
    play = false;
}

void DS_MainWindow::Game_Tick()
{
    tick_count++;
    R_Tick_Count++;
    B_Tick_Count++;
    G_Tick_Count++;
    qDebug() << "\n" << "Tick" << tick_count << "========================================";

    //Skip?
    R_Do = R_Tick_Count % R_Skip_Ticks == 0;
    B_Do = B_Tick_Count % B_Skip_Ticks == 0;
    G_Do = G_Tick_Count % G_Skip_Ticks == 0;

    //Routine
    Move_Pos();
    Draw_Point();
    G_Spawn();
    Show_Field();

    R_Act = true;
    B_Act = true;

    qDebug() << "R_Direction:" << R_Direction << "- R_Pos_X:" << R_Pos_X << "- R_Pos_Y:" << R_Pos_Y;
    qDebug() << "B_Direction:" << B_Direction << "- B_Pos_X:" << B_Pos_X << "- B_Pos_Y:" << B_Pos_Y;
}

void DS_MainWindow::Init_StartPos()
{
    R_Pos_X = (int)(0.25 * X_Max);
    R_Pos_Y = (int)(0.50 * Y_Max);
    R_Direction = RR;
    R_Skip_Ticks = 10;
    R_Shift = 1;

    B_Pos_X = 0.75 * X_Max;
    B_Pos_Y = 0.50 * Y_Max;
    B_Direction = LL;
    B_Skip_Ticks = 10;
    B_Shift = 1;
}

void DS_MainWindow::Move_Pos()
{
    if(R_Do)
        Move_Pos(&R_Pos_X, &R_Pos_Y, R_Direction, R_Shift);

    if(B_Do)
        Move_Pos(&B_Pos_X, &B_Pos_Y, B_Direction, B_Shift);
}

void DS_MainWindow::Move_Pos(int *X, int *Y, int D, int S)
{
    *X += Width;
    *Y += Height;

    switch (D) {
    case UU:                *Y -= S;    break;
    case UR:    *X += S;    *Y -= S;    break;
    case RR:    *X += S;                break;
    case DR:    *X += S;    *Y += S;    break;
    case DD:                *Y += S;    break;
    case DL:    *X -= S;    *Y += S;    break;
    case LL:    *X -= S;                break;
    case UL:    *X -= S;    *Y -= S;    break;
    default:                            break;}

    *X %= Width;
    *Y %= Height;
}

void DS_MainWindow::Draw_Point()
{
    QColor R_Target = QI_Field.pixelColor(R_Pos_X, R_Pos_Y);

    if(R_Do)
    {
        if((R_Target == Qt::red) || (R_Target == Qt::blue))
        {
            Stop_Game();
            QMessageBox MSG;
            MSG.setText("Blue wins! :-)");
            MSG.exec();
            B_Score++;
            ui->label_B_Score->setText(QString::number(B_Score));
        }
        else if(R_Target == Qt::green)
        {

            switch(Effect_Random())
            {
            case c_SKIP:
                B_Shift++;
                if(R_Shift > 1)
                    R_Shift--;
                break;

            case c_MULTI:
                G_Do = true;
                for(int i = 0; i < 5; i++)
                    G_Spawn();
                break;

            case c_CLEAR:
                QI_Field.fill(Qt::black);
                break;

            default:
                break;
            }
        }
        else
        {}

        QI_Field.setPixelColor(R_Pos_X, R_Pos_Y, Qt::red);
    }

    QColor B_Target = QI_Field.pixelColor(B_Pos_X, B_Pos_Y);

    if(B_Do)
    {
        if((B_Target == Qt::red) || (B_Target == Qt::blue))
        {
            Stop_Game();
            QMessageBox MSG;
            MSG.setText("Red wins! :-)");
            MSG.exec();
            R_Score++;
            ui->label_R_Score->setText(QString::number(R_Score));
        }
        else if(B_Target == Qt::green)
        {
            switch(Effect_Random())
            {
            case c_SKIP:
                R_Shift++;
                if(B_Shift > 1)
                    B_Shift--;
                break;

            case c_MULTI:
                G_Do = true;
                for(int i = 0; i < 5; i++)
                    G_Spawn();
                break;

            case c_CLEAR:
                QI_Field.fill(Qt::black);
                break;

            default:
                break;
            }

        }
        else
        {}

        QI_Field.setPixelColor(B_Pos_X, B_Pos_Y, Qt::blue);
    }
}

int DS_MainWindow::Effect_Random()
{
    int ran = rand() % 100;

    if(ran > 40)
        return c_SKIP;
    else if(ran > 20)
        return c_MULTI;
    else
        return c_CLEAR;
}

QPoint DS_MainWindow::Pos_Random()
{
    return QPoint(rand() % X_Max, rand() % Y_Max);
}

void DS_MainWindow::G_Spawn()
{
    if(G_Do)
        QI_Field.setPixelColor(Pos_Random(), Qt::green);
}

void DS_MainWindow::on_action_Quit_triggered()
{
    this->close();
}

void DS_MainWindow::on_action_Play_Pause_triggered(bool checked)
{
    play = !checked;
    if(play)
        Run_Game();
    else
        Stop_Game();
}

void DS_MainWindow::on_spinBox_Tick_valueChanged(int arg1)
{
    tick = arg1;
}

void DS_MainWindow::on_spinBox_Size_X_valueChanged(int arg1)
{
    Width = arg1;
    X_Max = Width - 1;
}

void DS_MainWindow::on_spinBox_Size_Y_valueChanged(int arg1)
{
    Height = arg1;
    Y_Max = Height - 1;
}

void DS_MainWindow::on_action_R_Left_triggered()
{
    R_Direction = (R_Direction + 7) % 8;
    R_Act = false;
}

void DS_MainWindow::on_action_R_Right_triggered()
{
    R_Direction = (R_Direction + 1) % 8;
    R_Act = false;
}

void DS_MainWindow::on_action_B_Left_triggered()
{
    B_Direction = (B_Direction + 7) % 8;
    B_Act = false;
}

void DS_MainWindow::on_action_B_Right_triggered()
{
    B_Direction = (B_Direction + 1) % 8;
    B_Act = false;
}

void DS_MainWindow::on_spinBox_Shift_valueChanged(int arg1)
{
    R_Shift = arg1;
    B_Shift = arg1;
}

void DS_MainWindow::on_spinBox_Spawn_valueChanged(int arg1)
{
    G_Skip_Ticks = arg1;
}
