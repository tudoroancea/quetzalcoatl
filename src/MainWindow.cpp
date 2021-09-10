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
#include <QPainterPath>
#include <QPropertyAnimation>
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

    // Timers ===============
    m_changeDirectionTimer.setSingleShot(true);
    m_repaintTimer.callOnTimeout([&]() {
        this->update();
    });

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
    auto drawFractionBody = [&](enum Direction dir, double x, double y, double step) {
        QRectF rect;
        switch (dir) {
            case Right:
                rect = QRectF(0, 0, unit / steps, unit);
                // painter.resetTransform();
                painter.translate(step, 0);
                break;
            case Left:
                rect = QRectF(0, 0, -unit / steps, unit);
                // painter.resetTransform();
                painter.translate(-step, 0);
                break;
            case Up:
                rect = QRectF(0, 0, unit, -unit / steps);
                // painter.resetTransform();
                painter.translate(0, -step);
                break;
            case Down:
                rect = QRectF(0, 0, unit, unit / steps);
                // painter.resetTransform();
                painter.translate(0, step);
                break;
        }
        painter.fillRect(rect, darkBlue);
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
        // default = left to up angle
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        //  painter.drawChord(QRectF(-unit, -unit, 2 * unit - 1, 2 * unit - 1), 270 * 16, 90 * 16);
        QPainterPath path;
        // qDebug("%d,%d",path.currentPosition().x(), path.currentPosition().y());
        path.moveTo(unit * 0.25, 0);
        path.arcTo(-unit * 0.25, -unit * 0.25, unit * 0.5, unit * 0.5, 0, -90);
        // qDebug("%d,%d",path.currentPosition().x(), path.currentPosition().y());
        path.lineTo(0, unit * 0.75);
        path.arcTo(-unit * 0.75, -unit * 0.75, unit * 1.5, unit * 1.5, 270, 90);
        path.closeSubpath();
        painter.drawPath(path);
        // QPolygonF triangle;
        // triangle << QPointF(0.5, 0.5) << QPointF(0.5, unit) << QPointF(unit, 0.5);
        // painter.drawConvexPolygon(triangle);
    };


    Snake const& snake(m_game.snake());

    unsigned tic = himym();

    for (unsigned i(0); i < snake.size(); i++) {
        double x = snake[i].first * unit + x0;
        double y = snake[i].second * unit + y0;
        painter.translate(x, y);
        if (i == 0) {  // drawing the head
            for (unsigned j(0); j < tic; j++) {
                drawFractionBody(snake.direction(), x, y, j / steps);
            }
            if (snake.direction() == Down) {  // Down
                painter.rotate(90);
                painter.translate(0, -unit);
                if (head == preHead + Coord{0, -1}) {
                    orientation = Straight;
                } else if (head == preHead + Coord{-1, 0}) {
                    orientation = RightTurn;
                } else if (head == preHead + Coord{1, 0}) {
                    orientation = LeftTurn;
                } else {
                    // backwards
                }
            } else if (snake.direction() == Up) {  // Up
                painter.rotate(270);
                painter.translate(-unit, 0);
                if (head == preHead + Coord{0, 1}) {
                    orientation = Straight;
                } else if (head == preHead + Coord{1, 0}) {
                    orientation = RightTurn;
                } else if (head == preHead + Coord{-1, 0}) {
                    orientation = LeftTurn;
                } else {
                    // backwards
                }
            } else if (snake.direction() == Left) {  // Left
                painter.rotate(180);
                painter.translate(-unit, -unit);
                if (head == preHead + Coord{-1, 0}) {
                    orientation = Straight;
                } else if (head == preHead + Coord{0, 1}) {
                    orientation = RightTurn;
                } else if (head == preHead + Coord{0, -1}) {
                    orientation = LeftTurn;
                } else {
                    // backwards
                }
            } else if (snake.direction() == Right) {  // Right
                // Nothing to do
                if (head == preHead + Coord{-1, 0}) {
                    orientation = Straight;
                } else if (head == preHead + Coord{0, -1}) {
                    orientation = RightTurn;
                } else if (head == preHead + Coord{0, 1}) {
                    orientation = LeftTurn;
                } else {
                    // backwards
                }
            }
            // drawHeadWithoutNeck();
            drawHead(orientation);
        } else if (i < snake.size() - 2) {
            // drawing the body

            if ((snake[i - 1] == snake[i] + Coord{1, 0} && snake[i + 1] == snake[i] + Coord{-1, 0}) ||
                (snake[i - 1] == snake[i] + Coord{-1, 0} && snake[i + 1] == snake[i] + Coord{1, 0})) {
                // drawing horizontal body cell
                painter.fillRect(0, unit * 0.25, unit, unit * 0.5, darkBlue);
            } else if ((snake[i - 1] == snake[i] + Coord{0, 1} && snake[i + 1] == snake[i] + Coord{0, -1}) ||
                       (snake[i - 1] == snake[i] + Coord{0, -1} && snake[i + 1] == snake[i] + Coord{0, 1})) {
                // Drawing vertical body cell
                painter.fillRect(unit * 0.25, 0, unit * 0.5, unit, darkBlue);
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
        } else {  // drawing the tail and its neighbor
            Coord currentTail(snake.tail());
            Coord prevTail(snake.tail(1));
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
        if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
            Direction newDirection;
            switch (event->key()) {
                case Qt::Key_Up:
                    newDirection = Up;
                    break;
                case Qt::Key_Down:
                    newDirection = Down;
                    break;
                case Qt::Key_Left:
                    newDirection = Left;
                    break;
                case Qt::Key_Right:
                    newDirection = Right;
                    break;
                default:
                    break;
            }
            // if (m_changeDirectionTimer.isActive()) {
            //     // on change de direction mtn
            //     this->stopSimulationTimer();
            m_game.snake().setDirection(newDirection);
            //     this->startSimulationTimer();
            // } else {
            //     delayedDirection = newDirection;
            // }
        }
    }
}

