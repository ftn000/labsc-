#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fstream>

template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        if (entity->getHealth() <= 0) {
            throw std::invalid_argument("Entity has invalid health\n");
        }
        entities.push_back(std::move(entity));
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    const std::vector<T>& getEntities() const {
        return entities;
    }
};

template <typename T>
class Queue {
private:
    std::vector<T> items;

public:
    Queue() {
        std::cout << "Queue created!\n";
    }

    void push(const T& item) {
        items.push_back(item);
    }

    void pop() {
        if (items.empty()) {
            throw std::out_of_range("Queue is empty, can't remove any items\n");
        }
        items.erase(items.begin());
    }

    bool empty() const {
        return items.empty();
    }

    void display() const {
        std::cout << "Queue contents:\n";
        for (const auto& item : items) {
            std::cout << item << std::endl;
        }
    }

    ~Queue() {
        std::cout << "Queue ended!\n";
    }
};

class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }
    virtual int getHealth() const {
        return health;
    }
    virtual void save(std::ofstream& file) const{
        file << " " << name << " " << health << std::endl;
    }
    virtual ~Entity() {}
};

class Player : public Entity {
    int level;
public:
    Player(const std::string& n, int h, int l)
        : Entity(n, h), level(l) {}
    void displayInfo() const override {
        std::cout << "Player: " << name << ", HP: " << health
            << ", Level: " << level << std::endl;
    }
    void save(std::ofstream& file) const {
        file << name << " " << health << " " << level <<std::endl;
    }
};

void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }
    for (const auto& entity : manager.getEntities()){
        entity->save(file);
    }
    std::cout << "Saving completed!\n";
}

void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }
    std::string name;
    int HP, level;

    while (file >> name >> HP >> level) {
        manager.addEntity(new Player(name, HP, level));
    }
    std::cout << "Loading completed!\n";
}

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 0));
    manager.addEntity(new Player("Hero2", 200, 2));
    manager.displayAll();
    saveToFile(manager, "game_save.txt");

    GameManager<Entity*> loadedManager;
    loadFromFile(loadedManager, "game_save.txt");
    loadedManager.displayAll();

    return 0;
}
