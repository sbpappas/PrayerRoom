#include "UserManager.h"
#include <fstream>

UserManager::UserManager() {}

void UserManager::loadUsers(const std::string& filename) {
    fs::path p(usersFile);
    if (!fs::exists(p)) { users = json::object(); return; }
    std::ifstream in(p);
    if (in) in >> users;
}

void UserManager::saveUsers(const std::string& filename) {
    // implementation...
}

bool UserManager::userExists(const std::string& username) {
    // implementation...
}

void UserManager::addUser(const std::string& username) {
    users.push_back(username);
}
