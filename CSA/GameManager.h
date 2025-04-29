// GameManager.h
#pragma once
#include "Character.h"
#include "Enemy.h"
#include "Player.h"

class GameManager {
public:
    // Metodă statică pentru resetarea jocului
    static void resetGame(Character* characters[], int& level);
};
