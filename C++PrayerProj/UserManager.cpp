#include "UserManager.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <algorithm>
#include <iostream>


using json = nlohmann::json;
namespace fs = std::filesystem;

UserManager::UserManager() {
  load();
}

void UserManager::load() {
    std::cout << "loading users from " << std::endl;
    if (fs::exists(file_)) {
    std::ifstream in(file_);
    if (in) in >> users_;
  } else {
    users_ = json::object();
  }
}

void UserManager::save() {
  std::ofstream out(file_);
  out << users_.dump(2);
}

std::string UserManager::hashPassword(const std::string &password) {
  // simple non-cryptographic hash for demo only
  //std::hash<std::string> h;
  //std::ostringstream ss;
  //ss << std::hex << h(password);
  //return ss.str();
  return "placeholder";
}

bool UserManager::addUser(const std::string &username, const std::string &password) {
  std::string key = username;
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  if (users_.contains(key)) return false;
  users_[key] = { {"hash", hashPassword(password)} };
  save();
  return true;
}

bool UserManager::verifyUser(const std::string &username, const std::string &password) {
  std::string key = username;
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  if (!users_.contains(key)) return false;
  return users_[key]["hash"].get<std::string>() == hashPassword(password);
}

bool UserManager::login(std::string &currentUser) {
  std::string username, password;
  std::cout << "Username: ";
  std::cin >> username;
  std::cout << "Password: ";
  std::cin >> password;
  
  if (verifyUser(username, password)) {
    std::cout << "Login successful.\n";
    currentUser = username;
    return true;
  } else {
    std::cout << "Login failed. Invalid username or password.\n";
    return false;
  }
}

