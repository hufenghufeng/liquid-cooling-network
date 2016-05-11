#include "subchip.h"

void Subchip::fillColdCell(size_t num){
    vector<int> fillIndex;

    if (topLayer.getDeltaT()>bottomLayer.getDeltaT()){
        fillIndex=channel.chooseCell(topLayer.getTmap(),num);
        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
        }
    }
    else{
        fillIndex=channel.chooseCell(bottomLayer.getTmap(),num);
        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
        }
    }
}

void Subchip::generateTempTmap(float setCoolingEnergy){
    /// how to run simulation under the same cooling energy
    float coolingEnergy=0.0;

    coolingEnergy=getCoolingEnergy();

    system("~/new-ice/bin/new-ice case1/test_case_01.stk");
    tempTopTmap.updateTmap("~/new-ice/bin/output1.txt");
    tempBottomTmap.updateTmap("~/new-ice/bin/output2.txt");
}

float Subchip::getCoolingEnergy(){
    std::ifstream fin;
    std::string line;
    float coolingEnergy;
    fin.open("~/new-ice/bin/cooling_energy.dat");

    if(!fin){
  //      cerr<<"open cooling energy file ERROR!"<<std::endl;
    }
    getline(fin,line);
    coolingEnergy=atof(line.c_str());

    return coolingEnergy;
}
