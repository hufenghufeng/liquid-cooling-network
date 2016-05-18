#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string.h>

using std::vector;
using std::fstream;
using std::ostream;

class Result
{
public:
    Result(){};

    ~Result(){
        clear();
    }

    void clear(){
        if (bestChannel!=NULL){
            delete [] bestChannel;
            bestChannel=NULL;
        }
    }

        void initialBestChannel(size_t rowSize, size_t colSize);

    void setBestChannel(size_t rowSize, size_t colSize,int* channel);

    void outputResult();

        friend ostream& operator<<(ostream& os, const Result& rhs);

    float getLastDeltaT(){
        return dT.at(dT.size()-1);
    }

         void setBestPressure(float pressure){
            bestPressure=pressure;
         }

         float getBestPressure(){
             return bestPressure;
         }

         void setBestTmax(float Tmax){
                bestTmax=Tmax;
         }

         float getBestTmax(){
             return bestTmax;
         }

         void setBestDeltaT(float deltaT){
             bestDeltaT=deltaT;
         }

         float getBestDeltaT(){
             return bestDeltaT;
         }

         void setBestStep(int step){
             bestStep=step;
         }

         int getBestStep(){
                return bestStep;
         }

    void addPressure(float Pin){
        pressure.push_back(Pin);
    }

    void addMaxT(float Tmax){
        maxT.push_back(Tmax);
    }

    void addDTTop(float dtTop){
        dTTop.push_back(dtTop);
    }

    void addDTBottom(float dtBottom){
        dTBottom.push_back(dtBottom);
    }

    void addDT(float dt){
        dT.push_back(dt);
    }

    void setTempPin(float p){
        tempPin=p;
    }

    float getTempPin(){
        return tempPin;
    }

    void copyLastResult(){
        pressure.push_back(pressure.at(pressure.size()-1));
        maxT.push_back(maxT.at(maxT.size()-1));
        dTTop.push_back(dTTop.at(dTTop.size()-1));
        dTBottom.push_back(dTBottom.at(dTBottom.size()-1));
        dT.push_back(dT.at(dT.size()-1));
    }

private:
    vector<float> pressure;
    vector<float> maxT;
    vector<float> dTTop;
    vector<float> dTBottom;
    vector<float> dT;

    float bestPressure;
    float bestTmax;
    float bestDeltaT;
    int   bestStep;

    float tempPin;

    int* bestChannel;
    size_t _N_h;
    size_t _N_w;
};

#endif // RESULT_H
