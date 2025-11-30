#ifndef USERMANAGER_H
#define USERMANAGER_H


#pragma once
#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


//header file for user management to define user related ops
class UserManager {
public:
  UserManager();
  bool addUser(const std::string &username, const std::string &password);
  bool verifyUser(const std::string &username, const std::string &password);
  bool login(std::string &currentUser);  // returns success, sets currentUser
  bool signup(); //returns success, sets current User?

private:
  nlohmann::json users_;
  std::string file_ = "users.json";
  void load();
  void save();
  std::string hashPassword(const std::string &password);
};

#endif //?
