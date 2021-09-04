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
#include <QPropertyAnimation>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(int headProperty MEMBER m_headProperty WRITE setHeadProperty NOTIFY headPropertyChanged)

private:
    Game m_game;
    int m_bestScore = 0;
    QList<QLabel*> m_statusLabels;
    int m_simulationSpeed = simulationTabSpeed[0];
    QTimer m_simulationTimer = QTimer();
    QPropertyAnimation animation = QPropertyAnimation(this, "headProperty");

    void hideLabels() const;
    void showLabels() const;
    void startSimulationTimer();
    void stopSimulationTimer();

    int m_headProperty;

private slots:
    void simulationTimerEvent();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    // void timerEvent(QTimerEvent* event) override;

public:
    explicit MainWindow();

    void setHeadProperty(int headProperty);

public:
signals:
    void headPropertyChanged();
};


#endif  // QUETZALCOATL_MAINWINDOW_HPP
