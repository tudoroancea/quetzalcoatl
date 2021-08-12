//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "Game.h"

#include <algorithm>
#include <iostream>
#include <random>


Game::Game() {
    List snakeInit;
    Coord init = {1, (gridSize - 1) / 2};
    for (size_t i(0); i < snakeBaseSize; i++) {
        snakeInit.push_back({init.first + i, init.second});
    }
    List ordered = List(snakeInit.rbegin(),snakeInit.rend());
    snake = Snake(ordered,Right);
    apple = regenApple();
}

Coord Game::regenApple() {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib(0, gridSize - 1);
    List body = snake.getBody();
    Coord candid({distrib(gen), distrib(gen)});
    while (std::find(body.begin(), body.end(), candid) != body.end()) {
        candid = {distrib(gen), distrib(gen)};
    }
    return candid;
}
Coord Game::avancer() const {
    switch (snake.getDirection()) {
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

void Game::update() {
    std::cerr << "Game::update()" << std::endl;
    Coord futureSnakeHead = {snake.head().first + avancer().first, snake.head().second + avancer().second};
    if (futureSnakeHead.first > 14 || futureSnakeHead.second > 14) {
        std::cout << "error"<<std::endl;
        isFinished = true;
    }

    List body = snake.getBody();
    auto it(std::find(body.begin(), body.end(), futureSnakeHead));  // test if the future snake head will intercept its body
    if (it != body.end() && it != std::prev(body.end())) {
        std::cout << "error"<<std::endl;
        isFinished = true;
    }
    if (!isFinished) {
        if (futureSnakeHead == apple) {
            ++score;
            apple = regenApple();
            snake.grow(futureSnakeHead);
        } else {
            snake.evolve(futureSnakeHead);
        }
        std::cout << "Snake : ";
        for (const auto& item : body) {
            std::cout << "{" << item.first << "," << item.second << "} ; ";
        }
        std::cout << std::endl;
    }
}
unsigned Game::getScore() const {
    return score;
}
Coord Game::getApple() const {
    return apple;
}
void Game::setScore(unsigned int const& newScore) {
    score = newScore;
}
void Game::setApple(Coord const& newApple) {
    apple = newApple;
}
Snake& Game::getSnake() {
    return snake;
}
bool Game::getIsFinished(){
return isFinished;
}
// void Game::error() {
//     /* TODO : to implement */
// }
