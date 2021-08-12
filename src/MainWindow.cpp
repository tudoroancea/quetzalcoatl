//
// Created by Tudor Oancea on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "MainWindow.hpp"
//#include "global.h"

#include <iostream>

#include <QApplication>
#include <QLabel>
#include <QPainter>
#include <QScreen>
#include <QStatusBar>


MainWindow::MainWindow() : game(new Game()), scoreLabel(new QLabel), bestScoreLabel(new QLabel("0")) {
    scoreLabel->setNum((int) game->getScore());

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
    std::cerr << window.width() << ", " << window.height() << ", " << window.x() << ", " << window.y() << std::endl;
}
