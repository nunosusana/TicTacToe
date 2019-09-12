#include "tictactoe.h"
#include "./ui_tictactoe.h"
#include <iostream>
#include <string>
#include <algorithm>

short player,opponent;
QPushButton *buttons[9];
bool isSinglePlayer;

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
    isSinglePlayer=false;
    player=1;
    opponent=2;
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
    opponent=(opponent==1?2:1);
    short winner = CheckTable();
    if(winner!=0){
        DisableTable();
        if(winner==1){
            ui->player1->setText(QString("YOU\nWIN"));
        }else{
            ui->player2->setText(QString("YOU\nWIN"));
        }
    }else{
        if(isSinglePlayer && isMovesLeft()){ // AI mode (AI is always the player 2)
            Move mv = findBestMove();
            std::cout << "Best move is: " << mv.lin+1 << "/" << mv.col+1 << std::endl;
            QString butName = "butt"+QString::number(mv.lin+1)+QString::number(mv.col+1);
            TicTacToe::findChild<QPushButton *>(butName)->setText("O");
            tab[mv.lin][mv.col]=player;
            player=(player==1?2:1);
            opponent=(opponent==1?2:1);
            winner = CheckTable();
            if(winner!=0){
                DisableTable();
                if(winner==1){
                    ui->player1->setText(QString("YOU\nWIN"));
                }else{
                    ui->player2->setText(QString("YOU\nWIN"));
                }
            }
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
            TicTacToe::findChild<QPushButton *>(butName)->setText(QString(""));
            TicTacToe::findChild<QPushButton *>(butName)->setEnabled(true);
            tab[i-1][j-1]=0;
        }
    }
    ui->player1->setText(QString(""));
    ui->player2->setText(QString(""));
}

void TicTacToe::on_actionPlayer_2_AI_triggered(){
    isSinglePlayer=!isSinglePlayer;
}


// Code based on geeksforgeeks article
bool TicTacToe::isMovesLeft(){
    for (int i = 0; i<3; ++i)
        for (int j = 0; j<3; ++j)
            if (tab[i][j]==0)
                return true;
    return false;
}

int TicTacToe::evaluate(){
    for (int lin = 0; lin<3; lin++){
        if (tab[lin][0]==tab[lin][1] && tab[lin][1]==tab[lin][2]){
            if (tab[lin][0]==player)
                return +10;
            else if (tab[lin][0]==opponent)
                return -10;
        }
    }
    for (int col = 0; col<3; col++){
        if (tab[0][col]==tab[1][col] && tab[1][col]==tab[2][col]){
            if (tab[0][col]==player)
                return +10;
            else if (tab[0][col]==opponent)
                return -10;
        }
    }
    if (tab[0][0]==tab[1][1] && tab[1][1]==tab[2][2]){
        if (tab[0][0]==player)
            return +10;
        else if (tab[0][0]==opponent)
            return -10;
    }

    if (tab[0][2]==tab[1][1] && tab[1][1]==tab[2][0]){
        if (tab[0][2]==player)
            return +10;
        else if (tab[0][2]==opponent)
            return -10;
    }
    return 0;
}


int TicTacToe::min_max(int depth, bool isMax){
    int score = evaluate();
    if (score == 10)
        return score;
    if (score == -10)
        return score;
    if (isMovesLeft()==false)
        return 0;
    if (isMax){
        int best = -1000;
        for (int i = 0; i<3; ++i){
            for (int j = 0; j<3; ++j){
                if (tab[i][j]==0){
                    tab[i][j] = player;
                    best = std::max(best,min_max(depth+1,!isMax));
                    tab[i][j] = 0;
                }
            }
        }
        return best;
    }else{
        int best = 1000;
        for (int i = 0; i<3; i++){
            for (int j = 0; j<3; j++){
                if (tab[i][j]==0){
                    tab[i][j] = opponent;
                    best = std::min(best,min_max(depth+1, !isMax));
                    tab[i][j] = 0;
                }
            }
        }
        return best;
    }
}

Move TicTacToe::findBestMove(){
    int bestVal = -1000;
    Move bestMove;
    bestMove.lin = -1;
    bestMove.col = -1;
    for (int i = 0; i<3; ++i){
        for (int j = 0; j<3; ++j){
            if (tab[i][j]==0){
                tab[i][j] = player;
                int moveVal = min_max(0, false);
                tab[i][j] = 0;
                if (moveVal > bestVal)
                {
                    bestMove.lin = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

