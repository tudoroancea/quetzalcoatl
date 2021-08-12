//
// Created by Tudor Oancea on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once

#ifndef QUETZALCOATL_MAINWINDOW_HPP
#define QUETZALCOATL_MAINWINDOW_HPP

#include "Game.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
private:
    Game* game;
    int score = 0;
    int bestScore = 0;
    QLabel* scoreLabel;
    QLabel* bestScoreLabel;
    /**
     * Si égal à 0 alors le timer est arrêté. Sinon indique l'id du timer.
     */
    int timerId = 0;
    int simulationSpeed = simulationBaseSpeed;

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

public:
    explicit MainWindow();

public slots:
    void startTimer();
    void killTimer();
};


#endif  // QUETZALCOATL_MAINWINDOW_HPP
