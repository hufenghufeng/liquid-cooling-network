#ifndef SUBCHIP_H
#define SUBCHIP_H

#include "tmap.h"
#include "mchannel.h"
#include "result.h"
#include "configure.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <iostream>

#include "CoolingSystem.h"
#include "OptSpec.h"
#include "Parser.h"
#include "Simulator.h"
#include "Channel.h"

using std::vector;
using std::string;
using std::iostream;
using std::sort;

class Subchip
{
public:
//    Subchip();
    /// \brief construct function
      Subchip(size_t N_h, size_t N_w,const char* topTmapFile, const char* bottomTmapFile, const char* channelFile){

        _N_h=N_h;
        _N_w=N_w;

        topLayer=Tmap(N_h,N_w,topTmapFile);
        bottomLayer=Tmap(N_h,N_w,bottomTmapFile);

        tempTopTmap=Tmap(N_h,N_w,topTmapFile);
        tempBottomTmap=Tmap(N_h,N_w,bottomTmapFile);

        channel=mchannel(N_h,N_w);
        tempChannel=mchannel(N_h,N_w);

        CE=0.0;
    }
    /// \brief construct function
    Subchip(size_t N_h, size_t N_w){
        _N_h=N_h;
        _N_w=N_w;

        topLayer=Tmap(N_h,N_w);
        bottomLayer=Tmap(N_h,N_w);

        tempTopTmap=Tmap(N_h,N_w);
        tempBottomTmap=Tmap(N_h,N_w);

        channel=mchannel(N_h,N_w);
        tempChannel=mchannel(N_h,N_w);

        CE=0.0;
    }

    void config(const string caseName, int Steps, int channelIndex,double ce){
         //myconfig=configure(caseName,Steps,channelIndex,ce);
        myconfig=configure("case.ini");
    }

    void config(const char* ini){
        myconfig=configure(ini);
    }

    /// \brief initial the result
    void initialResult(double setCoolingEnergy,CoolingSystem& sys);

    /// \brief clean result for next result history
    void cleanResult();

    /// \brief according to the index you input, initial the channel to the corresponding index
    void initialChannel(int index);

    /// \brief remove the liquid cell with little or no liquid flow through it
    void removeDeadCell(double pThred);

    /// \brief check whether cell in (i,j) is dead cell or not and remove them, these cell are not I/O
    void checkDeadInnerCell(int i,int j,ddmat &pressure,double pThred);

    /// \brief remove dead IO cells
    void removeDeadIO(mchannel& ch,ddmat pressure,double pThred);

    /// \brief choose the best IO pattern
    int chooseBestIO(CoolingSystem& sys);

    /// \brief optimize  barrier
    void optimizeBarrier(double setCoolingEnergy, CoolingSystem &sys);

    /// \brief find where is cold and then fill there
    void fillColdCell(int num);

    /// \brief fill hot area with pattern
    void fillHotArea(double percent);

    /// \brief fill the hot area in output area
    void fillOutputArea(int size);

    /// \brief fill the hot area in center area
    void fillCenterArea(int size);

    /// \brief Search for the CE that satisfier temperature constraints, if no CE satisfies, return the nearest.
    double searchCE(double initCE,double TmaxConst, double DTMax, double Thred,CoolingSystem& sys);

    /// \brief Binary search for the cooling energy if upper and lower bound is given
    double binarySearchCE(double lowerBoundry, double upperBoundry,double dtConst, double TmaxConst, double Thred,CoolingSystem& sys);

    /// \brief you need to set the cooling enery and the where to read the Tmap file
    void generateTempTmap(double setCoolingEnergy,CoolingSystem& sys,dvec& x0);

    //void generateTmap(double setCoolingEnergy);

    /// \brief Decide whether accepting the new channel or not
    void evaluateTempTmap(int index);

    /// \brief main function, begin filling channels and taking down results
    void beginFilling(CoolingSystem& sys);

    /// \brief output the best Tmap
    void outputBestTmap(CoolingSystem& sys);

    void keepResult();

    double getCoolingEnergy();

    void setCE(double ce);

    double getPin();

    void setPin(double pin);

    double getMaxDT();

    double getMaxT();

    double getTempMaxDT();

    double getTempMaxT();

    void setSimulator(CoolingSystem& sys){
        sim.set(sys);
    }

    void setSysChannel(CoolingSystem& sys){
        sys.set_channel(0,ch);
    }

    mchannel getChannel(){
        return channel;
    }

    mchannel getTempChannel(){
        return tempChannel;
    }

private:
    size_t _N_h;
    size_t _N_w;

    Tmap topLayer;
    Tmap bottomLayer;
    mchannel channel;
    /// \brief temp data
    Tmap tempTopTmap;
    Tmap tempBottomTmap;
    mchannel tempChannel;
    /// \brief result
    Result resultHistory;
   // Result barrierHistory;

    configure myconfig;
    double CE;

//    Parser* parser;
//    CoolingSystem* sys;
//    OptSpec* spec;

    Simulator sim;
    /// \brief channel for simulation
    Channel ch;
};

#endif // SUBCHIP_H
