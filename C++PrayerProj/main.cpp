#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <sstream>

//compile with clang++ -std=c++17 main.cpp -o app

#include "nlohmann/json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

struct Prayer {
    int id;
    std::string user;
    std::string title;
    std::string content;
    std::string created_at; // keep as ISO string
    bool answered = false;
    bool intercessory = false;

    Prayer() = default; // default constructor
    Prayer(int i, std::string u, std::string t, std::string c, const std::string &ts)
      : id(i), user(std::move(u)), title(std::move(t)), content(std::move(c)), created_at(ts) {}
};

inline void to_json(json &j, const Prayer &p) {
  j = json{
    {"id", p.id},
    {"user", p.user},
    {"title", p.title},
    {"content", p.content},
    {"answered", p.answered},
    {"intercessory", p.intercessory},
    {"created_at", p.created_at}
  };
}
inline void from_json(const json &j, Prayer &p) {
  j.at("id").get_to(p.id);
  j.at("user").get_to(p.user);
  j.at("title").get_to(p.title);
  j.at("content").get_to(p.content);
  j.at("answered").get_to(p.answered);
  if (j.contains("intercessory")) j.at("intercessory").get_to(p.intercessory);
  j.at("created_at").get_to(p.created_at);
}

class PrayerApp {
    std::string usersFile = "users.json";
    std::string prayersFile = "prayers.json";
    json users; // in-memory users object
    std::vector<Prayer> prayers;

public:
    PrayerApp() {
      loadUsers();
      loadPrayers();
    }

    void run() {
      std::cout << "PrayerApp started. Loaded " << prayers.size() << " prayer(s).\n";
      // minimal loop or expand as needed
    }

private:
    void loadUsers() {
      if (!fs::exists(usersFile)) { users = json::object(); return; }
      std::ifstream in(usersFile);
      if (in) in >> users;
    }

    void saveUsers() {
      std::ofstream out(usersFile);
      out << users.dump(2);
    }

    void loadPrayers() {
      if (!fs::exists(prayersFile)) { prayers.clear(); return; }
      std::ifstream in(prayersFile);
      json j;
      if (in) {
        in >> j;
        prayers.clear();
        for (auto &el : j) prayers.push_back(el.get<Prayer>());
      }
    }

    void savePrayers() {
      json j = json::array();
      for (auto &p : prayers) j.push_back(p);
      std::ofstream out(prayersFile);
      out << j.dump(2);
    }
};

int main() {
    PrayerApp app;
    app.run();
    return 0;
}
