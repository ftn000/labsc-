#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>

class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel) {
        if (name.empty()) throw std::invalid_argument("Имя пользователя не может быть пустым.");
        if (accessLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным.");
        this->name = name;
        this->id = id;
        this->accessLevel = accessLevel;
    }
    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    virtual void displayInfo() const {
        std::cout << "Имя: " << name << ", ID: " << id << ", Доступ: " << accessLevel << std::endl;
    }
};

class Student : public User {
    std::string group;
public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        std::cout << "[Студент] ";
        User::displayInfo();
        std::cout << "Группа: " << group << std::endl;
    }
};

class Teacher : public User {
    std::string department;
public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    void displayInfo() const override {
        std::cout << "[Преподаватель] ";
        User::displayInfo();
        std::cout << "Кафедра: " << department << std::endl;
    }
};

class Administrator : public User {
    std::string role;
public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        std::cout << "[Администратор] ";
        User::displayInfo();
        std::cout << "Роль: " << role << std::endl;
    }
};

class Resource {
    std::string resourceName;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int level)
        : resourceName(name), requiredAccessLevel(level) {}

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void display() const {
        std::cout << "Ресурс: " << resourceName << ", Требуемый доступ: " << requiredAccessLevel << std::endl;
    }
};

template<typename T>
class AccessControlSystem {
    std::vector<std::shared_ptr<T>> users;
    std::vector<Resource> resources;

public:
    void addUser(std::shared_ptr<T> user) {
        users.push_back(user);
    }

    void addResource(const Resource& resource) {
        resources.push_back(resource);
    }

    void checkAccess(int userId, const std::string& resourceName) {
        auto userIt = std::find_if(users.begin(), users.end(), [userId](const std::shared_ptr<T>& u) {
            return u->getId() == userId;
        });

        if (userIt == users.end()) {
            std::cout << "Пользователь не найден." << std::endl;
            return;
        }

        auto resIt = std::find_if(resources.begin(), resources.end(), [&resourceName](const Resource& r) {
            return r.getName() == resourceName;
        });

        if (resIt == resources.end()) {
            std::cout << "Ресурс не найден." << std::endl;
            return;
        }

        bool access = resIt->checkAccess(**userIt);
        std::cout << (access ? "Доступ разрешён" : "Доступ запрещён") << std::endl;
    }

    void saveToFile(const std::string& filename) {
        std::ofstream out(filename);
        for (const auto& user : users) {
            out << user->getId() << "," << user->getName() << "," << user->getAccessLevel() << "\n";
        }
        out.close();
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
            std::cout << std::endl;
        }
    }

    void sortUsersByAccess() {
        std::sort(users.begin(), users.end(), [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return a->getAccessLevel() > b->getAccessLevel();
        });
    }
};

int main() {
    try {
        AccessControlSystem<User> system;
        system.addUser(std::make_shared<Student>("Иван", 1, 1, "Группа 101"));
        system.addUser(std::make_shared<Teacher>("Петров", 2, 3, "Физика"));
        system.addUser(std::make_shared<Administrator>("Сидоров", 3, 5, "Системный администратор"));

        system.addResource(Resource("Библиотека", 1));
        system.addResource(Resource("Лаборатория", 3));

        std::cout << "Пользователи:\n";
        system.displayAllUsers();

        std::cout << "\nПроверка доступа:\n";
        system.checkAccess(1, "Лаборатория"); // Студент
        system.checkAccess(2, "Лаборатория"); // Преподаватель

        std::cout << "\nПользователи после сортировки:\n";
        system.sortUsersByAccess();
        system.displayAllUsers();

        system.saveToFile("users.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}
