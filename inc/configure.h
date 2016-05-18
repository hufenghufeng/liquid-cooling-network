#ifndef CONFIGURE_H
#define CONFIGURE_H
#include <string>

using std::string;

class configure
{
public:
    configure();

    configure(std::string casename, int steps, int channelIndex);

    int totalFillStep(){
        return totalStep;
    }

    int getFillNum(){
        return fillNum;
    }

    float getTmaxConstraint(){
        return TmaxConstraint;
    }

    int getInitialIndex(){
        return initialIndex;
    }



private:
    int totalStep;
    int fillNum;
    float TmaxConstraint;
    std::string caseName;
    int initialIndex;
};

#endif // CONFIGURE_H
