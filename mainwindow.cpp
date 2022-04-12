#include "mainwindow.hh"
#include "gameboard.hh"
#include "ui_mainwindow.h"
#include <iostream>
#include <QGridLayout>
#include <QPushButton>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimer);

    connect(ui->pushButtonStartReset,
            &QPushButton::clicked,
            this,
            &MainWindow::onPushButtonStartResetClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPushButtonStartResetClicked()
{
    //Reset board
    board_ = {};
    //Read seedvalue
    board_.init(ui->spinBoxSeed->text().toInt());
    //Reset textbrowser
    ui->textBrowserInfo->clear();
    //Print the game board
    printGameboard();
    //Clear and start the timer
    ui->lcdNumberSecs->display(0);
    ui->lcdNumberMins->display(0);
    timer->start(1000);
}

void MainWindow::onTimer()
{
    int seconds = ui->lcdNumberSecs->value();
    int minutes = ui->lcdNumberMins->value();

    ++seconds;
    if (seconds == 60) {
        seconds = 0;
        ++minutes;
    }

    ui->lcdNumberSecs->display(seconds);
    ui->lcdNumberMins->display(minutes);
}
void MainWindow::printGameboard()
{
    QGridLayout *gLayout = ui->boardGridLayout;

    //Add QSpacer to keep the items next to each other when resizing window
    auto horizontalSpacer = new QSpacerItem(40,
                                            20,
                                            QSizePolicy::Expanding,
                                            QSizePolicy::Minimum);
    gLayout->addItem(horizontalSpacer, 0, board_.getSize() + 1, 1, 1);

    //Create and style pushbuttons related to the board info
    for (int y = 0; y < board_.getSize(); ++y) {
        for (int x = 0; x < board_.getSize(); ++x) {
            QPushButton *pushButton = new QPushButton("", this);
            pushButton->setFixedHeight(BOARD_ITEM_HEIGHT);
            pushButton->setFixedWidth(BOARD_ITEM_WIDTH);
            pushButton->setStyleSheet("background-color: orange");

            Square current_square = board_.getSquare(x, y);
            if (current_square.hasFlag()) {
                QPixmap flag_image(":/resources/img/flag.png");
                pushButton->setIcon(flag_image);
            } else if (not current_square.isOpen()) {
                pushButton->setText(".");
            } else if (current_square.hasMine()) {
                QPixmap mine_image(":/resources/img/mine.png");
                pushButton->setIcon(mine_image);
                pushButton->setStyleSheet("background-color: red");
            } else {
                pushButton->setText(
                    QString::number(current_square.countAdjacent()));
                pushButton->setStyleSheet("background-color: yellow");
            }

            //Pair a button to its corresponding square
            squareByButton_.insert(pushButton, current_square);

            //Add styled widget to UI
            gLayout->addWidget(pushButton, y, x);
            //Connect
            connect(pushButton,
                    &QPushButton::clicked,
                    this,
                    &MainWindow::handleBoardItemClick);
        }
    }
}

void MainWindow::handleBoardItemClick()
{
    //Iterate the map find sender button and its corresponding square
    for (auto it = squareByButton_.begin(); it != squareByButton_.end(); ++it) {
        //Find sender button and its corresponding square and its coordinates
        if (it.key() == sender()) {
            int x = it->getCoordinates().first;
            int y = it->getCoordinates().second;
            Square squareOfButton = it.value();

            handleGameMove(squareOfButton, x, y);

            updateUI(squareOfButton);

            return;
        }
    }
}

void MainWindow::handleGameMove(Square sq, int x, int y)
{
    //See which command is intended from the user
    if (ui->radioButtonOpen->isChecked()) {
        sq.open();
    } else if (ui->radioButtonFlag->isChecked() && not sq.hasFlag()) {
        sq.addFlag();
    } else {
        sq.removeFlag();
    }
    //Set the corresponding square to the board
    board_.setSquare(sq, x, y);
}

void MainWindow::updateUI(Square sq)
{
    //Always refresh gameboard
    printGameboard();

    //Update textbrowser if the game has been ended.
    if (not sq.open() and ui->radioButtonOpen->isChecked()) {
        ui->textBrowserInfo->setText("BOOOM! Game over!");
        disableButtons();
        timer->stop();
        return;
    }

    if (board_.isGameOver()) {
        ui->textBrowserInfo->setText("You won the game!");
        disableButtons();
        timer->stop();
        return;
    }
}

void MainWindow::disableButtons()
{
    //Disable all buttons
    for (auto it = squareByButton_.begin(); it != squareByButton_.end(); ++it) {
        it.key()->setEnabled(false);
    }
}
