#ifndef CONFIGURE_H
#define CONFIGURE_H
#include <string>
#include "../../lib/iniparser/src/iniparser.h"
using std::string;

class configure
{
public:
    configure();

    configure(std::string casename, int steps, int channelIndex,double CE);

    configure(const char *ini);

    int totalFillStep(){
        return totalStep;
    }

    int getFillNum(){
        return fillNum;
    }

    double getTmaxConstraint(){
        return TmaxConstraint;
    }

    int getInitialIndex(){
        return initialIndex;
    }

    void setInitialIndex(int index){
        initialIndex=index;
    }

    double getCE(){
        return CE;
    }

    bool isStartFromBegin(){
        return startFromBegin;
    }

    bool needChangeCE(){
        return changeCE;
    }

    bool needFindIO(){
        return findIO;
    }

    bool needFindBarrier(){
        return findBarrier;
    }

    bool needPlaceStraightPattern(){
        return placeStraightPattern;
    }

    bool needFillTinyFlow(){
        return fillTinyFlowCell;
    }

    double getTinyFlowThred(){
        return pthreld;
    }

    int getFlowCheckInterval(){
        return tinyFlowCheckInterval;
    }

private:
    int totalStep;
    int fillNum;
    double TmaxConstraint;
    std::string caseName;
    int initialIndex;
    double CE;

    bool startFromBegin;
    bool changeCE;
    bool findIO;
    bool findBarrier;
    bool placeStraightPattern;

    bool fillTinyFlowCell;
    double pthreld;
    int tinyFlowCheckInterval;
};

#endif // CONFIGURE_H
