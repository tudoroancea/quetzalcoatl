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
#include <QTimer>
#include <optional>

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
    QTimer m_simulationTimer = QTimer();
    QTimer m_changeDirectionTimer = QTimer();
    QTimer m_repaintTimer = QTimer();

    /**
     * Si égal à 0 alors le timer est arrêté. Sinon indique l'id du timer.
     */
    int m_timerId = 0;
    int m_simulationSpeed = simulationTabSpeed[0];
    QTimer m_simulationTimer = QTimer();
    QPropertyAnimation animation = QPropertyAnimation(this, "headProperty");

    std::optional<Direction> delayedDirection = std::nullopt;

    void hideLabels() const;
    void showLabels() const;
    void startSimulationTimer();
    void stopSimulationTimer();
    void simulationTimerEvent();
    void startChangeDirectionTimer();
    void stopChangeDirectionTimer();
    void startRepaintTimer();
    void stopRepaintTimer();
    /**
     * The name cannot ever be changed. EVER.
     * Oh you still want to ? ok but it's gonna be, wait for it…
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     * not legendary
     */
    [[nodiscard]] unsigned int himym() const;

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
