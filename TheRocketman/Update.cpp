//
// Created by josip on 21.08.2021..
//

#include "Game.h"

void Game::update(sf::Time dt) {

    if (state_ == State::Start)
    {
        messageText_.setString("SPACE RETRO TROUBLE\n\nPress P to play\nPress Q to quit");
        messageText_.setCharacterSize(70);
        messageText_.setFillColor(sf::Color::White);
        messageText_.setPosition(resolution_.x / 2 - 300, resolution_.y / 2 - 200);
    }

    if (state_ == State::Game_Over)
    {
        shutDown();

        messageText_.setString("Game over!\n\nPress P to play\nPress Q to quit");
        messageText_.setCharacterSize(70);
        messageText_.setFillColor(sf::Color::White);
        messageText_.setPosition(resolution_.x / 2  - 300, resolution_.y / 2  - 200);
    }

    if (state_ == State::Paused)
    {
        messageText_.setString("Paused\n\nPress Esc to resume\nPress Q to quit");
        messageText_.setCharacterSize(70);
        messageText_.setFillColor(sf::Color::White);
        messageText_.setPosition(resolution_.x / 2  - 300, resolution_.y / 2  - 200);
    }

    if (state_ == State::Playing)
    {
        // Collision check

        collision();

        // Spawn game objects from files

        spawnGameObjects();

        // Player update

        levelRestart_ = player.update(dt, totalTime_, resolution_);

        // Game objects update

        if (level_ == 4 || !enemyFleet_.empty())
        {
            updateGameObjects(dt, enemyFleet_);
        }

        if (level_ == 3 || !meteors_.empty())
        {
            updateGameObjects(dt, meteors_);
        }

        if (level_ == 1 || !mines_.empty())
        {
            updateGameObjects(dt,mines_);
        }

        updateGameObjects(dt, rockets_, player.getPosition());
        updateGameObjects(dt, tanks_, player.getPosition());
        updateGameObjects(dt, fuelTanks_);
        updateGameObjects(dt, tiles_);

        background_.update(dt, resolution_);

        updateHUD();
        totalDistance_ += GAME_SPEED * dt.asSeconds();

        if (totalDistance_ > LEVEL_DISTANCE * level_)
        {
        	++level_;
            levelChange_ = true;
            if (level_ == 5)
                state_ = State::Game_Over;
        }
    }
}

void Game::updateGameObjects(sf::Time dt, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    std::for_each(gameObjects.begin(), gameObjects.end(), [&](auto& gameObject)
        {
            if (gameObject->getPosition().x < -TILE_DIMENSION * 2 || gameObject->getPosition().y < 0)
            {
                gameObject->stop();
            }
            if (gameObject->isActive())
                gameObject->update(dt, totalTime_);
        }
    );
}

void Game::updateGameObjects(sf::Time dt, std::vector<std::shared_ptr<GameObject>>& gameObjects, sf::Vector2f playerPosition)
{

    for_each(gameObjects.begin(), gameObjects.end(), [&](auto& gameObject)
        {
            if (gameObject->getPosition().x < -TILE_DIMENSION * 2 || gameObject->getPosition().y < 0)
            {
                gameObject->stop();
            }
            if (gameObject->isActive())
                gameObject->update(dt, totalTime_, playerPosition);
        }
    );
}

void Game::updateHUD()
{

    std::stringstream ss;
    ss << "Score: " << score_;
    ss << "         " << "Lives: " << lives_;
    ss << "         " << "Fuel: " << player.getFuelLevel();
    ss << "         " << "Level: " << level_;

    scoreText_.setString(ss.str());
    scoreText_.setCharacterSize(55);
    scoreText_.setFillColor(sf::Color::White);
    scoreText_.setPosition(20, 0);

    ss.str("");
}
