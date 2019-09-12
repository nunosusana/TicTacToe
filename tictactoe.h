#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TicTacToe; }
QT_END_NAMESPACE

typedef struct _Move{
    int lin,col;
}Move;

class TicTacToe : public QMainWindow
{
    Q_OBJECT
    short tab[3][3];

public:
    TicTacToe(QWidget *parent = nullptr);
    ~TicTacToe();

private:
    Ui::TicTacToe *ui;
    short CheckTable();
    void DisableTable();
    bool isMovesLeft();
    int evaluate();
    int min_max(int depth, bool isMax);
    Move findBestMove();

private slots:
    void MakeMove();
    void on_actionNew_Game_triggered();
    void on_actionPlayer_2_AI_triggered();
};
#endif // TICTACTOE_H