void MainWindow::timerEvent(QTimerEvent* event) {
    QObject::timerEvent(event);
    /*m_repaintTimer.stop();
    if (delayedDirection) {
        m_game.snake().setDirection(delayedDirection.value());
        delayedDirection = std::nullopt;
    }
    m_game.update();
    m_changeDirectionTimer.setSingleShot(true);
    m_repaintTimer.setTimerType(Qt::PreciseTimer);
    m_repaintTimer.callOnTimeout([&]() {
        this->update();
        qDebug("%d | %d", himym(), m_repaintTimer.remainingTime());
    });
    qDebug("%d", m_simulationSpeed/steps);
    m_repaintTimer.setInterval(m_simulationSpeed/steps);
    m_repaintTimer.start();
    qDebug("%d", m_repaintTimer.isActive());
    int score = (int) m_game.score();
    m_statusLabels[1]->setNum(score);
    if (score > m_bestScore) {
        m_bestScore = score;
        m_statusLabels[3]->setNum(m_bestScore);
    }
    if (score == 225) {
        */
    /*
   * TODO : create special event for winning the game
  this->QObject::killTimer(m_timerId);
  m_timerId = 0;
  QDialog dialog;
  QLabel final("<a href=\"https://youtu.be/dQw4w9WgXcQ\">Vous êtes arrivés à la fin du jeu. Voici votre récompense. (ce n'est pas un virus)</a>", &dialog);
  final.setTextFormat(Qt::RichText);
  final.setTextInteractionFlags(Qt::TextBrowserInteraction);
  final.setOpenExternalLinks(true);
  dialog.show();
  dialog.setFocus();
  dialog.raise();
  dialog.activateWindow();*/
    /*
}

if (m_game.isFinished()) {
QSound::play(":/media/fail.wav");
m_repaintTimer.stop();
this->stopSimulationTimer();
m_timerId = 0;
QMessageBox::StandardButton result = QMessageBox::information(this, "You have been killed", "You have been killed. Better luck next time !\nDo you want to try again?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
if (result == QMessageBox::Yes) {
m_game = Game();
this->update();
this->hideLabels();
m_statusLabels[1]->setNum(0);
this->statusBar()->showMessage("Press space bar to start snake");
}
} else {
for (int i(1); i <= 3; ++i) {
if (score == 5 * i) {
this->stopSimulationTimer();
m_simulationSpeed = simulationTabSpeed[i];
this->startSimulationTimer();  // also replace the timerId with the new one
}
}
this->startChangeDirectionTimer();
}*/
}

void MainWindow::startSimulationTimer() {
    m_simulationTimer.setTimerType(Qt::PreciseTimer);
    m_simulationTimer.callOnTimeout(this, &MainWindow::simulationTimerEvent);
    m_simulationTimer.start(m_simulationSpeed);
    qDebug("startSimulationTimer");
}

void MainWindow::stopSimulationTimer() {
    qDebug("stopSimulationTimer");
    m_simulationTimer.stop();
    this->stopRepaintTimer();
    this->stopChangeDirectionTimer();
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
void MainWindow::startChangeDirectionTimer() {
    qDebug("\t\t\tstartChangeDirectionTimer");
    m_changeDirectionTimer.setSingleShot(true);
    m_changeDirectionTimer.start(m_simulationSpeed / 2);
}
void MainWindow::stopChangeDirectionTimer() {
    m_changeDirectionTimer.stop();
    qDebug("\t\t\t\tstopChangeDirectionTimer");
}
unsigned int MainWindow::himym() const {
    if (m_repaintTimer.isActive()) {
        return steps * (1.0 - double(m_simulationTimer.remainingTime()) / double(m_simulationSpeed));
    } else {
        return steps;
    }
}

void MainWindow::simulationTimerEvent() {
    // m_repaintTimer.stop();
    // if (delayedDirection) {
    //     m_game.snake().setDirection(delayedDirection.value());
    //     delayedDirection = std::nullopt;
    // }
    m_game.update();

    int score = (int) m_game.score();
    m_statusLabels[1]->setNum(score);
    if (score > m_bestScore) {
        m_bestScore = score;
        m_statusLabels[3]->setNum(m_bestScore);
    }
    if (m_game.isFinished()) {
        this->stopSimulationTimer();
        QSound::play(":/media/fail.wav");
        m_repaintTimer.stop();
        this->stopSimulationTimer();
        this->stopChangeDirectionTimer();
        // this->stopRepaintTimer();
        m_timerId = 0;
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
        // for (int i(1); i <= 3; ++i) {
        //     if (score == 5 * i) {
        //         this->stopSimulationTimer();
        //         m_simulationSpeed = simulationTabSpeed[i];
        //         this->startSimulationTimer();  // also replace the timerId with the new one
        //     }
        // }
        this->startChangeDirectionTimer();  // we have to call this here, otherwise it will be called
        this->startRepaintTimer();
    }
}
void MainWindow::startRepaintTimer() {
    m_repaintTimer.setTimerType(Qt::PreciseTimer);
    m_repaintTimer.callOnTimeout([&]() {
        this->update();
        // qDebug("%d | %d", himym(), m_repaintTimer.remainingTime());
    });
    // qDebug("%d", m_simulationSpeed/steps);
    m_repaintTimer.setInterval(m_simulationSpeed / steps);
    m_repaintTimer.start();
    qDebug("\tstartRepaintTimer");
}
void MainWindow::stopRepaintTimer() {
    m_repaintTimer.stop();
    qDebug("\t\tstopRepaintTimer");
}

#pragma clang diagnostic pop