//
// Created by Tudor Oancea on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "MainWindow.hpp"
#include "global.h"

#include <iostream>

#include <QApplication>
#include <QLabel>
#include <QPainter>
#include <QScreen>
#include <QStatusBar>


MainWindow::MainWindow() : game(new Game()), scoreLabel(new QLabel), bestScoreLabel(new QLabel("0")) {
    scoreLabel->setNum((int) game->getScore());
    connect(game, SIGNAL(error()), this, SLOT(killTimer()));

    this->setCentralWidget(new QWidget);
    this->setWindowTitle(PROJECT_NAME);
    this->setMinimumSize(500, 500);
    this->resize(500, 500);
    QList screens(QGuiApplication::screens());
    this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
    this->setUnifiedTitleAndToolBarOnMac(true);
    this->statusBar()->addWidget(new QLabel("Current Score : "));
    this->statusBar()->addWidget(scoreLabel);
    this->statusBar()->addWidget(new QLabel("  "), 1);
    this->statusBar()->addWidget(new QLabel("Best Score : "));
    this->statusBar()->addWidget(bestScoreLabel);
#ifdef RELEASE_MODE
    this->statusBar()->addPermanentWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#else
    this->statusBar()->showMessage("Press space bar to start snake");
#endif
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
    QRect background(x0,y0,size,size);
    painter.fillRect(background, Qt::black);
    double unit((maxi-20)/gridSize);
    for(unsigned i(0); i < gridSize; i++) {
        for(unsigned j(0); j < gridSize; j++) {
            QRect background(i*unit+x0, j*unit+y0, i*unit+unit+x0, j*unit+unit+y0);
            if((i+j)%2 == 0) {
                painter.fillRect(background, darkGreen);
            }else {
                painter.fillRect(background, lightGreen);
            }
        }
    }

    //std::cerr << window.width() << ", " << window.height() << ", " << window.x() << ", " << window.y() << std::endl;
}
void MainWindow::killTimer() {
    this->QObject::killTimer(timerId);
}
void MainWindow::startTimer() {
    timerId = this->QObject::startTimer(simulationSpeed, Qt::PreciseTimer);
}
