#include "tictactoe.h"
#include "./ui_tictactoe.h"
#include <iostream>
#include <string>

short player;
QPushButton *buttons[9];

TicTacToe::TicTacToe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TicTacToe)
{
    ui->setupUi(this);
    for(int i = 1; i <= 3; ++i){
        for(int j = 1; j <= 3; ++j){
            QString butName = "butt"+QString::number(i)+QString::number(j);
            buttons[i*3+j]=TicTacToe::findChild<QPushButton *>(butName);
            connect(buttons[i*3+j],SIGNAL(released()),this,SLOT(MakeMove()));
        }
    }
    player=1;
}

TicTacToe::~TicTacToe()
{
    delete ui;
}

short TicTacToe::CheckTable(){
    if((tab[0][0] == tab[0][1]) && (tab[0][1] == tab[0][2]) && (tab[0][0] > 0))          // lines
        return tab[0][0];
    else if((tab[1][0] == tab[1][1]) && (tab[1][1] == tab[1][2]) && (tab[1][0] > 0))
        return tab[1][0];
    else if((tab[2][0] == tab[2][1]) && (tab[2][1] == tab[2][2]) && (tab[2][0] > 0))
        return tab[2][0];
    else if((tab[0][0] == tab[1][0]) && (tab[1][0] == tab[2][0]) && (tab[0][0] > 0))     // collumns
        return tab[0][0];
    else if((tab[0][1] == tab[1][1]) && (tab[1][1] == tab[2][1]) && (tab[0][1] > 0))
        return tab[0][1];
    else if((tab[0][2] == tab[1][2]) && (tab[1][2] == tab[2][2]) && (tab[0][2] > 0))
        return tab[0][2];
    else if((tab[0][0] == tab[1][1]) && (tab[1][1] == tab[2][2]) && (tab[0][0] > 0))     //diagonal
        return tab[0][0];
    else if((tab[0][2] == tab[1][1]) && (tab[1][1] == tab[2][0]) && (tab[0][2] > 0))
        return tab[0][2];
    else
        return 0;
}

void TicTacToe::MakeMove(){
    QPushButton *butt = qobject_cast<QPushButton*>(sender());
    QString bName = butt->objectName();
    int i = bName[4].unicode()-49;
    int j = bName[5].unicode()-49;
    if((butt->text().compare(QString("X")) != 0) && (butt->text().compare(QString("O")) != 0)){
        if(player==1){
            butt->setText(QString('X'));
        }else{
            butt->setText(QString('O'));
        }
        tab[i][j]=player;
    }
    player=(player==1?2:1);
    short winner = CheckTable();
    if(winner!=0){
        DisableTable();
        if(winner==1){
            ui->player1->setText(QString("YOU\nWIN"));
        }else{
            ui->player2->setText(QString("YOU\nWIN"));
        }
    }
}

void TicTacToe::DisableTable(){
    for(int i = 1; i <= 3; ++i){
        for(int j = 1; j <= 3; ++j){
            QString butName = "butt"+QString::number(i)+QString::number(j);
            TicTacToe::findChild<QPushButton *>(butName)->setEnabled(false);
        }
    }
}

void TicTacToe::on_actionNew_Game_triggered(){
    player=1;
    for(int i = 1; i <= 3; ++i){
        for(int j = 1; j <= 3; ++j){
            QString butName = "butt"+QString::number(i)+QString::number(j);
            TicTacToe::findChild<QPushButton *>(butName)->setText(QString(" "));
            TicTacToe::findChild<QPushButton *>(butName)->setEnabled(true);
            tab[i-1][j-1]=0;
        }
    }
    ui->player1->setText(QString(" "));
    ui->player2->setText(QString(" "));
}

