#include "PrayerManager.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>



using json = nlohmann::json;
namespace fs = std::filesystem;

PrayerManager::PrayerManager() {
  loadPrayers();
}
bool PrayerManager::addPrayer(const std::string &username, const std::string &password){
    
}

bool PrayerManager::retrievePrayerByID(const std::string &username, const std::string &password);
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
