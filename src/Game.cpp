//
// Created by berth on 12/08/2021.
// Copyright (c) 2021 Tudor Oancea & Mattéo Berthet. All rights reserved.
// Licensed under the MIT license (see https://github.com/tudoroancea/quetzalcoatl/blob/develop/LICENSE)
//

#include "Game.h"

#include <random>

Game::Game() {
    List snakeInit;
    Coord init = {(gridSize - 1) / 2 - snakeBaseSize / 2, (gridSize - 1) / 2};
    for (size_t i(0); i < snakeBaseSize; i++) {
        snakeInit.push_back({init.first+i,init.second});
    }
    snake = Snake(snakeInit);
    apple = regenApple();
}

Coord Game::regenApple() {
    bool success(false);
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib(0, gridSize-1);
    List body = snake.getBody();
    while (!success) {
        Coord candidat = {distrib(gen), distrib(gen)};
        for(auto const& coord : body) {
            if (coord==candidat) {
                continue;
            }
        }
        success = true;
        return candidat;
    }

}

void Game::update() {

    Coord actual = {snake.head().first+avancer().first, snake.head().second+avancer().second};
    if (actual.first > 14 || actual.second > 14) {
        error();
    }
    List body = snake.getBody();
    auto it(std::find(body.begin(), body.end(), actual));
    if (it != body.end() && it != std::prev(body.end()) /*actual != body.back()*/) {
        error();
    }
    if (actual == apple) {
        ++score;
        apple = regenApple();
        snake.grow();
    }else {
        snake.evolve();
    }
}
unsigned Game::getScore(){
    return 0;
}
Coord Game::getApple(){
return Coord();
}
void Game::setScore(){

}
void Game::setApple(){

}
Snake& Game::getSnake(){
return snake;
}
void Game::error(){

}
Coord Game::avancer(){
    Coord next;
    switch (snake.getDirection()) {
        case Up :
            next = {0,1};
            break;
        case Down :
            next = {0,-1};
            break;
        case Right :
            next = {-1,0};
            break;
        case Left :
            next = {0,1};
            break;
    }
    return next;
}
