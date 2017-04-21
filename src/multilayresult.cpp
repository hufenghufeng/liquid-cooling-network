#include "../inc/multilayresult.h"
#include <sstream>

void multiResult::cleanMultiResult(){
    pressure.clear();
    maxT.clear();
    dT.clear();
    CE.clear();

    bestPressure=99999999;
    bestTmax=9999999;
    bestDeltaT=9999999;
    bestCE=9999999;
    bestStep=-1;

    bestChannelVec.clear();
}

void multiResult::setBestChannel(size_t rowSize, size_t colSize,int* channel,int layerIndex){
    std::cout<<"channel layer num:"<<channelLayerNum<<std::endl;
    if(rowSize*colSize!=_N_h*_N_w){
        std::cout<<"channel size mismatch!";
    }
    if(layerIndex>=bestChannelVec.size()){
        std::cout<<"channel index exceed!"<<std::endl;
    }
    bestChannelVec[layerIndex]=Channel(rowSize,colSize,channel);
}

void multiResult::addBestChannel(int* channel){
    Channel tempChannel(_N_h,_N_w,channel);
    bestChannelVec.push_back(tempChannel);
}

Channel& multiResult::getBestChannel(unsigned int k){
    if(k>=bestChannelVec.size()){
        std::cout<<"best channel vector has not enough layer"<<std::endl;
    }
    return bestChannelVec[k];
}

void multiResult::outputMultiResult(){
    std::ofstream fout;
    //std::stringstream ss;
    std::stringstream ss;
    for(unsigned int k=0;k<bestChannelVec.size();++k){
        string fileName("bestChannel");
        string fileIndex;
        ss<<k;
        fileIndex=ss.str();
        //fileindex='0'+k-0;
        ss.str("");
        //ss.clear();
        fileName=fileName+fileIndex;
        fout.open(fileName.c_str());
        fout<<bestChannelVec[k];
        fout.close();
    }

   fout.open("history.dat");
    //  add % for the convenience of reading of matlab
   fout<<"% pressure"<<std::endl;
   for(vector<double>::iterator iter=pressure.begin();iter!=pressure.end();iter++){
       fout<<*iter<<' ';
   }
   fout<<std::endl;

   fout<<"% max T"<<std::endl;
    for(vector<double>::iterator iter=maxT.begin();iter!=maxT.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;
/*
    fout<<"% dt of top layer"<<std::endl;
    for(vector<double>::iterator iter=dTTop.begin();iter!=dTTop.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout<<"% dt of bottom layer"<<std::endl;
    for(vector<double>::iterator iter=dTBottom.begin();iter!=dTBottom.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;
*/
    fout<<"% dt "<<std::endl;
    for(vector<double>::iterator iter=dT.begin();iter!=dT.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout<<"% cooling energy "<<std::endl;
    for(vector<double>::iterator iter=CE.begin();iter!=CE.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout.close();

    std::cout<<"****************************"<<std::endl;
    std::cout<<"*       best result        *"<<std::endl;
    std::cout<<"*--------------------------*"<<std::endl;
    std::cout<<"* best pressure:"<<bestPressure<<std::endl;
    std::cout<<"* best Tmax    :"<<bestTmax<<std::endl;
    std::cout<<"* best DeltaT  :"<<bestDeltaT<<std::endl;
    std::cout<<"* best step    :"<<bestStep<<std::endl;
    std::cout<<"* best CE      :"<<bestCE<<std::endl;
    std::cout<<"****************************"<<std::endl;
}
