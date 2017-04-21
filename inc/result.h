#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include "Channel.h"

using std::vector;
using std::fstream;
using std::ostream;

class Result
{
public:
    Result():bestChannel(NULL){
        _N_h=101;
        _N_w=101;
    }

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

    Channel getBestChannel();

    void outputResult();

        friend ostream& operator<<(ostream& os, const Result& rhs);

    double getLastDeltaT(){
        return dT.at(dT.size()-1);
    }

         void setBestPressure(double pressure){
            bestPressure=pressure;
         }

         double getBestPressure(){
             return bestPressure;
         }

         void setBestTmax(double Tmax){
                bestTmax=Tmax;
         }

         double getBestTmax(){
             return bestTmax;
         }

         void setBestDeltaT(double deltaT){
             bestDeltaT=deltaT;
         }

         double getBestDeltaT(){
             return bestDeltaT;
         }

         double getBestCE(){
             return bestCE;
         }

         void setBestCE(double ce){
             bestCE=ce;
         }

         void setBestStep(int step){
             bestStep=step;
         }

         int getBestStep(){
                return bestStep;
         }

    void addPressure(double Pin){
        pressure.push_back(Pin);
    }

    void addMaxT(double Tmax){
        maxT.push_back(Tmax);
    }

    void addDTTop(double dtTop){
        dTTop.push_back(dtTop);
    }

    void addDTBottom(double dtBottom){
        dTBottom.push_back(dtBottom);
    }

    void addDT(double dt){
        dT.push_back(dt);
    }

    void addCE(double ce){
        CE.push_back(ce);
    }

    void setTempPin(double p){
        tempPin=p;
    }

    double getTempPin(){
        return tempPin;
    }

    void copyLastResult(){
        pressure.push_back(pressure.at(pressure.size()-1));
        maxT.push_back(maxT.at(maxT.size()-1));
        dTTop.push_back(dTTop.at(dTTop.size()-1));
        dTBottom.push_back(dTBottom.at(dTBottom.size()-1));
        dT.push_back(dT.at(dT.size()-1));
        CE.push_back(CE.at(CE.size()-1));
    }

    void changeLastResult(double pin,double tmax,double dtTop,double dtBottom,double dt,double ce){
        pressure.at(pressure.size()-1)=pin;
        maxT.at(maxT.size()-1)=tmax;
        dTTop.at(dTTop.size()-1)=dtTop;
        dTBottom.at(dTBottom.size()-1)=dtBottom;
        dT.at(dT.size()-1)=dt;
        CE.at(CE.size()-1)=ce;
    }

    void cleanResult();

private:
    vector<double> pressure;
    vector<double> maxT;
    vector<double> dTTop;
    vector<double> dTBottom;
    vector<double> dT;
    vector<double> CE;

    double bestPressure;
    double bestTmax;
    double bestDeltaT;
    double bestCE;
    int   bestStep;

    double tempPin;

    int* bestChannel;
    size_t _N_h;
    size_t _N_w;
};

#endif // RESULT_H
