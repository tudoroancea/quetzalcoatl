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

    m_simulationTimer.setInterval(m_simulationSpeed);
    connect(&m_simulationTimer, SIGNAL(timeout()), this, SLOT(simulationTimerEvent()), Qt::UniqueConnection);
    connect(this, SIGNAL(headPropertyChanged()), this, SLOT(update()), Qt::UniqueConnection);

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
    auto drawHeadWithoutNeck = [&]() {
        // QPainterPath path;
        // path.moveTo(0,-unit*0.25);
        // path.lineTo(0,unit*0.25);
        // path.arcTo(0,unit*0.1875, unit*0.25, unit*0.125, 180,90);
        // path.arcTo(-unit*0.25, -unit*0.3125, unit*0.55, unit*0.625, 270,180);
        // path.arcTo(0,-unit*0.3125, unit*0.25, unit*0.0625, 90,90);
        // path.closeSubpath();
        // painter.translate(0,unit*0.5);
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        painter.drawChord(-unit * 0.5, unit * 0.25, unit, unit * 0.5, 270 * 16, 180 * 16);
        // painter.drawPath(path);
        /*
         * TODO : add eyes
         */
    };
    enum HeadOrientation { Straight,
                           RightTurn,
                           LeftTurn };
    [[maybe_unused]] auto drawHead = [&](HeadOrientation orientation) {
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        if (orientation == Straight) {
            painter.drawRect(0, unit * 0.25, unit * 0.25 + m_headProperty, unit * 0.25);
            painter.translate(unit * 0.25 + m_headProperty, 0);
            drawHeadWithoutNeck();
            painter.translate(-unit * 0.25 - m_headProperty, 0);
        } else if (orientation == RightTurn) {
            painter.rotate(90);
            painter.translate(0, -unit * 0.5 + m_headProperty);
            drawHeadWithoutNeck();
            painter.translate(0, unit * 0.5 - m_headProperty);
            painter.rotate(-90);
            QPainterPath path;
            path.moveTo(0, unit * 0.75);
            path.quadTo(unit * 0.25, unit * 0.75, unit * 0.25, unit * 0.5 + (m_headProperty <= unit * 0.5 ? m_headProperty : unit * 0.5));
            path.lineTo(unit * 0.75, unit * 0.5 + (m_headProperty <= unit * 0.5 ? m_headProperty : unit * 0.5));
            path.quadTo(unit * 0.75, unit * 0.25, unit, unit * 0.25);
            path.closeSubpath();
            painter.drawPath(path);
            if (m_headProperty >= unit * 0.5) {
                painter.fillRect(unit * 0.25, unit, unit * 0.5, m_headProperty - unit * 0.5, darkBlue);
            }
        } else {
            // orientation == RightTurn
            painter.rotate(270);
            painter.translate(-unit * 0.5 + m_headProperty, 0);
            drawHeadWithoutNeck();
            painter.translate(unit * 0.5 - m_headProperty, 0);
            painter.rotate(-270);
            QPainterPath path;
            path.moveTo(0, unit * 0.25);
            path.quadTo(unit * 0.25, unit * 0.25, unit * 0.25, unit * 0.5 - (m_headProperty <= unit * 0.5 ? m_headProperty : unit * 0.5));
            path.lineTo(unit * 0.75, unit * 0.5 - (m_headProperty <= unit * 0.5 ? m_headProperty : unit * 0.5));
            path.quadTo(unit * 0.75, unit * 0.75, 0, unit * 0.75);
            path.closeSubpath();
            painter.drawPath(path);
            if (m_headProperty >= unit * 0.5) {
                painter.fillRect(unit * 0.25, unit * 0.5 - m_headProperty, unit * 0.5, m_headProperty - unit * 0.5, darkBlue);
            }
        }

        // // first part
        // painter.fillRect(0, margin, m_headProperty + unit / 2, unit - 2 * margin, darkBlue);  // NOLINT(cppcoreguidelines-narrowing-conversions)
        //
        // // round part
        // int startAngle = 270 * 16;
        // int spanAngle = 180 * 16;
        // painter.setPen(darkBlue);
        // painter.setBrush(darkBlue);
        // painter.drawChord(m_headProperty, 2 * margin, unit - 2 * margin, unit - 4 * margin, 270 * 16, 180 * 16);
        //
        // // drawing eyes
        // painter.setBrush(Qt::white);
        // painter.setPen(Qt::white);
        // painter.drawEllipse(m_headProperty + 0.15 * unit, 0.05 * unit, 0.5 * unit, 0.4 * unit);
        // painter.drawEllipse(m_headProperty + 0.15 * unit, 0.55 * unit, 0.5 * unit, 0.4 * unit);
        //
        // painter.setBrush(Qt::black);
        // painter.setPen(Qt::black);
        // painter.drawEllipse(m_headProperty + 0.35 * unit, 0.15 * unit, 0.2 * unit, 0.2 * unit);
        // painter.drawEllipse(m_headProperty + 0.35 * unit, 0.65 * unit, 0.2 * unit, 0.2 * unit);
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

    // draw head
    // draw tail and its neighbor
    // draw the rest of the body
    for (unsigned int i(1); i < snake.size() - 2; ++i) {
    }
    for (unsigned i(0); i < snake.size(); i++) {
        painter.translate(snake[i].first * unit + x0, snake[i].second * unit + y0);
        if (i == 0) {  // drawing the head
            HeadOrientation orientation;
            Coord head(m_game.snake().head()), preHead(m_game.snake().head(1));

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