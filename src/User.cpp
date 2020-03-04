//
// Created by eran on 11/23/19.
//

#include <iostream>
#include <algorithm>
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"



using namespace std;

User::User(const std::string& name) : history(), name(name){} //to-check: need to initialization the history?
std::string User::getName() const{return name;}//to-check: return by value?
std::vector<Watchable*> User::get_history() const{return history;} //r//to-check: eturn by value?


LengthRecommenderUser::LengthRecommenderUser(const std::string& name):User(name) ,contentSumLength(0), contentNum(0){}

Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    Watchable* lenRec = s.lengthRecommend(this, (double)contentSumLength/contentNum);
    return lenRec;
}

User* LengthRecommenderUser::duplicateUser(std::string& newUser) {
    LengthRecommenderUser* duplicate = new LengthRecommenderUser(newUser);
    duplicate->contentSumLength = LengthRecommenderUser::contentSumLength;
    duplicate->contentNum = LengthRecommenderUser::contentNum;
    duplicate->history = User::history;
    return duplicate;
}

void LengthRecommenderUser::addToHistory(Watchable *content) {
    history.push_back(content);
    contentSumLength += content->getLength();
    contentNum++;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string& name):User(name),indexOfRec(0){}

User* RerunRecommenderUser::duplicateUser(std::string& newUser) {
    RerunRecommenderUser* duplicate = new RerunRecommenderUser(newUser);
    duplicate->indexOfRec = RerunRecommenderUser::indexOfRec;
    (*duplicate).history = (*this).history;
    return duplicate;
}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable* ret = history.at(indexOfRec);
    indexOfRec = (indexOfRec+1)%(history.size());
    return ret;
}

void RerunRecommenderUser::addToHistory(Watchable *content) {
    history.push_back(content);
}

GenreRecommenderUser::GenreRecommenderUser(const std::string& name):User(name),userTags(){}

User* GenreRecommenderUser::duplicateUser(std::string& newUser) {
    GenreRecommenderUser* duplicate = new GenreRecommenderUser(newUser);
    duplicate->userTags = GenreRecommenderUser::userTags; ///Is it a copy of pairs??
    (*duplicate).history = (*this).history;
    return duplicate;
}

bool compare(const pair<std::string, int>&i, const pair<std::string, int>&j)
{
    //sort by nuber of tags and yhen by abc
    if(i.second > j.second)
        return true;
    else if(i.second < j.second)
        return false;
    else
        return i.first.compare(j.first)<0;
}

Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    std::sort(userTags.begin(), userTags.end(), compare);
    Watchable* lenRec = s.genreRecommend(this, userTags);
    return lenRec;
}

void GenreRecommenderUser::addToHistory(Watchable *content) {
    history.push_back(content);
    vector<std::string> tag =content->getTags();
    for (unsigned i = 0; i < tag.size(); ++i) {
        bool found = false;
        for (unsigned j = 0; j <userTags.size() &&!found; ++j) {
            if(userTags.at(j).first==tag.at(i)){
                found = true;
                userTags.at(j).second++;
            }
        }
        if(!found){
            userTags.push_back(std::make_pair(tag.at(i),1));
        }
    }
}


bool User::isInHistory(Watchable* content){
    for(unsigned i=0; i<history.size(); i++){
        if(history[i]==content)
            return true;
    }
    return false;
}

User::~User() {}

User* LengthRecommenderUser::clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent){
    LengthRecommenderUser* userClone = new LengthRecommenderUser(User::getName());
    userClone->contentSumLength = LengthRecommenderUser::contentSumLength;
    userClone->contentNum = LengthRecommenderUser::contentNum;
    //userClone->history = User::history;
    for (unsigned i = 0; i < User::history.size(); ++i) {
        std::vector<Watchable*>::iterator it = std::find(originalContent.begin(), originalContent.end(), history.at(i));
        if (it != originalContent.end())
            (userClone->history).push_back(cloneContent.at(std::distance(originalContent.begin(), it)));
    }
    return userClone;
}

User* RerunRecommenderUser::clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent){
    RerunRecommenderUser* userClone = new RerunRecommenderUser(User::getName());
    userClone->indexOfRec = RerunRecommenderUser::indexOfRec;
    //userClone->history = User::history;
    for (unsigned i = 0; i < User::history.size(); ++i) {
        std::vector<Watchable*>::iterator it = std::find(originalContent.begin(), originalContent.end(), history.at(i));
        if (it != originalContent.end())
            (userClone->history).push_back(cloneContent.at(std::distance(originalContent.begin(), it)));
    }
    return userClone;
}

User* GenreRecommenderUser::clone(std::vector<Watchable*> cloneContent,std::vector<Watchable*> originalContent){
    GenreRecommenderUser* userClone = new GenreRecommenderUser(User::getName());
    userClone->userTags = GenreRecommenderUser::userTags; ///Is it a copy of pairs??
    //userClone->history = User::history;
    for (unsigned i = 0; i < User::history.size(); ++i) {
        std::vector<Watchable*>::iterator it = std::find(originalContent.begin(), originalContent.end(), history.at(i));
        if (it != originalContent.end())
            (userClone->history).push_back(cloneContent.at(std::distance(originalContent.begin(), it)));
    }
    return userClone;
}