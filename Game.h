#pragma once
#include "father_file.h"
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Wall.h"
#include "Image.h"
#include "Background.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    PlayerTank player;
    std::vector<Wall> walls;
    std::vector<EnemyTank> enemies;
    Uint32 lastEnemySpawnTime;

    Image* playerTankImage;
    Image* enemyTankImage;
    Image* bulletImage;
    Image* wallImage;
    Background* background;

public:
    Game();
    ~Game();
   //void setImage(Image* img) { playerTankImage = img; }
    bool loadResources();
    void generateWalls();
    void generateEnemies();
    void spawnEnemy();
    void handleEvents();
    void update();
    void checkCollisions();
    void renderThanhmau();
    void render();
    void run();
};
