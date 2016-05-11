#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include <stdlib.h>
using std::vector;
class result
{
public:
    result();

    ~result(){
        clear();
    }

    void clear(){
        if (bestChannel!=NULL){
            delete [] bestChannel;
            bestChannel=NULL;
        }
    }

private:
    vector<float> pressure;
    vector<float> maxT;
    vector<float> minT;
    vector<float> dT;

    float bestPressure;
    float bestTmax;
    float bestDeltaT;
    int   bestStep;

    int* bestChannel;
};

#endif // RESULT_H
