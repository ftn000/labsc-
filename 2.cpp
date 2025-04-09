#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // �����������
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // ����������
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // �����������
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // ����������
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Weapon {
    std::string name;
    int damage;
    int weight;

public:
    Weapon(const std::string name, int damage, int weight)
        : name(name), damage(damage), weight(weight) {
    }

    void displayInfo() {
        std::cout << "Name: " << name << "\nDamage: " << damage << "\nWeight: " << weight << std::endl;
    }

    ~Weapon() {
        std::cout << name << "is deleted" << std::endl;
    }
};

int main() {
    Weapon ak("Ak-47", 100, 50);

    ak.displayInfo();
}