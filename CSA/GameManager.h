// GameManager.h
#pragma once
#include "Character.h"
#include "Enemy.h"
#include "Player.h"

class GameManager {
public:
    // Metodă statică pentru resetarea jocului
    static void resetGame(Character* characters[], int& level);
    static void saveToFile(const Character& c, const std::string& filename);
    static void loadFromFile(Character& c, const std::string& filename);

};
