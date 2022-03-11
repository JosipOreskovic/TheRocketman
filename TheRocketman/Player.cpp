//
// Created by josip on 07.08.2021..
//

#include "Player.h"

Player::Player()
{
    active_ = true;
    fuelLevel_ = 100;
    outOfFuel_ = false;
    speed_ = 200.0f;

    newBomb_ = false;
    newMissile_ = false;

    shipSprite_ = sf::Sprite(AssetManager::GetTexture("Assets/Graphics/PlayerShip.png"));
    shipSprite_.setOrigin(shipSprite_.getLocalBounds().width / 2.0f, shipSprite_.getLocalBounds().height / 2.0f);

    engineAnim_ = new Animation("PlayerEngine", "Assets/Graphics/FlightEngine.png", sf::seconds(1), true,
        sf::Vector2f(position_.x, position_.y), sf::Vector2f(0, 0), sf::Vector2i(64, 64), 4);
    explosionAnim_ = new Animation("PlayerExplosion", "Assets/Graphics/PlayerExplosion.png", sf::seconds(2), false,
        sf::Vector2f(position_.x, position_.y), sf::Vector2f(0, 0), sf::Vector2i(256, 256), 11);

}

sf::Sprite& Player::getShipSprite()
{
    return shipSprite_;
}

sf::Vector2f Player::getPosition() const
{
    return position_;
}

bool Player::update(sf::Time dt, sf::Time totalTime, sf::Vector2f resolution)
{
    bool status = false; // flag for explosion animation is finished

    if (active_)
    {
        updateShip(dt, resolution);
        engineAnim_->update(dt, sf::Vector2f(position_.x - TILE_DIMENSION, position_.y));
        updateFuel(totalTime);
        
    }
    else
    {
        playerHitPosition_.x -= speed_ * dt.asSeconds();
        status =  explosionAnim_->update(dt, sf::Vector2f(playerHitPosition_.x, playerHitPosition_.y));
    }

    updateBombs(dt, totalTime, resolution);
    updateMissiles(dt, totalTime, resolution);

    return status;
}

void Player::setPosition(float x, float y)
{
    position_.x = x;
    position_.y = y;
}

void Player::updateShip(sf::Time dt, sf::Vector2f resolution)
{
    if (!outOfFuel_)
    {
        if (upPressed_)
        {
            position_.y -= speed_ * dt.asSeconds();
            if (position_.y < TILE_DIMENSION * 2.0f)
                position_.y = TILE_DIMENSION * 2.0f;
        }

        if (downPressed_)
        {
            position_.y += speed_ * dt.asSeconds();
            if (position_.y > resolution.y - TILE_DIMENSION / 2.0f)
                position_.y = resolution.y - TILE_DIMENSION / 2.0f;
        }

        if (leftPressed_)
        {
            position_.x -= speed_ * dt.asSeconds();
            if (position_.x < TILE_DIMENSION * 2.0f)
                position_.x = TILE_DIMENSION * 2.0f;
        }
        if (rightPressed_)
        {
            position_.x += speed_ * dt.asSeconds();
            if (position_.x > resolution.x - TILE_DIMENSION * 2.0f)
                position_.x = resolution.x - TILE_DIMENSION * 2.0f;
        }
    }
    else
    {
        position_.y += speed_ * dt.asSeconds();
    }

    shipSprite_.setPosition(position_.x, position_.y);
}

void Player::updateFuel(sf::Time totalTime)
{
        if (totalTime.asSeconds() - fuelUseTime_.asSeconds() > 1)
        {
            fuelLevel_--;
            fuelUseTime_ = totalTime;
            if (fuelLevel_ <= 0)
            {
                outOfFuel_ = true;
                fuelLevel_ = 0;
            }
        }
    }


void Player::moveLeft()
{
    leftPressed_ = true;
}

void Player::moveRight()
{
    rightPressed_ = true;
}

