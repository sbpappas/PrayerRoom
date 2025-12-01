#ifndef PRAYERMANAGER_H
#define PRAYERMANAGER_H


#pragma once
#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


//header file for prayer management to define user related ops
class UserManager {
public:
  PrayerManager();
  bool addPrayer(const std::string &username, const std::string &password);
  bool retrievePrayerByID(const std::string &username, const std::string &password);
  bool markAsAnswered(std::string &currentUser);  // returns success, sets currentUser
  bool removePrayer(); //returns success, sets current User?
  std::vector<T> listRecent();

private:
  nlohmann::json prayers_;
  std::string file_ = "prayers.json";
  void load();
  void save();
  std::string hashPassword(const std::string &password);
};

#endif //?
