#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    //virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual Watchable* clone() = 0;
    //void cloneParent(Watchable& watchable); //no need

    int getLength() const;
    std::string toStringTags() const;
    virtual int getNextWatchable() const = 0;
    long getContentId() const;
    bool hasTag(std::string tagName) const;
    std::vector<std::string> getTags();
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    //virtual Watchable* getNextWatchable(Session&) const;
    virtual int getNextWatchable() const;
    virtual Watchable* clone(); //const? cant because need to copy vec


private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    //virtual Watchable* getNextWatchable(Session&) const;
    void setNextEpisodeId(long nextEpisodeId);
    virtual int getNextWatchable() const;
    virtual Watchable* clone();


private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
