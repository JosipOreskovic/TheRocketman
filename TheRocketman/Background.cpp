//
// Created by josip on 27.08.2021..
//

#include "Background.h"

Background::Background()
{
    order_ = true;
    position_.x = 0.0;

    textures_.push_back(&AssetManager::GetTexture("Assets\\Graphics\\Background1.png"));
    textures_.push_back(&AssetManager::GetTexture("Assets\\Graphics\\Background2.png"));
}

void Background::update(sf::Time dt, sf::Vector2f resolution)
{
    position_.x -= BACKGROUND_SPEED * dt.asSeconds();
    if (position_.x  < -resolution.x)
    {
        position_.x  = 0.0f;
        order_ = !order_;
    }
}

void Background::draw(sf::RenderWindow &window, sf::Vector2f resolution)
{
    sprite_.setTexture(*textures_[order_]);
    sprite_.setPosition(position_.x,0.0f);
    window.draw(sprite_);
    sprite_.setTexture(*textures_[!order_]);
    sprite_.setPosition(resolution.x + position_.x, 0.0f);
    window.draw(sprite_);
}
