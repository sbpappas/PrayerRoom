#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <sstream>
#include "UserManager.h" //for the other methods

//compile with clang++ -std=c++17 main.cpp -o app

#include "nlohmann/json.hpp" // for json files
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
    std::vector<Prayer> prayers; // in memory prayers list

public:
    PrayerApp() {
      loadUsers();
      loadPrayers();
    }

    void run() {
      authMenu();
      std::cout << "PrayerApp started. Loaded " << prayers.size() << " prayer(s).\n";
      // minimal loop or expand as needed
    }

private:
    UserManager userManager; 
    std::string currentUser;

    void loadUsers() {
      fs::path p(usersFile);
      if (!fs::exists(p)) { users = json::object(); return; }
      std::ifstream in(p);
      if (in) in >> users;
    }

    void loadPrayers() {
      fs::path p(prayersFile);
      if (!fs::exists(p)) { prayers.clear(); return; }
      std::ifstream in(p);
      json j;
      if (in) {
        in >> j;
        prayers.clear();
        for (auto &el : j) prayers.push_back(el.get<Prayer>());
      }
    }
    
    void savePrayers() {
      fs::path p(prayersFile);
      json j = json::array();
      for (auto &pwr : prayers) j.push_back(pwr);
      std::ofstream out(p);
      out << j.dump(2);
    }

    void authMenu() {
      bool AppRunning = true;
      while (AppRunning) {
        int choice;
        std::cout << "\n1) Login\n2) Sign up\n3) Quit" << std::endl;
        std::cin >> choice;
        //have check for valid int input here?
        if (choice == 1) {
          if(userManager.login(currentUser)) { //returns true on success
            std::cout << "Welcome, " << currentUser << "!\n";
            AppRunning = false; //exit auth menu
          }
          else{
            std::cout << "Login failed. Please try again.\n";
          }
        } else if (choice == 2) {
          userManager.signup();
        } else if (choice == 3) {
          AppRunning = false;
          std::cout << "Exiting PrayerApp.\n";
          exit(0); //exit
        } else {
          std::cout << "Invalid choice. Try again.\n";
        }
      }
    }
};

int main() {
    //userManager.addUser("samuel");
    PrayerApp app;
    app.run();
    return 0;
}
