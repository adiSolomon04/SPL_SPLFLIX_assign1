#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();//destructor
    Session(const Session &other);//copy Constructor
    Session(Session &&other);//move Constructor
    Session& operator=(const Session &other);//copy assignment
    Session& operator=(Session &&other);//move assignment
    void clear();//for copy assign & des
    void copy(const Session &other);//for copy cons &copy assign
    void steal(Session &other);
    void start();

    std::string getActionText();
    bool userNameExsist(std::string& user_name);
    void pushUser(User* user);
    bool setActiveUser(std::string& activeUser);
    bool deleteUser(std::string& userName); // retrun true if sucsses
    User* getUser(std::string& name);
    void printContent();
    User* getActiveUser();
    Watchable* lengthRecommend(User* user, double contentAvg);
    Watchable* genreRecommend(User* user, std::vector< std::pair<std::string, int> > userTags);

    void watchContentId(long contentId);

    void printActionsLog();
    void exit();
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

    std::string actionText; // need to delete!
    bool active;
};
#endif
