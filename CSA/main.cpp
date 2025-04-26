#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include <cstdlib>
#include <ctime>

//salut

void showEnemyAction(Enemy& enemy) {
    // Randomly determine the enemy's action
    int action = rand() % 3;  // Random number between 0 and 2

    switch (action) {
    case 0:
        std::cout << enemy.getName() << " is about to attack!\n";
        break;
    case 1:
        std::cout << enemy.getName() << " is about to defend!\n";
        break;
    case 2:
        std::cout << enemy.getName() << " is about to heal!\n";
        break;
    }
}

int main() {
    // Seed for random number generation (for enemy actions)
    srand(static_cast<unsigned int>(time(0)));

    // Dynamically allocate a player (using 'new')
    Character* player = new Player("Hero");

    // Game loop
    int level = 1;
    while (player->isAlive()) {
        // Dynamically allocate an enemy with the current level
        Character* enemy = new Enemy("Goblin", level);

        // Print the starting message for the round
        std::cout << "\n====================\n";
        std::cout << "Level " << level << " Battle!\n";
        std::cout << "====================\n";

        // Game loop for the current battle
        while (player->isAlive() && enemy->isAlive()) {
            // Print status of both characters
            std::cout << "\n====================\n";
            std::cout << "Player Status:\n";
            player->printStatus();
            std::cout << "====================\n";
            std::cout << "Enemy Status:\n";
            enemy->printStatus();
            std::cout << "====================\n";

            // Show the enemy's next action
            showEnemyAction(*dynamic_cast<Enemy*>(enemy));  // Cast the base class pointer to Enemy

            // Player's turn: Let the player choose an action
            dynamic_cast<Player*>(player)->takeTurn(*enemy);  // Cast to Player and call takeTurn

            // Check if enemy is still alive
            if (!enemy->isAlive()) {
                std::cout << "\n" << enemy->getName() << " has been defeated! You win this round!\n";
                dynamic_cast<Player*>(player)->levelUp();  // Player levels up after defeating an enemy
                delete enemy;  // Delete the enemy object after the battle
                break;  // Exit the inner loop to start a new enemy encounter
            }

            // Enemy's turn (random action)
            std::cout << "\n" << enemy->getName() << "'s turn:\n";
            dynamic_cast<Enemy*>(enemy)->takeTurn(*player);  // Cast to Enemy and call takeTurn

            // Check if player is still alive
            if (!player->isAlive()) {
                std::cout << "\n" << player->getName() << " has been defeated! You lose...\n";
                delete enemy;  // Delete the enemy object after the battle
                break;  // Exit the loop if the player is defeated
            }
        }

        // If player is still alive, increase the level for the next round
        if (player->isAlive()) {
            level++;
        }
        else {
            delete player;  // Delete the player object if the game ends
            break;  // Exit the game if the player has been defeated
        }
    }

    std::cout << "\nGame Over!\n";

    return 0;
}
