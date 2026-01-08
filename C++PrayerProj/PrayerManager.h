#ifndef PRAYERMANAGER_H
#define PRAYERMANAGER_H

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Prayer {
  int id;
  std::string user;
  std::string title;
  std::string content;
  bool answered = false;
  std::string created_at;
};

inline void to_json(json &j, const Prayer &p) {
  j = json{{"id", p.id}, {"user", p.user}, {"title", p.title}, {"content", p.content},
           {"answered", p.answered}, {"created_at", p.created_at}};
}
inline void from_json(const json &j, Prayer &p) {
  j.at("id").get_to(p.id);
  j.at("user").get_to(p.user);
  j.at("title").get_to(p.title);
  j.at("content").get_to(p.content);
  j.at("answered").get_to(p.answered);
  j.at("created_at").get_to(p.created_at);
}


//header file for prayer management to define user related ops
class PrayerManager {
public:
  PrayerManager();
  bool addPrayer(const std::string &username);
  void listRecent(int limit = 10);
  bool retrievePrayerByID(const std::string &username);
  bool markAsAnswered(const std::string &currentUser);
  bool removePrayer(int id, const std::string &currentUser);
  int nextId() const;

private:
  std::vector<Prayer> prayers;
  std::string prayersFile = "prayers.json";
  void loadPrayers();
  void savePrayers();
};

#endif
