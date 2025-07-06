#include "user.h"
#include <fstream>
#include <filesystem>
#include <iostream>
// Konstruktor z parametrami
User::User(std::string name, std::string password, float money,bool isAdmin)
    : name(name), password(password), money(money), id(0), admin_privileges(isAdmin) {
    }

float User::getMoney() const {
    return money;
}

// Getter dla nazwy użytkownika
std::string User::getName() const {
    return name;
}

// Setter dla nazwy użytkownika
void User::setName(const std::string newName) {
    this->name = newName;
}

void User::setPassword(const std::string password)
{
    this->password = password;
}
void User::setMoney(float newMoney) {
    this->money = newMoney;
}

bool User::saveToFile() const
{
    bool success = false;
    
    std::string dirPath = "./dane/" + name;
    std::filesystem::create_directories(dirPath);
    try{
        std::ofstream file(dirPath + "/player.txt");
        if (file.is_open()) {
            file << *this; // Użycie operatora << do zapisu obiektu User
            file.close();
            success = true;
        }
        if (!file.fail()) {
                success = true;
            }
        success = true;
    }catch (const std::exception& e) {
        std::cerr << "Error saving user to file: " << e.what() << std::endl;
        return false;
    }
    
    return success;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << " " << user.getName() << " " << user.password << " " << user.getMoney() << " " << user.isAdmin();
    return os;
}