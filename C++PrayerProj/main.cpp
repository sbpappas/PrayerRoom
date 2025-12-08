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
#include "PrayerManager.h" 

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
      //userManager::loadUsers();
      //loadPrayers();
      
    }

    void run() {
      authMenu();
      std::cout << "PrayerApp started. Loaded " << prayers.size() << " prayer(s).\n";
      mainFlow(currentUser);
    }

private:
    UserManager userManager; 
    PrayerManager prayerManager;
    std::string currentUser;

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

    void mainFlow(std::string &currentUser){
      bool running = true;
      while (running) {
        int command;
        std::cout << "\nCommands:\n 1) Add prayer\n 2) List recent\n 3) View by id\n 4) Mark answered\n 5) Delete\n 6) Quit" << std::endl;
        std::cin >> command;
        switch (command) {
          case 1:
            prayerManager.addPrayer(currentUser);
            break;
          case 2:
            prayerManager.listRecent();
            break;
          case 3:
            prayerManager.viewByID();
            break;
          case 4:
            prayerManager.markAsAnswered();
            break;
          case 5:
            prayerManager.removePrayer();
            break;
          case 6:
            running = false;
            std::cout << "Exiting PrayerApp.\n";
            break;
          default:
            std::cout << "Invalid command. Try again.\n";
        }
      }

    }
};

int main() {
    PrayerApp app;
    app.run();
    return 0;
}
