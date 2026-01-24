#include "PrayerManager.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <chrono> //for getting current time
#include <ctime> //for std::gmtime

using json = nlohmann::json;
namespace fs = std::filesystem;

PrayerManager::PrayerManager() {
  loadPrayers();
}

int PrayerManager::nextId() const {
  int maxid = 0;
  for (const auto &p : prayers) maxid = std::max(maxid, p.id);
  return maxid + 1;
}

static std::string nowISO() {
  using namespace std::chrono;
  auto t = system_clock::now();
  std::time_t tt = system_clock::to_time_t(t);
  char buf[64];
  std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&tt));
  return std::string(buf);
}

bool PrayerManager::addPrayer(const std::string &username) {
  // prompt for title/content and add a new prayer for username
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear newline
  std::string title;
  std::cout << "Title (short): ";
  std::getline(std::cin, title);
  if (title.empty()) {
    std::cout << "Title required.\n";
    return false;
  }
  std::string content;
  std::cout << "Prayer content: ";
  std::getline(std::cin, content);

  Prayer p;
  p.id = nextId();
  p.user = username;
  p.title = title;
  p.content = content;
  p.answered = false;
  p.created_at = nowISO();

  prayers.push_back(p);
  savePrayers();
  std::cout << "Prayer added (id=" << p.id << ").\n";
  return true;
}

void PrayerManager::listRecent(int limit) {
  // list 10 most recent prayers
  int count = 0;
  std::cout << "\nRecent Prayers:\n";
  for (auto it = prayers.rbegin(); it != prayers.rend() && count < 10; ++it, ++count) {
    const Prayer &p = *it;
    std::cout << "ID: " << p.id << " | User: " << p.user << " | Title: " << p.title
              << " | Created: " << p.created_at << " | Answered: " << (p.answered ? "true" : "false") << "\n";
  }
}
bool PrayerManager::retrievePrayerByID(const std::string &username) {
  int id;
  std::cout << "Enter prayer ID to retrieve: ";
  std::cin >> id;
  auto it = std::find_if(prayers.begin(), prayers.end(), [&](const Prayer &p){ return p.id == id && p.user == username; });
  if (it == prayers.end()) {
    std::cout << "Prayer id " << id << " not found for user " << username << ".\n";
    return false;
  }
  const Prayer &p = *it;
  std::cout << "\nID: " << p.id << "\nUser: " << p.user << "\nTitle: " << p.title
            << "\nCreated: " << p.created_at << "\nAnswered: " << (p.answered ? "true" : "false")
            << "\n\n" << p.content << "\n";
  return true;
}

bool PrayerManager::markAsAnswered(const std::string &currentUser)
{
  int id;
  std::cout << "Enter prayer ID to mark as answered:: ";
  std::cin >> id;
  if (auto it = std::find_if(prayers.begin(), prayers.end(), [&](const Prayer &p){ return p.id == id && p.user == currentUser; });
      it != prayers.end()) {
    it->answered = true;
    savePrayers();
    std::cout << "Prayer id " << id << " marked as answered.\n";
    return true;
  } else {
    std::cout << "Prayer id " << id << " not found for user " << currentUser << ".\n";
    return false;
  }
}  // returns success, sets currentUser

bool PrayerManager::removePrayer(const std::string &currentUser){
  int id;
  std::cout << "Enter prayer ID to remove: ";
  std::cin >> id;
  if (std::cin.fail()) {
    std::cin.clear();  // Clear the error flag
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the buffer
    std::cout << "Invalid input. Please enter a number.\n";
    return false;
  }
  auto it = std::find_if(prayers.begin(), prayers.end(), [&](const Prayer &p){ return p.id == id && p.user == currentUser; });
  if (it != prayers.end()) {
    prayers.erase(it);
    savePrayers();
    std::cout << "Prayer id " << id << " removed.\n";
    return true;
  } else {
    std::cout << "Prayer id " << id << " not found for user " << currentUser << ".\n";
    return false;
  }
}


void PrayerManager::loadPrayers() {
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
    
void PrayerManager::savePrayers() {
    fs::path p(prayersFile);
    json j = json::array();
    for (auto &pwr : prayers) j.push_back(pwr);
    std::ofstream out(p);
    out << j.dump(2);
}
