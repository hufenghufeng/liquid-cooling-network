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

         std::ofstream debug_channel1;
         debug_channel1.open("afterCreateChannel.dat");
         debug_channel1<<channel;

//         tempTopTmap=topLayer;
//         tempBottomTmap=bottomLayer;
//         tempChannel=channel;
    }

    void config(const string caseName, int Steps, int channelIndex){
         myconfig=configure(caseName,Steps,channelIndex);
    }

    void initialResult(float setCoolingEnergy);

    void initialChannel(int index);

    void fillColdCell(int num);
    /// \brief you need to set the cooling enery and the where to read the Tmap file
    void generateTempTmap(float setCoolingEnergy);

    void evaluateTempTmap(int index);

    void keepResult();

    float getCoolingEnergy();

    float getPin();

    void setPin(float pin);

    /// \brief main function
    void beginFilling();

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

    configure myconfig;
};

#endif // SUBCHIP_H
