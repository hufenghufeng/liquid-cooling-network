#ifndef MULTILAYRESULT_H
#define MULTILAYRESULT_H
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include "CoolingSystem.h"
#include "Channel.h"

using std::vector;
using std::fstream;
using std::ostream;

class multiResult
{
public:
    multiResult(){
        _N_h=101;
        _N_w=101;
    }

    multiResult(size_t N_h,size_t N_w,int layerNum){
        _N_h=N_h;
        _N_w=N_w;
        channelLayerNum=layerNum;
        bestChannelVec.resize(channelLayerNum);
    }
/*
    ~multiResult(){
        clear();
    }

    void clear(){
        if (bestChannel!=NULL){
            delete [] bestChannel;
            bestChannel=NULL;
        }
    }

        void initialBestChannel(size_t rowSize, size_t colSize);
*/

    void cleanMultiResult();

    void setBestChannel(size_t rowSize, size_t colSize,int* channel,int layerIndex);

    void addBestChannel(int* channel);

    Channel& getBestChannel(unsigned int k);

    void clearBestChannel(){
        bestChannelVec.clear();
    }

    void outputMultiResult();

    void outputBestTmap(CoolingSystem& sys);

        //friend ostream& operator<<(ostream& os, const multiResult& rhs);

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
/*
    void addDTTop(double dtTop){
        dTTop.push_back(dtTop);
    }

    void addDTBottom(double dtBottom){
        dTBottom.push_back(dtBottom);
    }
*/
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

    void copyLastMultiResult(){
        pressure.push_back(pressure.at(pressure.size()-1));
        maxT.push_back(maxT.at(maxT.size()-1));
        dT.push_back(dT.at(dT.size()-1));
        CE.push_back(CE.at(CE.size()-1));
    }

    void changeLastMultiResult(double pin,double tmax,double dt,double ce){
        pressure.at(pressure.size()-1)=pin;
        maxT.at(maxT.size()-1)=tmax;
        dT.at(dT.size()-1)=dt;
        CE.at(CE.size()-1)=ce;
    }

private:
    vector<double> pressure;
    vector<double> maxT;
    vector<double> dT;
    vector<double> CE;

    double bestPressure;
    double bestTmax;
    double bestDeltaT;
    double bestCE;
    int   bestStep;

    double tempPin;

    vector<Channel> bestChannelVec;

    int channelLayerNum;
    size_t _N_h;
    size_t _N_w;
};

#endif // MULTILAYRESULT_H
