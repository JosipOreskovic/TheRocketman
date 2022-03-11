//
// Created by josip on 17.08.2021..
//
#pragma once

#include "GameObject.h"


class Tile : public GameObject
{
public:
    Tile(float x, float y, float speed, bool destructible, int score, int fuel);

    void update(sf::Time dt, sf::Time totalTime) override;
};

