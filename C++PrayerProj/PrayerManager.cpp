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
  load();
}