//
// Created by Tudor Oancea on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "MainWindow.hpp"
#include "global.h"

#include <iostream>

#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QScreen>
#include <QStatusBar>


MainWindow::MainWindow() : game(new Game()), scoreLabel(new QLabel("0")), bestScoreLabel(new QLabel("0")) {
    connect(game, &Game::error, this, &MainWindow::killTimer);

    this->setCentralWidget(new QWidget);
    this->setWindowTitle(PROJECT_NAME);
    this->setMinimumSize(550, 550);
    this->resize(550, 550);
    QList screens(QGuiApplication::screens());
    this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
    this->setUnifiedTitleAndToolBarOnMac(true);
//    this->initScoreLabels();
#ifdef RELEASE_MODE
    this->statusBar()->addPermanentWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#endif
    this->statusBar()->showMessage("Press space bar to start snake");
    this->show();
    this->setFocus();
}

void MainWindow::initScoreLabels() const {
    statusBar()->addWidget(new QLabel("Current Score : "));
    statusBar()->addWidget(scoreLabel);
    statusBar()->addWidget(new QLabel("Best Score : "));
    statusBar()->addWidget(bestScoreLabel);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    QRect window(painter.window());

    int height = this->size().height();
    int width = this->size().width();
    int maxi = std::min(width, height - 20);
    // painter.setWindow((width-maxi)/2+10, (height-maxi)/2+5, maxi-20, maxi-20));
    int x0 = (width-maxi)/2+10;
    int y0 = (height-maxi)/2+5;
    int size = maxi-20;
    double unit((maxi-20)/gridSize);
    for(unsigned i(0); i < gridSize; i++) {
        for(unsigned j(0); j < gridSize; j++) {
            QRect background(i*unit+x0, j*unit+y0, unit, unit);
            if ((i + j) % 2 == 0) {
                painter.fillRect(background, darkGreen);
            } else {
                painter.fillRect(background, lightGreen);
            }
        }
    }
    List body = game->getSnake().getBody();
    for(auto const& box : body) {
        QRect background(box.first*unit+x0, box.second*unit+y0, unit, unit);
        painter.fillRect(background, darkBlue);
    }

    QRect background(game->getApple().first*unit+x0, game->getApple().second*unit+y0, unit, unit);
    painter.fillRect(background, darkRed);

    // std::cerr << window.width() << ", " << window.height() << ", " << window.x() << ", " << window.y() << std::endl;
}
void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:
            if (game->getSnake().getDirection() != Down) game->getSnake().setDirection(Up);
            break;
        case Qt::Key_Down:
            if (game->getSnake().getDirection() != Up) game->getSnake().setDirection(Down);
            break;
        case Qt::Key_Left:
            if (game->getSnake().getDirection() != Right) game->getSnake().setDirection(Left);
            break;
        case Qt::Key_Right:
            if (game->getSnake().getDirection() != Left) game->getSnake().setDirection(Right);
            break;
        case Qt::Key_Space: {
            if (timerId == 0) {
                this->initScoreLabels();
                this->startTimer();
                this->statusBar()->clearMessage();
            }
            break;
        }
        default:
            break;
    }
}

void MainWindow::timerEvent(QTimerEvent* event) {
    QObject::timerEvent(event);
    game->update();
    this->update();
    score = (int) game->getScore();
    scoreLabel->setNum(score);
    if (score > bestScore) {
        bestScore = score;
        bestScoreLabel->setNum(bestScore);
    }
}

void MainWindow::killTimer() {
    this->QObject::killTimer(timerId);
    timerId = 0;
    QMessageBox::StandardButton result = QMessageBox::information(this, "You have been killed", "You have been killed. better luck next time !\nDo you want to try again?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (result == QMessageBox::Yes) {
        delete game;
        game = new Game();
        this->update();
        this->statusBar()->showMessage("Press space bar to start snake");
    } else {
        /*
         *  TODO : Est-ce que si on répond non on ferme l'appli ?
         */
    }
}
void MainWindow::startTimer() {
    timerId = this->QObject::startTimer(simulationSpeed, Qt::PreciseTimer);
}
