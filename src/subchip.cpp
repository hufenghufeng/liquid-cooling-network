#include "subchip.h"
using std::fstream;

void Subchip::fillColdCell(int num){
    std::vector<int> fillIndex;

    if (topLayer.getDeltaT()>bottomLayer.getDeltaT()){
        const float* tmap=topLayer.getTmap();
        std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
        fillIndex=channel.chooseCell(topLayer.getTmap(),num);

        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
            channel.markCell(*iter);
            tempChannel.markCell(*iter);
            std::cout<<"fill location is :"<<*iter;
        }
    }
    else{
        fillIndex=channel.chooseCell(bottomLayer.getTmap(),num);
        const float* tmap=topLayer.getTmap();
        std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
            channel.markCell(*iter);
            tempChannel.markCell(*iter);
            std::cout<<"fill location is :"<<*iter;
        }
    }
}

void Subchip::generateTempTmap(float setCoolingEnergy){
    /// how to run simulation under the same cooling energy
    float testPin=3000.0;

    std::ofstream writeChannel;
    writeChannel.open("/home/hf/new-ice/bin/channel1.dat");
    if (!writeChannel.is_open()){
        std::cout<<"can't open channel file for write!";
        writeChannel.clear();
    }

    writeChannel<<tempChannel;
    writeChannel.close();
    setPin(testPin);
    system("/home/hf/new-ice/bin/do_simulate.sh");

    float testCoolingEnenrgy=getCoolingEnergy();
    float setpin=0.0;
    std::cout<<"test Pin:"<<testPin<<" testCoolingEnergy:"<<testCoolingEnenrgy<<" /"<<testPin*testPin/testCoolingEnenrgy<<std::endl;
    setpin=testPin*sqrt(setCoolingEnergy/testCoolingEnenrgy);
    std::cout<<"set Pin:"<<setpin<<" setCoolingEnergy:"<<setCoolingEnergy<<" /"<<setpin*testPin/setCoolingEnergy<<std::endl;

    setPin(setpin);
    //coolingEnergy=getCoolingEnergy();

    system("/home/hf/new-ice/bin/do_simulate.sh");
    tempTopTmap.updateTmap("/home/hf/new-ice/bin/output1.txt");
    tempBottomTmap.updateTmap("/home/hf/new-ice/bin/output2.txt");

    resultHistory.setTempPin(setpin);
}



void Subchip::evaluateTempTmap(int index){

    float TmaxConstraints=360.0;
    tempTopTmap.caculateTmap();
    tempBottomTmap.caculateTmap();

    float Tmax=std::max(tempTopTmap.getTmax(),tempBottomTmap.getTmax());
    float dTtop=tempTopTmap.getDeltaT();
    float dTbottom=tempBottomTmap.getDeltaT();
    float dTmax=std::max(dTtop,dTbottom);

    if ((dTmax<resultHistory.getLastDeltaT())){
//    if ((Tmax<TmaxConstraints)&&(dTmax<resultHistory.getLastDeltaT())){
        //accept
        std::cout<<"accept!"<<std::endl;
        topLayer   =tempTopTmap;
        bottomLayer=tempBottomTmap;
        channel    =tempChannel;

        resultHistory.addPressure(resultHistory.getTempPin());
        resultHistory.addMaxT(Tmax);
        resultHistory.addDT(dTmax);
        resultHistory.addDTTop(dTtop);
        resultHistory.addDTBottom(dTbottom);

        resultHistory.setBestChannel(_N_h,_N_w,tempChannel.getChannel());
        resultHistory.setBestDeltaT(dTmax);
        resultHistory.setBestPressure(resultHistory.getTempPin());
        resultHistory.setBestTmax(Tmax);
        resultHistory.setBestStep(index);
    }
    else{
        //refuse
        std::cout<<"rejected!"<<std::endl;
        resultHistory.copyLastResult();
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

    std::string line;

    std::ifstream ffin;

//        fin.open("/home/hf/new-ice/bin/cooling_energy.dat");



    float coolingEnergy;
    ffin.open("/home/hf/new-ice/bin/cooling_energy.dat",std::ios_base::in);

    if(!ffin.is_open()){
        std::cerr<<"open /home/hf/new-ice/bin/cooling_energy.dat ERROR!"<<std::endl;
        ffin.clear();
    }
    getline(ffin,line);
    coolingEnergy=atof(line.c_str());

    ffin.close();

    return coolingEnergy;
//    return 0.01;
}

float Subchip::getPin(){
    float IOpressure;
    std::ifstream fin;
    fin.open("/home/hf/new-ice/bin/Pin.dat");

    if(!fin.is_open()){
        std::cerr<<"open /home/hf/new-ice/bin/Pin.dat ERROR!"<<std::endl;
        fin.clear();
    }

    fin>>IOpressure;
    fin.close();

    return IOpressure;
}

void Subchip::setPin(float Pin){
    int height=200;
    std::ofstream fout1;
    fout1.open("/home/hf/new-ice/bin/Pin.dat");

    fout1<<Pin<<'\t'<<height;

    fout1.close();
}

void Subchip::beginFilling(){
    //std::ofstream debug_channel1;
    //debug_channel1.open("beforeinitial.dat");
    //debug_channel1<<channel;
    config("case1",10,1);
    initialChannel(1);
    std::ofstream debug_channel;
    debug_channel.open("debugChannel.dat");
    debug_channel<<tempChannel;
    generateTempTmap(0.001);
    initialResult(0.001);
    for(int i=2;i<myconfig.totalFillStep();i++){

        std::ofstream debug_channel2;
        debug_channel2.open("beforechooseCell.dat");
        debug_channel2<<tempChannel;

        fillColdCell(1);
        generateTempTmap(0.001);
        evaluateTempTmap(i);
        //keepResult();
    }
    resultHistory.outputResult();
}

void Subchip::initialChannel(int index){
    channel.initialChannel(index);
    tempChannel=channel;
}
