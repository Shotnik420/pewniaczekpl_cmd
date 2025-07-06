#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(std::string name, std::string password, float money = 0.0f, bool isAdmin = false);

    std::string getName() const;
    std::string getPassword() const;

    float getMoney() const;



    void setName(const std::string name);
    void setPassword(const std::string password);
    void setMoney(float money);
    bool isAdmin() const {
        return admin_privileges;
    }
    void setAdmin(bool isAdmin) {
        admin_privileges = isAdmin;
    }
    bool saveToFile() const;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
private:
    int id;
    std::string name;
    std::string password;
    float money;
    bool admin_privileges = false; // Czy ma uprawnienia administratora
};

#endif // USER_H