void Player::moveUp()
{
    upPressed_ = true;
}

void Player::moveDown()
{
    downPressed_ = true;
}

void Player::stopLeft()
{
    leftPressed_ = false;
}

void Player::stopRight()
{
    rightPressed_ = false;
}

void Player::stopUp()
{
    upPressed_ = false;
}

void Player::stopDown()
{
    downPressed_ = false;
}

void Player::hit(sf::Vector2f hitPosition, sf::Time totalTime)
{
    active_ = false;
    playerHitPosition_ = hitPosition;
    explosionAnim_->resetElapsedTime();
}

bool Player::isActive() const
{
    return active_;
}

void Player::setActive() {
    active_ = true;
}

void Player::reset()
{
    active_ = true;

    fuelLevel_ = 100;
    outOfFuel_ = false;

    bombs_.clear();
    missiles_.clear();
 
    newBomb_ = false;
    newMissile_ = false;
}
int Player::getFuelLevel() const
{
    return fuelLevel_;
}

void Player::updateBombs(sf::Time dt, sf::Time totalTime, sf::Vector2f resolution)
{

    for (auto &bomb : bombs_)
    {
        if (bomb->getPosition().y > resolution.y)
        {
            bomb->stop();
        }
        if (bomb->isActive())
            bomb->update(dt);
    }

    newBombTime_ = totalTime;

    if ((newBombTime_.asMilliseconds() - lastBombTime_.asMilliseconds() > 500) && active_)
    {
        newBomb_ = false;
    }
}

void Player::updateMissiles(sf::Time dt, sf::Time totalTime, sf::Vector2f resolution)
{

    for (auto &missile : missiles_)
    {
        if (missile->getPosition().x > resolution.x)
        {
            missile->stop();
        }
        if (missile->isActive())
            missile->update(dt);
   
    }

    newMissileTime_ = totalTime;
    if ((newMissileTime_.asMilliseconds() - lastMissileTime_.asMilliseconds() > 100) && active_)
    {
        newMissile_ = false;
    }
}

void Player::draw(sf::RenderWindow &window)
{
    if (active_)
    {
        window.draw(shipSprite_);
        window.draw(engineAnim_->getSprite());
    }
    else
    {
        window.draw(explosionAnim_->getSprite());
    }

    for (auto &bomb : bombs_)
    {
       
       if (bomb->isActive())
           bomb->draw(window);
    }
    for (auto &missile : missiles_)
    {
        if (missile->isActive())
            window.draw(missile->getSprite());
    }
}

std::vector<std::shared_ptr<GameObject>>& Player::getMissiles()
{
    return missiles_;
}
std::vector<std::shared_ptr<GameObject>>& Player::getBombs()
{
    return bombs_;
}

void Player::input(sf::Event& event, sf::Time totalTime)
{
    if (event.key.code == sf::Keyboard::Enter)
    {
        if (!newMissile_)
        {
            newMissile_ = true;
            missiles_.push_back(std::make_shared<Missile>(position_.x + TILE_DIMENSION, position_.y + TILE_DIMENSION / 4., 600, false, 0, 0));
            lastMissileTime_ = totalTime;
        }
    }

    // Handle Space

    if (event.key.code == sf::Keyboard::Space)
    {
        if (!newBomb_)
        {
            newBomb_ = true;
            bombs_.push_back(std::make_shared<Bomb>(position_.x, position_.y + TILE_DIMENSION / 4.0f, 300, true, 0, 0));
            lastBombTime_ = totalTime;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        moveUp();
    }
    else
    {
        stopUp();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        moveDown();
    }
    else
    {
        stopDown();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        moveLeft();
    }
    else
    {
        stopLeft();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        moveRight();
    }
    else
    {
        stopRight();
    }
}

void Player::setFuelLevel(int fuel)
{
    fuelLevel_ += fuel;
    if (fuelLevel_ > 100)
        fuelLevel_ = 100;
}

