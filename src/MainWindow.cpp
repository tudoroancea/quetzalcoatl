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
#include <QSound>
#include <QStatusBar>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
MainWindow::MainWindow()
    : m_game(),
      m_statusLabels({new QLabel("Current Score : "),
                      new QLabel("0"),
                      new QLabel("Best Score : "),
                      new QLabel("0")})
// clang-format off
{
    // clang-format on
    //    Status bar =====================================
    for (const auto& label : m_statusLabels) {
        this->statusBar()->addWidget(label);
    }
    this->hideLabels();
#ifdef RELEASE_MODE
    this->statusBar()->addPermanentWidget(new QLabel(QString("Version ").append(PROJECT_VERSION)));
#endif
    this->statusBar()->showMessage("Press space bar to start snake");

    m_simulationTimer.setInterval(m_simulationSpeed);
    connect(&m_simulationTimer, SIGNAL(timeout()), this, SLOT(simulationTimerEvent()), Qt::UniqueConnection);
    connect(this, SIGNAL(headPropertyChanged()), this, SLOT(update()));

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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
void MainWindow::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    // Defining the painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
#pragma clang diagnostic pop

    // Some useful variables for drawing the grid
    int windowHeight = this->size().height();
    int windowWidth = this->size().width();
    int maxi = std::min(windowWidth, windowHeight - 20);
    int x0 = (windowWidth - maxi) / 2 + 10;
    int y0 = (windowHeight - maxi) / 2 + 5;
    double unit((maxi - 20) / gridSize);  // NOLINT(bugprone-integer-division)
    int margin(1);  // the numbers of pixels between the border of a cell and the snake. On each side

    // Drawing the grid
    for (unsigned i(0); i < gridSize; i++) {
        for (unsigned j(0); j < gridSize; j++) {
            painter.fillRect(i * unit + x0, j * unit + y0, unit, unit, ((i + j) % 2 == 0 ? darkGreen : lightGreen));
        }
    }

    // Some useful lambdas (used as nested functions) for drawing the head and the tail
    [[maybe_unused]] auto drawHead = [&]() {
        // first part
        painter.fillRect(0, margin, m_headProperty + unit / 2, unit - 2 * margin, darkBlue);  // NOLINT(cppcoreguidelines-narrowing-conversions)

        // round part
        int startAngle = 270 * 16;
        int spanAngle = 180 * 16;
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        painter.drawChord(m_headProperty, 2 * margin, unit - 2 * margin, unit - 4 * margin, 270 * 16, 180 * 16);

        // drawing eyes
        painter.setBrush(Qt::white);
        painter.setPen(Qt::white);
        painter.drawEllipse(m_headProperty + 0.15 * unit, 0.05 * unit, 0.5 * unit, 0.4 * unit);
        painter.drawEllipse(m_headProperty + 0.15 * unit, 0.55 * unit, 0.5 * unit, 0.4 * unit);

        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);
        painter.drawEllipse(m_headProperty + 0.35 * unit, 0.15 * unit, 0.2 * unit, 0.2 * unit);
        painter.drawEllipse(m_headProperty + 0.35 * unit, 0.65 * unit, 0.2 * unit, 0.2 * unit);
    };
    auto drawSemiCircle = [&]() {
        // rectangle part
        painter.fillRect(0, margin, unit / 2, unit - 2 * margin, darkBlue);  // NOLINT(cppcoreguidelines-narrowing-conversions)

        // round part
        int startAngle = 270 * 16;
        int spanAngle = 180 * 16;
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        painter.drawChord(0, 2 * margin, unit - 2 * margin, unit - 4 * margin, startAngle, spanAngle);
    };

    auto drawSemiCircleWithEyes = [&]() {
        drawSemiCircle();
        painter.setBrush(Qt::white);
        painter.setPen(Qt::white);

        QRectF rectangle(0.15 * unit, 0.05 * unit, 0.5 * unit, 0.4 * unit);
        painter.drawEllipse(rectangle);

        QRectF rect(0.15 * unit, 0.55 * unit, 0.5 * unit, 0.4 * unit);
        painter.drawEllipse(rect);

        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);

        QRectF rectangle2(0.35 * unit, 0.15 * unit, 0.2 * unit, 0.2 * unit);
        painter.drawEllipse(rectangle2);

        QRectF rect2(0.35 * unit, 0.65 * unit, 0.2 * unit, 0.2 * unit);
        painter.drawEllipse(rect2);
    };
    auto drawAngle = [&]() {
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        painter.drawChord(QRectF(-unit, -unit, 2 * unit - 1, 2 * unit - 1), 270 * 16, 90 * 16);
        QPolygonF triangle;
        triangle << QPointF(0.5, 0.5) << QPointF(0.5, unit) << QPointF(unit, 0.5);
        painter.drawConvexPolygon(triangle);
    };

    Coord prevTail(m_game.snake().tail(1));
    Snake const& snake(m_game.snake());

    for (unsigned i(0); i < snake.size(); i++) {
        painter.translate(snake[i].first * unit + x0, snake[i].second * unit + y0);
        if (i == 0) {  // drawing the head
            if (snake.direction() == Down) {  // Down
                painter.rotate(90);
                painter.translate(0, -unit);
            } else if (snake.direction() == Up) {  // Up
                painter.rotate(270);
                painter.translate(-unit, 0);
            } else if (snake.direction() == Left) {  // Left
                painter.rotate(180);
                painter.translate(-unit, -unit);
            } else if (snake.direction() == Right) {  // Right
                // Nothing to do
            }
            // drawSemiCircleWithEyes();
            drawHead();
        } else if (i == snake.size() - 1) {  // drawing the tail
            Coord currentTail(snake.tail());
            if (currentTail + Coord{0, 1} == prevTail) {  // Down
                painter.rotate(270);
                painter.translate(-unit, 0);
            } else if (currentTail + Coord{0, -1} == prevTail) {  // Up
                painter.rotate(90);
                painter.translate(0, -unit);
            } else if (currentTail + Coord{1, 0} == prevTail) {  // Right
                painter.rotate(180);
                painter.translate(-unit, -unit);
            } else if (currentTail + Coord{-1, 0} == prevTail) {  // Left
                // Nothing to do
            }
            drawSemiCircle();
        } else {
            if ((snake[i - 1] == snake[i] + Coord{1, 0} && snake[i + 1] == snake[i] + Coord{-1, 0}) ||
                (snake[i - 1] == snake[i] + Coord{-1, 0} && snake[i + 1] == snake[i] + Coord{1, 0}) ||
                (snake[i - 1] == snake[i] + Coord{0, 1} && snake[i + 1] == snake[i] + Coord{0, -1}) ||
                (snake[i - 1] == snake[i] + Coord{0, -1} && snake[i + 1] == snake[i] + Coord{0, 1})) {
                // Drawing normal body cell
                painter.fillRect(QRect(0, 0, unit, unit), darkBlue);
            } else {
                // Drawing corner body cell
                if ((snake[i - 1] == snake[i] + Coord{0, -1} && snake[i + 1] == snake[i] + Coord{1, 0}) ||
                    (snake[i + 1] == snake[i] + Coord{0, -1} && snake[i - 1] == snake[i] + Coord{1, 0})) {
                    // drawing right/up angle
                    painter.rotate(90);
                    painter.translate(0, -unit);
                } else if ((snake[i - 1] == snake[i] + Coord{1, 0} && snake[i + 1] == snake[i] + Coord{0, 1}) ||
                           (snake[i + 1] == snake[i] + Coord{1, 0} && snake[i - 1] == snake[i] + Coord{0, 1})) {
                    // drawing right/down angle
                    painter.rotate(180);
                    painter.translate(-unit, -unit);
                } else if ((snake[i - 1] == snake[i] + Coord{-1, 0} && snake[i + 1] == snake[i] + Coord{0, 1}) ||
                           (snake[i + 1] == snake[i] + Coord{-1, 0} && snake[i - 1] == snake[i] + Coord{0, 1})) {
                    // drawing left/down angle
                    painter.rotate(270);
                    painter.translate(-unit, 0);
                } else if ((snake[i - 1] == snake[i] + Coord{0, -1} && snake[i + 1] == snake[i] + Coord{-1, 0}) ||
                           (snake[i + 1] == snake[i] + Coord{0, -1} && snake[i - 1] == snake[i] + Coord{-1, 0})) {
                    // drawing left/up angle
                    // Nothing to do
                }
                drawAngle();
            }
        }
        painter.resetTransform();
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
}
#pragma clang diagnostic pop

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (!m_game.hasBegun() && (event->key() == Qt::Key_Space || event->key() == Qt::Key_Down || event->key() == Qt::Key_Up || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)) {
        m_game.begin();
        this->statusBar()->clearMessage();
        this->showLabels();
        m_simulationSpeed = simulationTabSpeed[0];
        this->startSimulationTimer();
        QSound::play(":/media/start.wav");
    } else {
        switch (event->key()) {
            case Qt::Key_Up:
                m_game.snake().setDirection(Up);
                break;
            case Qt::Key_Down:
                m_game.snake().setDirection(Down);
                break;
            case Qt::Key_Left:
                m_game.snake().setDirection(Left);
                break;
            case Qt::Key_Right:
                m_game.snake().setDirection(Right);
                break;
            default:
                break;
        }
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
    if (score == 225) {
        /*
         * TODO : create special event for winning the game
        this->stopSimulationTimer(;
        QDialog dialog;
        QLabel final("<a href=\"https://youtu.be/dQw4w9WgXcQ\">Vous êtes arrivés à la fin du jeu. Voici votre récompense. (ce n'est pas un virus)</a>", &dialog);
        final.setTextFormat(Qt::RichText);
        final.setTextInteractionFlags(Qt::TextBrowserInteraction);
        final.setOpenExternalLinks(true);
        dialog.show();
        dialog.setFocus();
        dialog.raise();
        dialog.activateWindow();*/
    }

    if (m_game.isFinished()) {
        QSound::play(":/media/fail.wav");
        this->stopSimulationTimer();
        QMessageBox::StandardButton result = QMessageBox::information(this, "You have been killed", "You have been killed. Better luck next time !\nDo you want to try again?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result == QMessageBox::Yes) {
            m_game = Game();
            this->update();
            this->hideLabels();
            m_statusLabels[1]->setNum(0);
            this->statusBar()->showMessage("Press space bar to start snake");
        }
    } else {
        for (int i(1); i <= 6; ++i) {
            if (score == 5 * i) {
                this->stopSimulationTimer();
                m_simulationSpeed = simulationTabSpeed[i];
                this->startSimulationTimer();  // also replace the timerId with the new one
            }
        }
    }
}

