#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual User* clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent) = 0;

    virtual User* duplicateUser(std::string& newUser) = 0;
    bool isInHistory(Watchable* content);
    virtual void addToHistory(Watchable* content) = 0;// for watch and add the support of each user type


protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* duplicateUser(std::string& newUser);
    virtual void addToHistory(Watchable* content);
    virtual User* clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent);
private:
    int contentSumLength; //for calc avg
    int contentNum;

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* duplicateUser(std::string& newUser);
    virtual void addToHistory(Watchable* content);
    virtual User* clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent);
private:
    int indexOfRec;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* duplicateUser(std::string& newUser);
    //bool compare(const std::pair<std::string, int>&i, const std::pair<std::string, int>&j);
    virtual void addToHistory(Watchable* content);
    virtual User* clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent);
private:
    std::vector< std::pair<std::string, int> > userTags;

};

#endif
