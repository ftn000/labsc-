#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <random>   
#include <atomic>
std::atomic<bool> gameRunning(true);


class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Entity(const std::string& n, int h, int a, int d) : name(n), health(h), attack(a), defense(d){}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }
    void takeDamage(int damage) { 
        int actualdamage = std::max(damage - defense, 0);
        health -= actualdamage;
    }
    virtual void attacking(Entity& target) = 0;

    virtual int getHealth() const {
        return health;
    }
    virtual int getAttack() const {
        return attack;
    }
    virtual std::string getName() const {
        return name;
    }
    virtual ~Entity() {}
};

class Character : public Entity {
    int level = 0;
    int nextlevelexp = 50;
    int exp = 0;
    int maxhealth;
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d),  maxhealth(h) {}
    void displayInfo() const override {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    void attacking(Entity& enemy) override
    {
        int attackDamage = getAttack();
        if (rand() % 100 < 30)
        {
            attackDamage *= 2;
            std::cout << "Strong hit! " << name << " attacks " << enemy.getName() << " for " << getAttack() * 2 << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << enemy.getName() << " for " << attack << " damage!\n";
        }
        enemy.takeDamage(attackDamage);
    };

    void getexp(int amount) {
        exp += amount;
        if (exp > nextlevelexp) {
            level++;
            health = maxhealth;
            exp -= 100;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distributiondamage(10, 30);
            int damageget = distributiondamage(gen);
            attack += damageget;
            nextlevelexp += 100;

            std::cout << name << " Just got a level " << level << " ,Health regenerated to full and he got +" << damageget << " damage\n";
        }
    }

    void displaylevel() {
        std::cout << name << "is at level: "<< level++ << " , he needs: " << exp << "/" << nextlevelexp << " experience for next level\n\n";
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d){}

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    void attacking(Entity& hero) override
    {
        int attackDamage = getAttack();
        if (rand() % 100 < 10)
        {
            attackDamage *= 3;
            std::cout << "Hit in the back! " << name << " attacks "
                << hero.getName() << " for " << getAttack() * 3 << " damage!\n";;
        }
        else {
            std::cout << name << " attacks " << hero.getName() << " for " << getAttack() << " damage!\n";
        }
        hero.takeDamage(attackDamage);
    }
};

std::vector<Monster> monsters;
std::mutex monstersMutex;
void generateMonsters() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distributionHealth(30, 100);
    std::uniform_int_distribution<> distributionAttack(5, 20);
    std::uniform_int_distribution<> distributionDefense(2, 6);
    const std::string monsterSpecies[] = { "Zombie", "Skeleton", "Goblin", "Demon" };
    std::uniform_int_distribution<> distributionName(0, std::size(monsterSpecies) - 1);
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (!gameRunning) break;
        std::cout << distributionName(gen);
        std::string specie = monsterSpecies[distributionName(gen)];
        int health = distributionHealth(gen);
        int attack = distributionAttack(gen);
        int defense = distributionDefense(gen);

        std::lock_guard<std::mutex> lock(monstersMutex);
        monsters.push_back(Monster(specie, health, attack, defense));
        std::cout << "New monster generated!\n";
    }
}

std::mutex fightMutex;
void fight(Character& hero, Monster& monster) {
    while (hero.getHealth() >= 0 and monster.getHealth() >= 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(fightMutex);

        hero.attacking(monster);
        std::cout << hero.getName() << " hits " << monster.getName() << " for " << hero.getAttack() << " damage.\n";

        monster.attacking(hero);
        std::cout << monster.getName() << " hits " << hero.getName() << " for " << monster.getAttack() << " damage.\n";

        std::cout << hero.getName() << " HP: " << hero.getHealth() << " | "
            << monster.getName() << " HP: " << monster.getHealth() << "\n\n";
    }
    if (hero.getHealth() > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distributionExp(10, 30);
        int xpget = distributionExp(gen);
        hero.getexp(xpget);
        std::cout << "Hero" << hero.getName() << " just defeated: " << monster.getName() << " and got " << xpget << " of xp \n";
        hero.displaylevel();
    }
}

int main() {
    std::thread monsterGenerator(generateMonsters);

    Character hero("Joe", 100, 20, 10);

    while (hero.getHealth() > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(monstersMutex);
        if (!monsters.empty()) {
            Monster currentMonster = monsters[0];
            monsters.erase(monsters.begin());
            std::thread fightThread(fight, std::ref(hero), std::ref(currentMonster));
            fightThread.join();
            std::cout << "\nCurrent status:\n";
            hero.displayInfo();
            std::cout << "Monsters remaining: " << monsters.size() << "\n";
        }
    }

    gameRunning = false;
    monsterGenerator.join();

    std::cout << "Game Over!\n";
    return 0;
}
