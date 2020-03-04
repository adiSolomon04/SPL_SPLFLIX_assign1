//
// Created by eran on 11/23/19.
//

#include <iostream>
#include "../include/Watchable.h"
using namespace std;

Watchable::Watchable(long id, int length, const std::vector<std::string>& tags):id(id), length(length),tags(tags){}//to-check: initialization vector???


int Watchable::getLength() const{
    return length;
}

std::string Watchable::toStringTags() const{
    std::string stringTags;
    for (size_t i = 0; i < tags.size()-1; i++)
        stringTags+= tags[i]+", ";
    stringTags+= tags[tags.size()-1];
    return stringTags;
}

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags),name(name) {}

std::string Movie::toString() const{
    return Movie::name;
}

Episode::Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags):Watchable(id,length,tags),seriesName(seriesName),  season(season),episode(episode), nextEpisodeId(0) {}

std::string Episode::toString() const{
    std::string toString = Episode::seriesName+" ";
    //season string
    if((*this).season<10)
        toString+="S0";
    else
        toString+="S";
    toString+=std::to_string((*this).season);
    //episode string
    if((*this).episode<10)
        toString+="E0";
    else
        toString+="E";
    toString+=std::to_string((*this).episode);
    return toString;
}

void Episode::setNextEpisodeId(long nextEpisodeId) {
    (this->nextEpisodeId)=nextEpisodeId;

}

int Episode::getNextWatchable() const{
    return nextEpisodeId;
}

int Movie::getNextWatchable() const{
    return 0;
}

long Watchable::getContentId() const{
    return id;
}

bool Watchable::hasTag(std::string tagName) const {
    for (unsigned i = 0; i <tags.size() ; ++i) {
        if(tags.at(i)==tagName)
            return true;
    }
    return false;
}

std::vector<std::string> Watchable::getTags() {
    return tags;
}

Watchable::~Watchable() {

}

//to-do:nextEpisodeId???
Watchable* Movie::clone(){
    return(new Movie(Watchable::getContentId(), name, Watchable::getLength(), Watchable::getTags()));
}
Watchable* Episode::clone(){
    return(new Episode(Watchable::getContentId(), seriesName, Watchable::getLength(), season, episode, Watchable::getTags()));
}


