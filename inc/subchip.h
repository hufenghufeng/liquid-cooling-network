#ifndef SUBCHIP_H
#define SUBCHIP_H

#include "tmap.h"
#include "Channel.h"
#include <vector>

using std::vector;

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
         Channel channel(N_h,N_w,channelFile);

        tempTopTmap=topLayer;
        tempBottomTmap=bottomLayer;
       tempChannel=channel;
    }

    void fillColdCell(size_t num);
    /// \brief you need to set the cooling enery and the where to read the Tmap file
    void generateTempTmap(float setCoolingEnergy);

    float getCoolingEnergy();

    void evaluateTempTmap();
private:
    size_t _N_h;
    size_t _N_w;

    Tmap topLayer;
    Tmap bottomLayer;
    Channel channel;
    /// \brief temp data
    Tmap tempTopTmap;
    Tmap tempBottomTmap;
    Channel tempChannel;
};

#endif // SUBCHIP_H
