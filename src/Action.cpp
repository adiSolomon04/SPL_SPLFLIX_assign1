//
// Created by eran on 11/25/19.
//

#include <iostream>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

using namespace std;

BaseAction::BaseAction(): errorMsg(""), status(PENDING){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    (*this).errorMsg = errorMsg;
    std::cout <<"Error - " + errorMsg << std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return "ERROR: " + errorMsg;
}




void CreateUser::act(Session &sess) {
    std::string actionStd =sess.getActionText();
    std::string name =actionStd.substr(0,actionStd.find(" "));
    std::string recommendationAlgorithm= actionStd.substr(actionStd.find(" ")+1);//works?
    if(sess.userNameExsist(name)){ /*user name exist*/
        (*this).error("User Name already Exists");
    }
    else{
        if(recommendationAlgorithm=="len") {
            sess.pushUser(new LengthRecommenderUser(name));
            this->complete();
        }
        else if(recommendationAlgorithm=="rer") {
            sess.pushUser(new RerunRecommenderUser(name));
            this->complete();
        }
        else if(recommendationAlgorithm=="gen"){
            sess.pushUser(new GenreRecommenderUser(name));
            this->complete();
        }
        else{
           (*this).error("User Algorithm does not Exists");
        }


    }
}

std::string CreateUser::toString() const {
    std::string text = "CreateUser ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

void ChangeActiveUser::act(Session &sess) {
    std::string name = sess.getActionText();
    if (sess.setActiveUser(name)) /*user name exist*/
        this->complete();
    else
        (*this).error("User does not exist");
}

std::string ChangeActiveUser::toString() const {
    std::string text = "ChangeActiveUser ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

void DeleteUser::act(Session &sess) {
    std::string name = sess.getActionText();
    if (sess.deleteUser(name)) { /*user name exist*/
        this->complete();
    }else{
        (*this).error("User does not exists");
    }
}

std::string DeleteUser::toString() const {
    std::string text = "DeleteUser ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

void DuplicateUser::act(Session &sess) {
    std::string actionStd = sess.getActionText();
    std::string originalUserName = actionStd.substr(0, actionStd.find(" "));
    std::string newUserName = actionStd.substr(actionStd.find(" ") + 1);
    if (!sess.userNameExsist(originalUserName)) {
        (*this).error("the original user doesn't exist");
    } else if(sess.userNameExsist(newUserName)){
        (*this).error("the new user name is already taken");
    }else{
        User* newUser = sess.getUser(originalUserName)->duplicateUser(newUserName);
        sess.pushUser(newUser);
        this->complete();
    }
}

std::string DuplicateUser::toString() const {
    std::string text = "DuplicateUser ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

void PrintActionsLog::act(Session &sess) {
    sess.printActionsLog();
    this->complete();
}

std::string PrintActionsLog::toString() const {
    std::string text = "PrintActionsLog ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;}

void PrintContentList::act(Session& sess){
    sess.printContent();
    this->complete();
}

std::string PrintContentList::toString() const {
    std::string text = "PrintContentList ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

void PrintWatchHistory::act(Session& sess){
    std::vector<Watchable*> watchhis = sess.getActiveUser()->get_history();
    std::string printText = "Watch history for " + sess.getActiveUser()->getName()+"\n";
    for (size_t i = 0; i < watchhis.size(); i++)
    {
        printText+= std::to_string(i+1)+". ";
        printText+= (*watchhis.at(i)).toString()+ "\n";
    }
    std::cout << printText;
    this->complete();
}

std::string PrintWatchHistory::toString() const {
    std::string text = "PrintWatchHistory ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;}

void Watch::act(Session& sess){
    long contentId = std::stol(sess.getActionText());
    sess.watchContentId(contentId);
    this->complete();
}


std::string Watch::toString() const {
    std::string text = "Watch ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

std::string Exit::toString() const {
    std::string text = "Exit ";
    if(this->getStatus() == COMPLETED)
        text += "COMPLETED";
    else if(this->getStatus() == ERROR)
        text += this->getErrorMsg();
    else if(this->getStatus() == PENDING)
        text += "PENDING";
    return text;
}

void Exit::act(Session &sess) {
    sess.exit();
    this->complete();
}

void BaseAction::cloneParent(BaseAction& baseAction){
    errorMsg = baseAction.errorMsg;
    status = baseAction.status;
}

BaseAction::~BaseAction() {

}

BaseAction* CreateUser::clone(){
    BaseAction* actionClone = new ChangeActiveUser();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* ChangeActiveUser::clone(){
    BaseAction* actionClone = new ChangeActiveUser();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* DeleteUser::clone(){
    BaseAction* actionClone = new DeleteUser();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* DuplicateUser::clone(){
    BaseAction* actionClone = new DuplicateUser();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* PrintContentList::clone(){
    BaseAction* actionClone = new PrintContentList();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* PrintWatchHistory::clone(){
    BaseAction* actionClone = new PrintWatchHistory();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* Watch::clone(){
    BaseAction* actionClone = new Watch();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* PrintActionsLog::clone(){
    BaseAction* actionClone = new PrintActionsLog();
    (*actionClone).cloneParent(*this);
    return actionClone;
}

BaseAction* Exit::clone(){
    BaseAction* actionClone = new Exit();
    (*actionClone).cloneParent(*this);
    return actionClone;
}
