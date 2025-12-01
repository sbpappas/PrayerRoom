#ifndef PRAYERMANAGER_H
#define PRAYERMANAGER_H

#pragma once
#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


//header file for prayer management to define user related ops
class PrayerManager {
public:
  PrayerManager();
  bool addPrayer(const std::string &username, const std::string &password);
  bool retrievePrayerByID(const std::string &username, int id);
  bool markAsAnswered(std::string &currentUser);  // returns success, sets currentUser
  bool removePrayer(); //returns success, sets current User?
  int nextId() const;
  static std::string nowISO();
   // std::vector<> listRecent();

private:
  nlohmann::json prayers_;
  std::string file_ = "prayers.json";
  void loadPrayers();
  void savePrayers();
};

#endif //?
