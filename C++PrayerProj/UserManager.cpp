#include "UserManager.h"
#include <fstream>
#include "nlohmann/json.hpp" // for json files
using json = nlohmann::json;
namespace fs = std::filesystem;

UserManager::UserManager() {}

void UserManager::loadUsers(const std::string& filename) {
    fs::path p(usersFile);
    if (!fs::exists(p)) { users = json::object(); return; }
    std::ifstream in(p);
    if (in) in >> users;
}

void UserManager::saveUsers(const std::string& filename) {
    
}

bool UserManager::userExists(const std::string& username) {
    // implementation...
}

void UserManager::addUser(const std::string& username) {
    users.push_back(username);
}
