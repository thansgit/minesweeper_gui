/* Minesweeper graphical user interface
 * 
 * Description:
 * The program implements a graphical user interface for minesweeper -game based
 * on previously written code. The board is 6 x 6, and can be changed from
 * gameboard.cpp -file. There are two options for game moves, to open a square
 * or to add a flag on a square. The game moves can be applied through the
 * interface's push buttons, which are coded to correspond to the coordinates
 * of the gameboard. There are two different actions the user can take, to place
 * a flag or open a square. If all the mines have been flagged, the player wins
 * the game. And if the player opens a mine, the player loses the game.
 * The text browser in the UI informs the player about the possible win/loss
 * condition. When opening a square, the opened squares inform the player
 * on how many adjacent mines whichever square has.
 * 
 * Writer of the program
 * Name: Timo Hanski
 * E-mail: timo.hanski@gmail.com
 * 
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include <QMainWindow>

#include <QMap>
#include <QPushButton>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //All actions to start/reset game.
    void onPushButtonStartResetClicked();
    //Updates LCD numbers to show time
    void onTimer();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    //Constants for board item (pushbutton) sizes
    const int BOARD_ITEM_HEIGHT = 40;
    const int BOARD_ITEM_WIDTH = 40;

    //Prints the gameboard
    void printGameboard();

    //Handles the board item click
    void handleBoardItemClick();

    //Handles the game move
    void handleGameMove(Square sq, int x, int y);

    //Updates the UI
    void updateUI(Square sq);

    //Disables all the buttons
    void disableButtons();

    int seedValue_;
    GameBoard board_;
    QMap<QPushButton *, Square> squareByButton_;
};
#endif // MAINWINDOW_HH
