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

bool PrayerManager::addPrayer(const std::string &username, const std::string & /*password*/) {
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

bool PrayerManager::retrievePrayerByID(int id) {
  auto it = std::find_if(prayers.begin(), prayers.end(), [&](const Prayer &p){ return p.id == id; });
  if (it == prayers.end()) {
    std::cout << "Prayer id " << id << " not found.\n";
    return false;
  }
  const Prayer &p = *it;
  std::cout << "\nID: " << p.id << "\nUser: " << p.user << "\nTitle: " << p.title
            << "\nCreated: " << p.created_at << "\nAnswered: " << (p.answered ? "true" : "false")
            << "\n\n" << p.content << "\n";
  return true;
}

bool PrayerManager::markAsAnswered(std::string &currentUser);  // returns success, sets currentUser
bool PrayerManager::removePrayer();


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
