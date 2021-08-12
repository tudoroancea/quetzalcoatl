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
    this->statusBar()->addWidget(new QLabel("Current Score : "));
    this->statusBar()->addWidget(scoreLabel);
    this->statusBar()->addWidget(new QLabel("Best Score : "));
    this->statusBar()->addWidget(bestScoreLabel);
#ifdef RELEASE_MODE
    this->statusBar()->addPermanentWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#endif
    //    this->statusBar()->showMessage("Press space bar to start snake", 2000);
    this->show();
    this->setFocus();
}
void MainWindow::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    QRect window(painter.window());

    int height = this->size().height();
    int width = this->size().width();
    int maxi = std::min(width,height-20);
    //painter.setWindow((width-maxi)/2+10, (height-maxi)/2+5, maxi-20, maxi-20));
    int x0 = (width-maxi)/2+10;
    int y0 = (height-maxi)/2+5;
    int size = maxi-20;
    double unit((maxi-20)/gridSize);
    for(unsigned i(0); i < gridSize; i++) {
        for(unsigned j(0); j < gridSize; j++) {
            QRect background(i*unit+x0, j*unit+y0, unit, unit);
            std::cout << "on" << std::endl;
            if ((i + j) % 2 == 0) {
                painter.fillRect(background, darkGreen);
                std::cout << "off" << std::endl;
            } else {
                painter.fillRect(background, lightGreen);
                std::cout << "off" << std::endl;
            }
        }
    }
    List body = game->getSnake().getBody();
    for(auto const& box : body) {
        QRect background(box.first*unit+x0, box.second*unit+y0, unit, unit);
        painter.fillRect(background, darkBlue);
    }

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
                this->startTimer();
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
    score = (int) game->getScore();
    scoreLabel->setNum(score);
    if (score > bestScore) {
        bestScore = score;
        bestScoreLabel->setNum(bestScore);
    }
    /*
     * TODO : add message in paintEvent
     */
}

void MainWindow::killTimer() {
    this->QObject::killTimer(timerId);
    timerId = 0;
}
void MainWindow::startTimer() {
    timerId = this->QObject::startTimer(simulationSpeed, Qt::PreciseTimer);
}
