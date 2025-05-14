#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <exception>
using namespace std;


template<typename T>
class Logger {
public:
    void logToFile(const T& message) {
        ofstream file("game_log.txt", ios::app);
        if (file.is_open()) {
            file << message << endl;
            file.close();
        }
    }
};

class Character {
private:
    string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;

public:
    Character(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0) {}

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            if (enemy.health < 0) enemy.health = 0;
            cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << endl;
        } else {
            cout << name << " attacks " << enemy.name << ", but it has no effect!" << endl;
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
        cout << name << " heals for " << amount << " HP!" << endl;
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            cout << name << " leveled up to level " << level << "!" << endl;
        }
    }

    void displayInfo() const {
        cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << endl;
    }

    const string& getName() const { return name; }
    int getHealth() const { return health; }
    void setHealth(int h) { health = h; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
};

class Monster {
protected:
    string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    virtual void attackCharacter(Character& character) {
        int damage = attack - character.getDefense();
        if (damage > 0) {
            int newHealth = character.getHealth() - damage;
            character.setHealth(newHealth > 0 ? newHealth : 0);
            cout << name << " attacks " << character.getName() << " for " << damage << " damage!" << endl;
        } else {
            cout << name << " attacks " << character.getName() << ", but it has no effect!" << endl;
        }
    }

    virtual void displayInfo() const {
        cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << endl;
    }

    bool isDead() const { return health <= 0; }
    void takeDamage(int dmg) { health = max(0, health - dmg); }
    const string& getName() const { return name; }
};

class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 50, 15, 5) {}
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 200, 30, 20) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 70, 20, 10) {}
};

class Inventory {
private:
    vector<string> items;

public:
    void addItem(const string& item) {
        items.push_back(item);
    }

    void removeItem(const string& item) {
        items.erase(remove(items.begin(), items.end(), item), items.end());
    }

    void displayInventory() const {
        cout << "Inventory: ";
        for (const auto& item : items) {
            cout << item << " ";
        }
        cout << endl;
    }
};

class Game {
private:
    Character player;
    Inventory inventory;
    Logger<string> logger;

public:
    Game(const string& playerName) : player(playerName, 100, 20, 10) {}

    void fight(Monster& monster) {
        while (player.getHealth() > 0 && !monster.isDead()) {
            player.attackEnemy(monster);
            logger.logToFile(player.getName() + " attacked " + monster.getName());
            if (!monster.isDead()) {
                monster.attackCharacter(player);
                logger.logToFile(monster.getName() + " attacked " + player.getName());
            }
        }

        if (player.getHealth() <= 0) {
            throw runtime_error("Player has died!");
        } else {
            cout << monster.getName() << " defeated!" << endl;
            player.gainExperience(50);
        }
    }

    void saveGame(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << player.getName() << endl;
            file << player.getHealth() << endl;
            file.close();
            cout << "Game saved!" << endl;
        }
    }

    void loadGame(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string name;
            int health;
            getline(file, name);
            file >> health;
            player = Character(name, health, 20, 10);
            file.close();
            cout << "Game loaded!" << endl;
        }
    }

    void showInventory() const {
        inventory.displayInventory();
    }

    void addItemToInventory(const string& item) {
        inventory.addItem(item);
    }
};

int main() {
    try {
        Game game("Hero");
        Goblin goblin;
        game.fight(goblin);
        game.addItemToInventory("Health Potion");
        game.showInventory();
        game.saveGame("save.txt");
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
