
#pragma once

#include <SDL.h>
#include <SDL_image.h>  // Thay vì <SDL2/SDL_image.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int ENEMY_SHOOT_INTERVAL = 2000;
const int ENEMY_SPAWN_INTERVAL = 2000;

extern int speed[3];

class Image;
class Wall;
class Bullet;
class HP;
class PlayerTank;
class EnemyTank;
class Game;
