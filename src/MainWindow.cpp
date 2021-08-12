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


MainWindow::MainWindow() : game(new Game()), statusLabels({new QLabel("Current Score : "), new QLabel("0"), new QLabel("Best Score : "), new QLabel("0")}) {
    this->setCentralWidget(new QWidget);
    this->setWindowTitle(PROJECT_NAME);
    this->setMinimumSize(550, 550);
    this->resize(550, 550);
    QList screens(QGuiApplication::screens());
    this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
    this->setUnifiedTitleAndToolBarOnMac(true);
    for (const auto& label : statusLabels) {
        this->statusBar()->addWidget(label);
    }
    this->hideLabels();
#ifdef RELEASE_MODE
    this->statusBar()->addPermanentWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#endif
    this->statusBar()->showMessage("Press space bar to start snake");
    this->show();
    this->setFocus();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    int height = this->size().height();
    int width = this->size().width();
    int maxi = std::min(width, height - 20);
    int x0 = (width-maxi)/2+10;
    int y0 = (height-maxi)/2+5;
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
            if (!game->getHasBegun()) {
                this->startTimer();
                game->begin();
                this->statusBar()->clearMessage();
                this->showLabels();
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
    int score = (int) game->getScore();
    statusLabels[1]->setNum(score);
    if (score > bestScore) {
        bestScore = score;
        statusLabels[3]->setNum(bestScore);
    }
    if (game->getIsFinished()) {
        this->QObject::killTimer(timerId);
        timerId = 0;
        QMessageBox::StandardButton result = QMessageBox::information(this, "You have been killed", "You have been killed. Better luck next time !\nDo you want to try again?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result == QMessageBox::Yes) {
            delete game;
            game = new Game();
            this->update();
            this->hideLabels();
            statusLabels[1]->setNum(0);
            this->statusBar()->showMessage("Press space bar to start snake");
        }
    }
}

void MainWindow::startTimer() {
    int score = game->getScore();
    if (score < 5) simulationSpeed = simulationTabSpeed[0];
    else if (score < 10)
        simulationSpeed = simulationTabSpeed[1];
    else if (score < 15)
        simulationSpeed = simulationTabSpeed[2];
    else if (score < 20)
        simulationSpeed = simulationTabSpeed[3];
    else if (score < 25)
        simulationSpeed = simulationTabSpeed[4];
    else if (score < 30)
        simulationSpeed = simulationTabSpeed[5];
    else if (score < 50)
        simulationSpeed = simulationTabSpeed[6];
    timerId = this->QObject::startTimer(simulationSpeed, Qt::PreciseTimer);
}
void MainWindow::hideLabels() const {
    for (const auto& label : statusLabels) {
        label->hide();
    }
}
void MainWindow::showLabels() const {
    for (const auto& label : statusLabels) {
        label->show();
    }
}
