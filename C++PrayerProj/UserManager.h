#ifndef USERMANAGER_H
#define USERMANAGER_H


#pragma once // ?
#include <string>
#include <vector>

//header file for user management to define user related ops
class UserManager {
public:
    UserManager();

    void loadUsers(const std::string& filename);
    void saveUsers(const std::string& filename);
    bool userExists(const std::string& username);
    void addUser(const std::string& username);
    bool verifyUser(const std::string &username, const std::string &password);


private:
    std::vector<std::string> users;
    void load();
    void save();
    std::string hashPassword(const std::string &password);
};

#endif //?
