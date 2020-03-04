//
// Created by eran on 11/24/19.
//

#include <iostream>
#include <fstream>
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"

using namespace std;

Session::Session(const std::string &configFilePath):content(),actionsLog(),userMap(),activeUser(nullptr),actionText(""),active(false){
    //read jason and put content(Watchable)
    //actionsLog?
    //userMap v
    //activeUser v
    User* defaultUser = new LengthRecommenderUser("default");
    userMap.insert({defaultUser->getName(),defaultUser});
    activeUser = defaultUser;

    // read a JSON file
    std::ifstream i(configFilePath);  //configFilePath
    nlohmann::json j;
    i >> j;
    long id_next = 1;

    for (size_t k = 0; k < j["movies"].size(); k++)
    {
        Watchable* content_movie = new Movie (id_next, j["movies"][k]["name"], j["movies"][k]["length"], j["movies"][k]["tags"]);
        id_next++;
        content.push_back(content_movie);
    }
    Episode* content_episode;
    for (size_t k = 0; k < j["tv_series"].size(); k++)
    {
        std::string series_name = j["tv_series"][k]["name"];
        int series_length = j["tv_series"][k]["episode_length"];
        std::vector<std::string> series_tags = j["tv_series"][k]["tags"];
        for (size_t l = 0; l < j["tv_series"][k]["seasons"].size(); l++)
        {
            for (size_t m = 0; m < j["tv_series"][k]["seasons"][l]; m++)
            {
                content_episode = new Episode (id_next, series_name, series_length, l+1, m+1, series_tags);
                id_next++;
                (*content_episode).setNextEpisodeId(id_next);
                content.push_back(content_episode);
            }

        }
        (*content_episode).setNextEpisodeId(0);
    }
}

