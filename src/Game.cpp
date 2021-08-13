//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "Game.h"

#include <QSound>
#include <algorithm>
#include <iostream>
#include <random>


Game::Game() {
    List snakeInit;
    Coord init = {1, (gridSize - 1) / 2};
    for (size_t i(0); i < snakeBaseSize; i++) {
        snakeInit.push_back({init.first + i, init.second});
    }
    List ordered = List(snakeInit.rbegin(), snakeInit.rend());
    m_snake = Snake(ordered, Right);
    m_apple = regenerateApple();
}

Coord Game::regenerateApple() {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib(0, gridSize - 1);
    List body = m_snake.body();
    Coord candid({distrib(gen), distrib(gen)});
    while (std::find(body.begin(), body.end(), candid) != body.end()) {
        candid = {distrib(gen), distrib(gen)};
    }
    return candid;
}
Coord Game::nextStep() const {
    switch (m_snake.direction()) {
        case Up:
            return {0, -1};
        case Down:
            return {0, 1};
        case Right:
            return {1, 0};
        case Left:
            return {-1, 0};
    }
}

unsigned Game::score() const {
    return m_score;
}
Coord Game::apple() const {
    return m_apple;
}
Snake& Game::snake() {
    return m_snake;
}
bool Game::isFinished() const {
    return m_isFinished;
}

bool Game::hasBegun() const {
    return m_hasBegun;
}
void Game::begin() {
    m_hasBegun = true;
}

void Game::update() {
    Coord futureSnakeHead = m_snake.head() + nextStep();
    if (futureSnakeHead.first >= gridSize || futureSnakeHead.second >= gridSize) {
        // The snake hit hits head on a wall
        m_isFinished = true;
    } else {
        List body(m_snake.body());
        if (std::find(body.begin(), body.end(), futureSnakeHead) != body.end() && futureSnakeHead != m_snake.tail()) {
            // The snake bit his own body
            m_isFinished = true;
        } else if (futureSnakeHead == m_apple) {
            // The snake bit the apple
            ++m_score;
            m_apple = regenerateApple();
            m_snake.grow(futureSnakeHead);
            QSound::play(":/media/apple.wav");
        } else {
            m_snake.evolve(futureSnakeHead);
        }
    }
}