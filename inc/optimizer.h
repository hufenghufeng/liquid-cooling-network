#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <vector>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include "CoolingSystem.h"
#include "OptSpec.h"
#include "Parser.h"
#include "Simulator.h"
#include "../new-optimizer/inc/subchip.h"
#include "../new-optimizer/inc/multilayerchip.h"

using namespace std;

class optimizer
{
public:
    optimizer();

    void optimize(char* casename);

   void  splitSystem(CoolingSystem& sys,vector<CoolingSystem>& systemVec){
       size_t num=sys.getChannelNum();
       for(size_t i=0;i<num;++i){
           CoolingSystem tempSys;
           tempSys.set_dim(sys.chip_length(),sys.chip_width(),sys.cell_length(),sys.cell_width());
           // find index of the i and i+1 channel layer
           size_t channelCounter=0;
           size_t beginIndex=0,endIndex=num;
           for(size_t j=0;j<sys.size();++j){
               channelCounter=0;
//               if (sys.layer_ptr(j)->layer_type==CHANNEL){
//                   channelCounter++;
 //              }
               if (channelCounter==i){
                   if(i==0)beginIndex=0;
                   else      beginIndex=j+1;
                   break;
               }
           }
           for(size_t j=0;j<sys.size();++j){
               channelCounter=0;
//               if (sys.layer_ptr(j)->layer_type==CHANNEL){
 //                  channelCounter++;
  //             }
               if(channelCounter==i+2){
                   endIndex=j;
                   break;
               }
           }


           // add layers
           //tempSys.add_die();
       }
   }

//private:
//    vector<CoolingSystem> sysVec;
//    CoolingSystem system;
    //Subchip chip;
    //multilayerchip wholechip;
};

#endif // OPTIMIZER_H