void Session::start() {
    std::cout << "SPLFLIX is now on" <<endl;

    active = true;
    while (active) 
    {
        //Wait for income Actions
        //std::cin >> actionText;
        getline(std::cin,actionText);
        std::string actName = actionText.substr(0,actionText.find(" "));
        //next text for act
        actionText = actionText.substr(actionText.find(" ")+1);
        if(actName=="createuser") {
            BaseAction* baseAction = new CreateUser();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "changeuser")
        {
            BaseAction* baseAction = new ChangeActiveUser();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "deleteuser")
        {
            BaseAction* baseAction = new DeleteUser();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "dupuser")
        {
            BaseAction* baseAction = new DuplicateUser();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "content")
        {
            BaseAction* baseAction = new PrintContentList();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "watchhist")
        {
            BaseAction* baseAction = new PrintWatchHistory();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "watch")
        {
            BaseAction* baseAction = new Watch();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "log")
        {
            BaseAction* baseAction = new PrintActionsLog();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
        else if (actName == "exit")
        {

            BaseAction* baseAction = new Exit();
            actionsLog.push_back(baseAction);
            (*baseAction).act(*this);
        }
    }
}

std::string Session::getActionText()  {
    return actionText;
}


bool Session::userNameExsist(std::string &user_name) {
    return userMap.find(user_name)!= userMap.end();
}

void Session::pushUser(User *user) {
    userMap.insert({user->getName(),user});
}

bool Session::setActiveUser(std::string& activeUser) {
    if(userMap.find(activeUser)!=userMap.end()){
        Session::activeUser = (*userMap.find(activeUser)).second;
        return true;
    }
    else
        return false;
}

bool Session::deleteUser(std::string& userName) {
    if (userMap.find(userName) != userMap.end())
        delete (*userMap.find(userName)).second; //prevent memory leak
    return userMap.erase(userName) != 0;
}
User *Session::getUser(std::string &name) {
    return (*userMap.find(name)).second;
}


void Session::printContent(){
    std::string printText;
    for (size_t i = 0; i < content.size(); i++)
    {
        printText+=std::to_string(i+1)+". "+(*content[i]).toString()
                   +" "+std::to_string((*content[i]).getLength())+" minutes"+
                   "["+(*content[i]).toStringTags()+"]\n";
    }
    std::cout << printText;
}

User* Session::getActiveUser(){
    return activeUser;
}

Watchable* Session::lengthRecommend(User* lenUser, double contentAvg){
    //check the closest that isnt on history
    double minLenDiff = std::numeric_limits<double>::infinity();//suppose no more than a 200 min movie
    Watchable* lenContent;
    for (size_t i = 0; i < content.size(); i++)
    {
        //diff id smaller and not in history
        if(std::abs((*content[i]).getLength()-contentAvg)<minLenDiff &&
           !(*lenUser).isInHistory(content[i])){
            minLenDiff = std::abs((*content[i]).getLength()-contentAvg);
            lenContent = content[i];
        }
    }
    return lenContent;
}


void Session::watchContentId(long contentId) {
    Watchable *watchContent = content[contentId - 1];
    std::cout << "Watching " + (*watchContent).toString()+"\n";
    (*activeUser).addToHistory(watchContent);
    if ((*watchContent).getNextWatchable() != 0) {//series
        std::cout << "We recommend watching " + (*content[contentId]).toString() + ", continue watching? [y/n]\n";
        std::string inputYN;
        std::cin >> inputYN;
        if (inputYN == "y") {//Recommandation of Next episode
            actionText = std::to_string(contentId + 1);
            BaseAction *baseAction = new Watch();
            actionsLog.push_back(baseAction);//here or after the act??
            (*baseAction).act(*this);
        }
    } else {//Movie or end of Series
        Watchable *contentRec = (*activeUser).getRecommendation(*this);
        if(contentRec!= nullptr) {
            std::cout << "We recommend watching " + (*contentRec).toString() + ", continue watching? [y/n]\n";
            std::string inputYN;
            std::cin >> inputYN;
            if (inputYN == "y") {//Recommandation by Algorithm
                actionText = std::to_string((*contentRec).getContentId());
                BaseAction *baseAction = new Watch();
                actionsLog.push_back(baseAction);
                (*baseAction).act(*this);
            }
        }
    }
}

void Session::printActionsLog() {
    for (unsigned i = 0; i < actionsLog.size() ; ++i) {
        std::cout<<actionsLog[i]->toString() << endl ;
    }
}

void Session::exit() {
    active = false;
}

Watchable *Session::genreRecommend(User *user, std::vector<std::pair<std::string, int> > userTags) {
    Watchable* rec = nullptr;
    bool found = false;
    for (unsigned i = 0; i < userTags.size()&&!found; ++i) {
        for (unsigned j = 0; j < content.size()&&!found ; ++j) {
            if(content.at(j)->hasTag(userTags.at(i).first)&&!user->isInHistory(content.at(j))){
                found = true;
                rec = content.at(j);
            }
        }
    }

    return rec;
}

Session::~Session(){ //destructor
    Session::clear();
}


Session::Session(const Session &other): content(),actionsLog(),userMap(),activeUser(nullptr),actionText(""),active(false) {//copy Constructor
    //copy all elements from vectors and make new vectors
    copy(other);
}


Session::Session(Session &&other)//move Constructor
//move of class type with std::move
        :content(std::move(other.content)), actionsLog(std::move(other.actionsLog)),
         userMap(std::move(other.userMap)), activeUser(other.activeUser),actionText(""),active(false) {
    other.activeUser = nullptr;
}

Session& Session::operator=(const Session &other){//copy assignment
    if(this!=&other)
    {
        Session::clear();
        copy(other);
    }
    return *this; //why *this? -adi
}
Session& Session::operator=(Session &&other){//move assignment
    Session::clear();
    Session::steal(other);
    return *this;
}

void Session::clear(){
    activeUser= nullptr;//delete in the user map
    for (unsigned i = 0; i <content.size() ; ++i) {
        delete content.at(i);
    }
    content.erase(content.begin(),content.end());
    for (unsigned i = 0; i <actionsLog.size() ; ++i) {
        delete actionsLog.at(i);
    }
    actionsLog.erase(actionsLog.begin(),actionsLog.end());
    for (auto it = userMap.begin(); it != userMap.end(); ++it ){
        delete it->second;
        //userMap.erase(it);
    }
    userMap.erase(userMap.begin(),userMap.end());

}
void Session::copy(const Session &other){ //for copy assign
    for (unsigned i = 0; i <other.content.size() ; ++i) {
        content.push_back((*(other.content.at(i))).clone());
    }

    for (unsigned i = 0; i <other.actionsLog.size() ; ++i) {
        actionsLog.push_back((*(other.actionsLog.at(i))).clone());
    }
    for (auto it = other.userMap.begin(); it != other.userMap.end(); ++it ){
        userMap.insert({it->first,  (*it->second).clone(content,other.content)});
    }
    activeUser = userMap.find((*other.activeUser).getName())->second;
}
void Session::steal(Session &other){
    //Move const of class std
    content = std::move(other.content);
    actionsLog = std::move(other.actionsLog);
    userMap = std::move(other.userMap);
    //Copy ptr & put null
    activeUser = other.activeUser;
    other.activeUser = nullptr;
}










