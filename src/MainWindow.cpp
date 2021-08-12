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


MainWindow::MainWindow()
    : m_game(),
      m_statusLabels({new QLabel("Current Score : "),
                      new QLabel("0"),
                      new QLabel("Best Score : "),
                      new QLabel("0")}) {
    //    Status bar =====================================
    for (const auto& label : m_statusLabels) {
        this->statusBar()->addWidget(label);
    }
    this->hideLabels();
#ifdef RELEASE_MODE
    this->statusBar()->addPermanentWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#endif
    this->statusBar()->showMessage("Press space bar to start snake");

    //    Window misc =================================
    this->setCentralWidget(new QWidget);
    this->setWindowTitle(PROJECT_NAME);
    this->setMinimumSize(550, 550);
    this->resize(550, 550);
    QList screens(QGuiApplication::screens());
    this->move(screens[(screens.size() > 1 ? 1 : 0)]->geometry().center() - frameGeometry().center());
    this->setUnifiedTitleAndToolBarOnMac(true);
    this->show();
    this->setFocus();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);


    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    int height = this->size().height();
    int width = this->size().width();
    int maxi = std::min(width, height - 20);
    int x0 = (width - maxi) / 2 + 10;
    int y0 = (height - maxi) / 2 + 5;
    double unit((maxi - 20) / gridSize);  // NOLINT(bugprone-integer-division)
    for (unsigned i(0); i < gridSize; i++) {
        for (unsigned j(0); j < gridSize; j++) {
            QRect background(i * unit + x0, j * unit + y0, unit, unit);
            if ((i + j) % 2 == 0) {
                painter.fillRect(background, darkGreen);
            } else {
                painter.fillRect(background, lightGreen);
            }
        }
    }
    List body = m_game.snake().body();
    for (auto const& box : body) {
        QRect background(box.first * unit + x0, box.second * unit + y0, unit, unit);  // NOLINT(cppcoreguidelines-narrowing-conversions)
        painter.fillRect(background, darkBlue);
    }

    // DRAW APPLE =====================

    QRectF rectangle(m_game.apple().first * unit + x0 + 0.2 * unit, m_game.apple().second * unit + y0 + 0.3 * unit, 0.6 * unit, 0.6 * unit);
    painter.setPen(darkRed);
    painter.setBrush(darkRed);
    painter.drawEllipse(rectangle);

    QRectF rect(m_game.apple().first * unit + x0 + 0.5 * unit, m_game.apple().second * unit + y0 + 0.15 * unit, 0.4 * unit, 0.4 * unit);
    int startAngle = 70 * 16;
    int spanAngle = 130 * 16;
    painter.setPen(appleGreen);
    painter.setBrush(appleGreen);
    painter.drawChord(rect, startAngle, spanAngle);


    // QRect background(game->getApple().first*unit+x0, game->getApple().second*unit+y0, unit, unit);
    // painter.fillRect(background, darkRed);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:
            if (m_game.snake().direction() != Down) m_game.snake().setDirection(Up);
            break;
        case Qt::Key_Down:
            if (m_game.snake().direction() != Up) m_game.snake().setDirection(Down);
            break;
        case Qt::Key_Left:
            if (m_game.snake().direction() != Right) m_game.snake().setDirection(Left);
            break;
        case Qt::Key_Right:
            if (m_game.snake().direction() != Left) m_game.snake().setDirection(Right);
            break;
        case Qt::Key_Space: {
            if (!m_game.hasBegun()) {
                m_game.begin();
                this->statusBar()->clearMessage();
                this->showLabels();
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
    m_game.update();
    this->update();  // redessine le jeu
    int score = (int) m_game.score();
    m_statusLabels[1]->setNum(score);
    if (score > m_bestScore) {
        m_bestScore = score;
        m_statusLabels[3]->setNum(m_bestScore);
    }
    if (m_game.isFinished()) {
        this->QObject::killTimer(m_timerId);
        m_timerId = 0;
        QMessageBox::StandardButton result = QMessageBox::information(this, "You have been killed", "You have been killed. Better luck next time !\nDo you want to try again?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result == QMessageBox::Yes) {
            m_game = Game();
            this->update();
            m_simulationSpeed = simulationTabSpeed[0];
            this->hideLabels();
            m_statusLabels[1]->setNum(0);
            this->statusBar()->showMessage("Press space bar to start snake");
        }
    } else {
        for (int i(1); i <= 6; ++i) {
            if (score == 2 * i) {
                m_simulationSpeed = simulationTabSpeed[i];
                this->QObject::killTimer(m_timerId);
                this->startTimer();  // also replace the timerId with the new one
            }
        }
    }
}

void MainWindow::startTimer() {
    m_timerId = this->QObject::startTimer(m_simulationSpeed, Qt::PreciseTimer);
}
void MainWindow::hideLabels() const {
    for (const auto& label : m_statusLabels) {
        label->hide();
    }
}
void MainWindow::showLabels() const {
    for (const auto& label : m_statusLabels) {
        label->show();
    }
}
