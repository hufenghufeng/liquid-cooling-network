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
            channel.markCell(*iter);
            tempChannel.markCell(*iter);
        }
    }
    else{
        fillIndex=channel.chooseCell(bottomLayer.getTmap(),num);

        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
            channel.markCell(*iter);
            tempChannel.markCell(*iter);
        }
    }
}

void Subchip::generateTempTmap(float setCoolingEnergy){
    /// how to run simulation under the same cooling energy
    float testPin=3000.0;
    float testCoolingEnenrgy=getCoolingEnergy();
    float setpin=0.0;
    std::cout<<"test Pin:"<<testPin<<" testCoolingEnergy:"<<testCoolingEnenrgy<<" /"<<testPin*testPin/testCoolingEnenrgy<<std::endl;
    setpin=testPin*sqrt(setCoolingEnergy/testCoolingEnenrgy);
    std::cout<<"set Pin:"<<setpin<<" setCoolingEnergy:"<<setCoolingEnergy<<" /"<<setpin*testPin/setCoolingEnergy<<std::endl;

    setPin(setpin);
    //coolingEnergy=getCoolingEnergy();

    system("~/new-ice/bin/new-ice case1/test_case_01.stk");
    tempTopTmap.updateTmap("~/new-ice/bin/output1.txt");
    tempBottomTmap.updateTmap("~/new-ice/bin/output2.txt");

}



void Subchip::evaluateTempTmap(){

    float TmaxConstraints=360.0;
    tempTopTmap.caculateTmap();
    tempBottomTmap.caculateTmap();

    float Tmax=std::max(tempTopTmap.getTmax(),tempBottomTmap.getTmax());
    float dTtop=tempTopTmap.getDeltaT();
    float dTbottom=tempBottomTmap.getDeltaT();
    float dTmax=std::max(dTtop,dTbottom);

    if ((Tmax<TmaxConstraints)&&(dTmax<resultHistory.getLastDeltaT())){
        //accept
        topLayer   =tempTopTmap;
        bottomLayer=tempBottomTmap;
        channel    =tempChannel;


    }
    else{
        //refuse
    }

}

void Subchip::initialResult(float setCoolingEnergy){
    //(*this).generateTempTmap(setCoolingEnergy);
    generateTempTmap(setCoolingEnergy);

    topLayer   =tempTopTmap;
    bottomLayer=tempBottomTmap;

    float IOpressure=getPin();
    float Tmax=std::max(topLayer.getTmax(),bottomLayer.getTmax());
    float topDeltaT=topLayer.getDeltaT();
    float bottomDeltaT=bottomLayer.getDeltaT();

    resultHistory.addPressure(IOpressure);
    resultHistory.addMaxT(Tmax);
    resultHistory.addDTTop(topDeltaT);
    resultHistory.addDTBottom(bottomDeltaT);
    resultHistory.addDT(std::max(topDeltaT,bottomDeltaT));

    resultHistory.initialBestChannel(_N_h,_N_w);
    resultHistory.setBestChannel(_N_h,_N_w,channel.getChannel());

    resultHistory.setBestPressure(IOpressure);
    resultHistory.setBestTmax(Tmax);
    resultHistory.setBestDeltaT(std::max(topDeltaT,bottomDeltaT));
    resultHistory.setBestStep(1);
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

    fin.close();

    return coolingEnergy;
}

float Subchip::getPin(){
    float IOpressure;
    std::ifstream fin;
    fin.open("~/new-ice/bin/Pin.dat");
    fin>>IOpressure;
    fin.close();

    return IOpressure;
}

void Subchip::setPin(float Pin){
    int height=200;
    std::ofstream fout;
    fout.open("~/new-ice/bin/Pin.dat");
    fout<<Pin<<'\t'<<height;

    fout.close();
}

void Subchip::beginFilling(){
    initialChannel();
    generateTempTmap();
    initialResult();
    for(int i=2;i<configure.totalFillStep();i++){
        fillColdCell(1);
        generateTempTmap();
        evaluateTempTmap();
        keepResult();
    }
    resultHistory.outputResult();
}