void MainWindow::hideLabels() const {
    for (auto const& label : m_statusLabels) {
        label->hide();
    }
}

void MainWindow::showLabels() const {
    for (auto const& label : m_statusLabels) {
        label->show();
    }
}
void MainWindow::simulationTimerEvent() {
    m_game.update();

    animation.stop();
    animation.setDuration(m_simulationSpeed);
    animation.setStartValue(int(0));
    animation.setEndValue(int((std::min(this->size().width(), this->size().height() - 20) - 20) / gridSize));
    animation.start();

    int score = (int) m_game.score();
    m_statusLabels[1]->setNum(score);
    if (score > m_bestScore) {
        m_bestScore = score;
        m_statusLabels[3]->setNum(m_bestScore);
    }
    if (m_game.isFinished()) {
        this->stopSimulationTimer();
        QSound::play(":/media/fail.wav");
        QMessageBox::StandardButton result = QMessageBox::information(this, "You have been killed", "You have been killed. Better luck next time !\nDo you want to try again?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (result == QMessageBox::Yes) {
            m_game = Game();
            this->update();
            this->hideLabels();
            this->setHeadProperty(0);
            m_statusLabels[1]->setNum(0);
            this->statusBar()->showMessage("Press space bar to start snake");
        }
    } else {
        for (int i(1); i <= 6; ++i) {
            if (score == 5 * i) {
                this->stopSimulationTimer();
                m_simulationSpeed = simulationTabSpeed[i];
                this->startSimulationTimer();  // also replace the timerId with the new one
            }
        }
    }
}

void MainWindow::startSimulationTimer() {
    m_simulationTimer.start();
}

void MainWindow::stopSimulationTimer() {
    m_simulationTimer.stop();
}

void MainWindow::setHeadProperty(int headProperty) {
    m_headProperty = headProperty;
    emit headPropertyChanged();
}

#pragma clang diagnostic pop