#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <vector>

//header file for user management to define user related ops
class UserManager {
public:
    UserManager();

    void loadUsers(const std::string& filename);
    void saveUsers(const std::string& filename);
    bool userExists(const std::string& username);
    void addUser(const std::string& username);

private:
    std::vector<std::string> users;
};

#endif
