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
    QPainter painter(this);


    painter.setRenderHint(QPainter::Antialiasing);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
#pragma clang diagnostic pop

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


    auto drawSemiCircle = [&]() {
        QRectF rect(0.5, 0.5, unit - 1, unit - 1);
        int startAngle = 270 * 16;
        int spanAngle = 180 * 16;
        painter.setPen(darkBlue);
        painter.setBrush(darkBlue);
        painter.drawChord(rect, startAngle, spanAngle);

        QRect background(0, 0, unit / 2, unit);  // NOLINT(cppcoreguidelines-narrowing-conversions)
        painter.fillRect(background, darkBlue);
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

    List body = m_game.snake().body();
    Coord prevTail = m_game.snake().tail(1);

    for (unsigned i(0); i < body.size(); i++) {
        painter.translate(body[i].first * unit + x0, body[i].second * unit + y0);
        if (i == 0) {  // drawing the head
            if (m_game.snake().direction() == Down) {  // Down
                painter.rotate(90);
                painter.translate(0, -unit);
            } else if (m_game.snake().direction() == Up) {  // Up
                painter.rotate(270);
                painter.translate(-unit, 0);
            } else if (m_game.snake().direction() == Left) {  // Left
                painter.rotate(180);
                painter.translate(-unit, -unit);
            } else if (m_game.snake().direction() == Right) {  // Right
                // Nothing to do
            }
            drawSemiCircleWithEyes();
        } else if (i == body.size() - 1) {  // drawing the tail
            Coord currentTail(m_game.snake().tail());
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
            if ((body[i - 1] == body[i] + Coord{1, 0} && body[i + 1] == body[i] + Coord{-1, 0}) ||
                (body[i - 1] == body[i] + Coord{-1, 0} && body[i + 1] == body[i] + Coord{1, 0}) ||
                (body[i - 1] == body[i] + Coord{0, 1} && body[i + 1] == body[i] + Coord{0, -1}) ||
                (body[i - 1] == body[i] + Coord{0, -1} && body[i + 1] == body[i] + Coord{0, 1})) {
                // Drawing normal body cell
                painter.fillRect(QRect(0, 0, unit, unit), darkBlue);
            } else {
                // Drawing corner body cell
                if ((body[i - 1] == body[i] + Coord{0, -1} && body[i + 1] == body[i] + Coord{1, 0}) ||
                    (body[i + 1] == body[i] + Coord{0, -1} && body[i - 1] == body[i] + Coord{1, 0})) {
                    // drawing right/up angle
                    painter.rotate(90);
                    painter.translate(0, -unit);
                } else if ((body[i - 1] == body[i] + Coord{1, 0} && body[i + 1] == body[i] + Coord{0, 1}) ||
                           (body[i + 1] == body[i] + Coord{1, 0} && body[i - 1] == body[i] + Coord{0, 1})) {
                    // drawing right/down angle
                    painter.rotate(180);
                    painter.translate(-unit, -unit);
                } else if ((body[i - 1] == body[i] + Coord{-1, 0} && body[i + 1] == body[i] + Coord{0, 1}) ||
                           (body[i + 1] == body[i] + Coord{-1, 0} && body[i - 1] == body[i] + Coord{0, 1})) {
                    // drawing left/down angle
                    painter.rotate(270);
                    painter.translate(-unit, 0);
                } else if ((body[i - 1] == body[i] + Coord{0, -1} && body[i + 1] == body[i] + Coord{-1, 0}) ||
                           (body[i + 1] == body[i] + Coord{0, -1} && body[i - 1] == body[i] + Coord{-1, 0})) {
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

    // QRect background(game->getApple().first*unit+x0, game->getApple().second*unit+y0, unit, unit);
    // painter.fillRect(background, darkRed);
}
#pragma clang diagnostic pop

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (!m_game.hasBegun() && event->modifiers() == Qt::NoModifier && QGuiApplication::keyboardModifiers() == Qt::NoModifier) {
        m_game.begin();
        this->statusBar()->clearMessage();
        this->showLabels();
        m_simulationSpeed = simulationTabSpeed[0];
        this->startTimer();
        QSound::play(":/media/start.wav");
    } else {
        switch (event->key()) {
            case Qt::Key_Up:
                if (m_game.snake().direction() != Down && m_game.snake().head(1) != m_game.snake().head() + Coord{0, -1}) {
                    m_game.snake().setDirection(Up);
                }
                break;
            case Qt::Key_Down:
                if (m_game.snake().direction() != Up && m_game.snake().head(1) != m_game.snake().head() + Coord{0, 1}) {
                    m_game.snake().setDirection(Down);
                }
                break;
            case Qt::Key_Left:
                if (m_game.snake().direction() != Right && m_game.snake().head(1) != m_game.snake().head() + Coord{-1, 0}) {
                    m_game.snake().setDirection(Left);
                }
                break;
            case Qt::Key_Right:
                if (m_game.snake().direction() != Left && m_game.snake().head(1) != m_game.snake().head() + Coord{1, 0}) {
                    m_game.snake().setDirection(Right);
                }
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
    }

    if (m_game.isFinished()) {
        QSound::play(":/media/fail.wav");
        this->QObject::killTimer(m_timerId);
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
        for (int i(1); i <= 6; ++i) {
            if (score == 5 * i) {
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

#pragma clang diagnostic pop