#include "Game.h"

Game::Game() : player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - TILE_SIZE),
              running(true), lastEnemySpawnTime(0),
              playerTankImage(nullptr), enemyTankImage(nullptr),
              bulletImage(nullptr), wallImage(nullptr),
              background(nullptr) {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Battle City Galaxy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (loadResources()) {
        player.setImage(playerTankImage);
        for (auto& wall : walls) {
            wall.setImage(wallImage);
        }
    }

    background = new Background(renderer, "assets/galaxy_bg_vertical.png");
    background->setScrollSpeed(2);

    generateWalls();
    generateEnemies();
}

Game::~Game() {
    delete playerTankImage;
    delete enemyTankImage;
    delete bulletImage;
    delete wallImage;
    delete background;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::loadResources() {
    playerTankImage = new Image(renderer, "assets/player_tank.png");
    enemyTankImage = new Image(renderer, "assets/enemy_tank.png");
    bulletImage = new Image(renderer, "assets/bullet.png");
    wallImage = new Image(renderer, "assets/wall.png");

    if (!playerTankImage->getTexture() || !enemyTankImage->getTexture() ||
        !bulletImage->getTexture() || !wallImage->getTexture()) {
        std::cerr << "Lỗi tải một hoặc nhiều hình ảnh!" << std::endl;
        return false;
    }

    return true;
}

void Game::generateWalls() {
    for (int i = 0; i < SCREEN_WIDTH; i += TILE_SIZE) {
        walls.push_back(Wall(i, SCREEN_HEIGHT - TILE_SIZE));
        if (wallImage) {
            walls.back().setImage(wallImage);
        }
    }
}

void Game::generateEnemies() {
    enemies.push_back(EnemyTank(100, 100));
    enemies.push_back(EnemyTank(300, 100));
    if (enemyTankImage) {
        for (auto& enemy : enemies) {
            enemy.setImage(enemyTankImage);
        }
    }
}

void Game::spawnEnemy() {
    int x = rand() % (SCREEN_WIDTH - TILE_SIZE);
    int y = rand() % (SCREEN_HEIGHT / 2);
    enemies.push_back(EnemyTank(x, y));
    if (enemyTankImage) {
        enemies.back().setImage(enemyTankImage);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_MOUSEMOTION) {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;
            player.move(mouseX, mouseY);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            player.shoot();
            if (bulletImage) {
                player.bullets.back().setImage(bulletImage);
            }
        }
    }
}

void Game::update() {
    background->update();

    player.updateBullets();
    for (auto& enemy : enemies) {
        enemy.update();
        if (bulletImage) {
            for (auto& bullet : enemy.bullets) {
                bullet.setImage(bulletImage);
            }
        }
    }
    checkCollisions();

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastEnemySpawnTime > ENEMY_SPAWN_INTERVAL) {
        int soluong = std::max(1, rand() % 4);
        for (int i = 0; i <= soluong; i++) {
            spawnEnemy();
        }
        lastEnemySpawnTime = currentTime;
    }
}

void Game::checkCollisions() {
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                bullet.active = false;
                enemy.active = false;
                player.score += 10;
                std::cout << "Score: " << player.score << std::endl;
            }
        }
    }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyTank& e) {
        return !e.active;
    }), enemies.end());

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                bullet.active = false;
                player.hp.dinhchuong(1);
                if (player.hp.die()) {
                    running = false;
                    std::cout << "Game Over! Final Score: " << player.score << std::endl;
                }
            }
        }
    }
}

void Game::renderThanhmau() {
    int barWidth = 200;
    int barHeight = 20;
    int barX = (SCREEN_WIDTH - barWidth) / 2;
    int barY = SCREEN_HEIGHT - 30;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect backgroundRect = { barX, barY, barWidth, barHeight };
    SDL_RenderFillRect(renderer, &backgroundRect);

    int currentHealthWidth = (player.hp.getMauHienTai() * barWidth) / player.hp.getMauToiDa();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthRect = { barX, barY, currentHealthWidth, barHeight };
    SDL_RenderFillRect(renderer, &healthRect);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    background->render(renderer);

    for (auto& wall : walls) {
        wall.render(renderer);
    }

    player.render(renderer);

    for (auto& enemy : enemies) {
        enemy.render(renderer);
    }

    renderThanhmau();

    SDL_RenderPresent(renderer);
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}
