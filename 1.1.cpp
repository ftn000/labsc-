#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Ïðèâàòíîå ïîëå: èìÿ ïåðñîíàæà
    int health;        // Ïðèâàòíîå ïîëå: óðîâåíü çäîðîâüÿ
    int attack;        // Ïðèâàòíîå ïîëå: óðîâåíü àòàêè
    int defense;       // Ïðèâàòíîå ïîëå: óðîâåíü çàùèòû


public:
    // Êîíñòðóêòîð äëÿ èíèöèàëèçàöèè äàííûõ
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // Ìåòîä äëÿ ïîëó÷åíèÿ óðîâíÿ çäîðîâüÿ
    int getHealth() const {
        return health;
    }

    // Ìåòîä äëÿ âûâîäà èíôîðìàöèè î ïåðñîíàæå
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Ìåòîä äëÿ àòàêè äðóãîãî ïåðñîíàæà
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Ìåòîä äëÿ ëå÷åíèÿ
    void heal(int heal_value) {
        if (heal_value + health > 100) {
            std::cout << "Heal is full" << std::endl;
            health = 100;
        }
        else {
            health += heal_value;
        }
    }

    void takeDamage(int damage) {
        if (health - damage < 0) {
            health = 0;
            std::cout << "You died" << std::endl;
        }
        else {
            health -= damage;
        }
    }
};

int main() {

    // Ñîçäàåì îáúåêòû ïåðñîíàæåé
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Âûâîäèì èíôîðìàöèþ î ïåðñîíàæàõ
    hero.displayInfo();
    monster.displayInfo();

    // Ãåðîé àòàêóåò ìîíñòðà
    hero.attackEnemy(monster);
    monster.displayInfo();

    hero.takeDamage(40);
    std::cout << "Your health " << hero.getHealth() << std::endl;

    hero.heal(50);
    std::cout << "Your health " << hero.getHealth() << std::endl;

    hero.takeDamage(150);
    return 0;
}