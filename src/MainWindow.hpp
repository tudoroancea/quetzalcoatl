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
    Game m_game;
    int m_bestScore = 0;
    QList<QLabel*> m_statusLabels;
    /**
     * Si égal à 0 alors le timer est arrêté. Sinon indique l'id du timer.
     */
    int m_timerId = 0;
    int m_simulationSpeed = simulationTabSpeed[0];

    void hideLabels() const;
    void showLabels() const;
    void startTimer();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

public:
    explicit MainWindow();
};


#endif  // QUETZALCOATL_MAINWINDOW_HPP
