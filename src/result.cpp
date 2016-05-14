#include "result.h"

void Result::initialBestChannel(size_t rowSize, size_t colSize){
    _N_h=rowSize;
    _N_w=colSize;
    bestChannel = new int[_N_h*_N_w];
    memset(bestChannel, 0, _N_h*_N_w*sizeof(int));
}

void Result::setBestChannel(size_t rowSize, size_t colSize, int *channel){
    if(rowSize*colSize!=_N_h*_N_w){
        std::cout<<"channel size mismatch!";
    }
    memcpy(bestChannel, channel, _N_h*_N_w*sizeof(int));
}
/*
ostream& Result::operator<<(ostream& os, const Result& rhs){
    for (size_t i=0;i<rhs._N_h;++i)
      {
        for (size_t j=0;j<rhs._N_w;++j){
           os<<rhs(i,j)<<" ";
           if(j==(rhs._N_w-1)){
               os<<std::endl;
           }
        }
      }
    return os;
}*/

void Result::outputResult(){
    std::ofstream fout;
    fout.open("bestChannel.dat");

    for (size_t i=0;i<_N_h;++i)
      {
        for (size_t j=0;j<_N_w;++j){
           fout<<bestChannel[i*_N_w+j];
           fout<<" ";
           if(j==(_N_w-1)){
               fout<<std::endl;
           }
        }
      }

   fout.close();

   fout.open("history.dat");
    //  add % for the convenience of reading of matlab
   fout<<"% pressure"<<std::endl;
   for(vector<float>::iterator iter=pressure.begin();iter!=pressure.end();iter++){
       fout<<*iter<<' ';
   }
   fout<<std::endl;

   fout<<"% max T"<<std::endl;
    for(vector<float>::iterator iter=maxT.begin();iter!=maxT.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout<<"% dt of top layer"<<std::endl;
    for(vector<float>::iterator iter=dTTop.begin();iter!=dTTop.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout<<"% dt of bottom layer"<<std::endl;
    for(vector<float>::iterator iter=dTBottom.begin();iter!=dTBottom.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout<<"% dt "<<std::endl;
    for(vector<float>::iterator iter=dT.begin();iter!=dT.end();iter++){
        fout<<*iter<<' ';
    }
    fout<<std::endl;

    fout.close();

    std::cout<<"****************************"<<std::endl;
    std::cout<<"*       best result        *"<<std::endl;
    std::cout<<"*--------------------------*"<<std::endl;
    std::cout<<"* best pressure:"<<bestPressure<<std::endl;
    std::cout<<"* best Tman    :"<<bestTmax<<std::endl;
    std::cout<<"* best DeltaT  :"<<bestDeltaT<<std::endl;
    std::cout<<"* best step    :"<<bestStep<<std::endl;
    std::cout<<"****************************"<<std::endl;
}
