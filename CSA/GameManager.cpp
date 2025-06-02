// GameManager.cpp
#include "GameManager.h"
#include "Boss.h"
#include "Enemy.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include "Character.h"
#include <string>

void GameManager::resetGame(Character* characters[], int& level) {
    level = 1;

    characters[0]->setHp(characters[0]->getMaxHp());
    characters[0]->setAttackPower(10); 
    characters[0]->setDefensePower(5);

    delete characters[1];
    characters[1] = new Enemy("Hot", level);

    std::cout << "Jocul a fost resetat! Nivelul este acum " << level << ".\n";
}
void GameManager::saveToFile(const Character& c, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << c.getName() << " " << c.getHp() << " " << c.getMaxHp() << " "
            << c.getAttackPower() << " " << c.getDefensePower();
        file.close();
    }
    else {
        std::cerr << "Error: Could not open file for writing.\n";
    }
}

void GameManager::loadFromFile(Character& c, const std::string& filename) {  
   std::ifstream file(filename);  
   if (file.is_open()) {  
       std::string name;  
       int hp, maxHp, attackPower, defensePower;  

       file >> name >> hp >> maxHp >> attackPower >> defensePower;  

       c.setName(name);  
       c.setHp(hp);  
       c.setMaxHp(maxHp);  
       c.setAttackPower(attackPower);  
       c.setDefensePower(defensePower);  

       file.close();  
   }  
   else {  
       std::cerr << "Error: Could not open file for reading.\n";  
   }  
}