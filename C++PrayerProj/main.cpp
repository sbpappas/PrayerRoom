#include <iostream>
#include <string>
#include <vector>
#include <filesystem>


#include "nlohmann/json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

struct Prayer{
    int id;
    std::string user;
    std::string title;
    std::string content;
    std::string created_at; //is there a date type?
    bool answered = false;
    bool intercessory = false;

    Prayer() = default //what does this do?
    Prayer(int i, std::string u, std::string t, std::string c, const std::string &ts)
    : id(i), user(std::move(u)), title(std::move(t)), content(std::move(c)), created_at(ts) {}
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

class PrayerApp{
    std::string userFile = "users.json";
    std:string prayerFile = "prayers.json";
    json users; // what does this do
    std::vector<Prayer> prayers;

};

    
int main() {
    PrayerApp app;
    app.run();
    return 0;
}
