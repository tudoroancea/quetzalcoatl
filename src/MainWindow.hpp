//
// Created by Tudor Oancea on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#pragma once

#ifndef QUETZALCOATL_MAINWINDOW_HPP
    #define QUETZALCOATL_MAINWINDOW_HPP

    #include <QMainWindow>

    #include "global.h"

class Game {
public:
    Game() {}
    int getScore() { return 0; }
};

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
private:
    Game* game;
    QLabel* scoreLabel;
    QLabel* bestScoreLabel;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    explicit MainWindow();
};


#endif  // QUETZALCOATL_MAINWINDOW_HPP
