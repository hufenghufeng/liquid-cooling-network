#ifndef MULTILAYERCHIP_H
#define MULTILAYERCHIP_H

#include "tmap.h"
#include "mchannel.h"
#include "result.h"
#include "configure.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include<vector>

#include "multilayresult.h"
#include "CoolingSystem.h"
#include "OptSpec.h"
#include "Parser.h"
#include "Simulator.h"
#include "Channel.h"

using namespace std;

class multilayerchip
{
public:
    multilayerchip(size_t N_h, size_t N_w, int channeLayerNum){
        _N_h=N_h;
        _N_w=N_w;
        channelLayerNum=channeLayerNum;

        for(int i=0;i<channelLayerNum;++i){
            mchannelVec.push_back(mchannel(N_h, N_w));
            tempMchannelVec.push_back(mchannel(N_h, N_w));
            TmapVec.push_back(Tmap(N_h,N_w));
            tempTmapVec.push_back(Tmap(N_h,N_w));
            channelVec.push_back(Channel(N_h,N_w));
        }
        TmapVec.push_back(Tmap(N_h,N_w));
        tempTmapVec.push_back(Tmap(N_h,N_w));

        CE=0.0;

        resultHistory=multiResult(_N_h,_N_w,channeLayerNum);
    }

    void config(const string caseName, int Steps, int channelIndex,double ce){
         myconfig=configure(caseName,Steps,channelIndex,ce);
    }

    void config(const char* ini){
        myconfig=configure(ini);
    }
    /// to do: optimize barrier

    /// \brief initial the result
    void initialResult(double setCoolingEnergy,CoolingSystem& sys);

    /// \brief clean result for next result history
    void cleanResult();

    /// \brief according to the index you input, initial the channel to the corresponding index
    void initialChannel(int index);

    /// \brief initial channel from file
    bool initialChannelFromFile(const char* channel_template_file,unsigned int layerIndex);

    /// \brief remove the liquid cell with little or no liquid flow through it
    void removeDeadCell(double pThred);

    /// \brief check whether cell in (i,j) is dead cell or not and remove them, these cell are not I/O
    void checkDeadInnerCell(int i,int j,ddmat &pressure,double pThred);

    /// \brief remove dead IO cells
    void removeDeadIO(mchannel& ch,ddmat pressure,double pThred);

    /// \brief find where is cold and then fill there
    void fillColdCell(int num);

    /// \brief Search for the CE that satisfier temperature constraints, if no CE satisfies, return the nearest.
    double searchCE(double initCE,double TmaxConst, double DTMax, double Thred,CoolingSystem& sys);

    /// \brief Binary search for the cooling energy if upper and lower bound is given
    double binarySearchCE(double lowerBoundry, double upperBoundry,double dtConst, double TmaxConst, double Thred,CoolingSystem& sys);

    /// \brief you need to set the cooling enery and the where to read the Tmap file
    void generateTempTmap(double setCoolingEnergy,CoolingSystem& sys,dvec& x0);

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
        for(int i=0;i<channelLayerNum;++i)
            sys.set_channel(i,channelVec[i]);
    }

    mchannel getChannel(int index){
        assert(index<channelLayerNum);
        return mchannelVec[index];
    }

    mchannel getTempChannel(int index){
        assert(index<channelLayerNum);
        return tempMchannelVec[index];
    }

private:
    size_t _N_h;
    size_t _N_w;

    int channelLayerNum;

    vector<Tmap> TmapVec;
    vector<mchannel> mchannelVec;

    vector<Tmap> tempTmapVec;
    vector<mchannel> tempMchannelVec;

    /// \brief result
    multiResult resultHistory;
    configure myconfig;

    double CE;

    Simulator sim;
    /// \brief channel for simulation
    vector<Channel> channelVec;

};

#endif // MULTILAYERCHIP_H
