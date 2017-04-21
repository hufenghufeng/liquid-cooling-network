#include "../inc/multilayerchip.h"

void multilayerchip::generateTempTmap(double setCoolingEnergy, CoolingSystem& sys,dvec& x0){
    double Pin;

    for(int k=0;k<channelLayerNum;++k){
        int* tc=tempMchannelVec[k].getChannel();

        for(size_t i=0;i<_N_h;++i){
            for(size_t j=0;j<_N_w;++j){
                channelVec[k].set(i,j,tc[i*_N_w+j]);
            }
        }
    }
/*
    std::ofstream debug_channel1;
    debug_channel1.open("beforeSysSetChannel.dat");
    debug_channel1<<ch;
    */
    //ch=Channel(_N_h,_N_w,tempChannel.getChannel());
    for( int  k=0;k<channelLayerNum;++k){
        sys.set_channel(k,channelVec[k]);
    }
//    sim.set(sys);
    sim.clear_P();
    sim.setup_P();

    double CE=sim.eval_P(5962);
    Pin=sim.get_Ps_at(setCoolingEnergy);
    std::cout<<"the set pressure is :"<<Pin<<std::endl;
    CE=sim.eval_P(Pin);
    std::cout<<"\n!!!Cooling Energy = "<<CE<<std::endl;

    sim.setup_Tsteady(Pin);
    //dvec x1;
    vector<ddmat> Ts;
    //   x0: the initial guess of Tsteady for simulator as input
    //       the solution of Tsteady as output
    //       eval_Tsteady also return the T distribution for each layer in Ts.
    Ts = sim.eval_Tsteady(x0);
    std::cout<<"TS's size:"<<Ts.size()<<std::endl;
    // Tmap is one more layer than channel
    for(int k=0;k<=channelLayerNum;++k){
        tempTmapVec[k].updateTmap(Ts[3*k]);
    }

    resultHistory.setTempPin(Pin);

    for(size_t k=0;k<channelLayerNum*3+2;++k){
        for(size_t i=0;i<_N_h;++i){
          for(size_t j=0;j<_N_w;++j){
              x0[k*_N_w*_N_h+j*_N_w+i]=Ts[k](i,j);
          }
        }
    }
}

void multilayerchip::initialResult(double setCoolingEnergy,CoolingSystem& sys){
    dvec x0;
    generateTempTmap(setCoolingEnergy,sys,x0);
/*
    topLayer   =tempTopTmap;
    std::cout<<"\n TempTop and top:"<<tempTopTmap(1,15)<<' '<<topLayer(1,15)<<std::endl;

    bottomLayer=tempBottomTmap;
*/
    for(unsigned int i=0;i<tempTmapVec.size();++i){
        TmapVec[i]=tempTmapVec[i];
    }

    double IOpressure=resultHistory.getTempPin();
    double Tmax=0;
    double DTmax=0;

    for(int k=0;k<=channelLayerNum;++k){
        if(tempTmapVec[k].getTmax()>Tmax){
            Tmax=tempTmapVec[k].getTmax();
        }
        if(tempTmapVec[k].getDeltaT()>DTmax){
            DTmax=tempTmapVec[k].getDeltaT();
        }
    }

    resultHistory.addPressure(IOpressure);
    resultHistory.addMaxT(Tmax);
    resultHistory.addDT(DTmax);
    std::cout<<"\n THE first result deltaT:"<<DTmax<<std::endl;

    resultHistory.addCE(CE);

    //resultHistory.initialBestChannel(_N_h,_N_w);
    resultHistory.clearBestChannel();
    for(int k=0;k<channelLayerNum;++k){
        resultHistory.addBestChannel(mchannelVec[k].getChannel());
    }
    resultHistory.setBestPressure(IOpressure);
    resultHistory.setBestTmax(Tmax);
    resultHistory.setBestDeltaT(DTmax);
    resultHistory.setBestCE(CE);
    resultHistory.setBestStep(1);
}

void multilayerchip::cleanResult(){
    resultHistory.cleanMultiResult();
}

void multilayerchip::initialChannel(int index){
    for(unsigned int i=0;i<mchannelVec.size();++i){
        mchannelVec[i].initialChannel(index*10+i);
        tempMchannelVec[i]=mchannelVec[i];
    }
}

bool multilayerchip::initialChannelFromFile(const char* channel_template_file,unsigned int layerIndex){
    if(layerIndex>=channelLayerNum){
        std::cout<<"error initial channel layer index!"<<endl;
        return false;
    }
    mchannelVec[layerIndex].read_from_file(_N_h,_N_w,channel_template_file);
    tempMchannelVec[layerIndex]=mchannelVec[layerIndex];
    return true;
}

void multilayerchip::fillColdCell(int num){
    // find which source layer is dominant
    unsigned int dominantSourceLay;
    double tempDT=0.0;

    for(unsigned int i=0;i<TmapVec.size();++i){
        if(TmapVec[i].getDeltaT()>tempDT){
            tempDT=TmapVec[i].getDeltaT();
            dominantSourceLay=i;
        }
    }

    std::vector<int> fillIndex;
    // top layer
    if(dominantSourceLay==0){
        const double* tmap=TmapVec[dominantSourceLay].getTmap();
        std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
        fillIndex=mchannelVec[dominantSourceLay].chooseCell(TmapVec[dominantSourceLay].getTmap(),num);

        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempMchannelVec[dominantSourceLay].fillChannel((*iter)/_N_w,(*iter)%_N_w);
            tempMchannelVec[dominantSourceLay-1].newCorrect();
            mchannelVec[dominantSourceLay].markCell(*iter);
            tempMchannelVec[dominantSourceLay].markCell(*iter);
            std::cout<<"fill location is :"<<*iter<<" layer:"<<dominantSourceLay<<std::endl;
        }
    }
    else{
        // bottom layer
        if(dominantSourceLay==TmapVec.size()-1){
            const double* tmap=TmapVec[dominantSourceLay-1].getTmap();
            std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
            fillIndex=mchannelVec[dominantSourceLay-1].chooseCell(TmapVec[dominantSourceLay-1].getTmap(),num);

            for(vector<int>::iterator iter=fillIndex.begin();
                                      iter!=fillIndex.end();
                                      iter++){
                // todo: add index -> location
                tempMchannelVec[dominantSourceLay-1].fillChannel((*iter)/_N_w,(*iter)%_N_w);
                tempMchannelVec[dominantSourceLay-1].newCorrect();
                mchannelVec[dominantSourceLay-1].markCell(*iter);
                tempMchannelVec[dominantSourceLay-1].markCell(*iter);
                std::cout<<"fill location is :"<<*iter<<" layer:"<<dominantSourceLay-1<<std::endl;
            }
        }
        else{
            // middle layer
            const double* tmap=TmapVec[dominantSourceLay-1].getTmap();
            std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
            fillIndex=mchannelVec[dominantSourceLay-1].chooseCell(TmapVec[dominantSourceLay-1].getTmap(),num);

            for(vector<int>::iterator iter=fillIndex.begin();
                                      iter!=fillIndex.end();
                                      iter++){
                // todo: add index -> location
                tempMchannelVec[dominantSourceLay-1].fillChannel((*iter)/_N_w,(*iter)%_N_w);
                tempMchannelVec[dominantSourceLay-1].newCorrect();
                mchannelVec[dominantSourceLay-1].markCell(*iter);
                tempMchannelVec[dominantSourceLay-1].markCell(*iter);
                std::cout<<"fill location is :"<<*iter<<" layer:"<<dominantSourceLay-1<<std::endl;
                }
          }
        }
}

void multilayerchip::evaluateTempTmap(int index){
    // caculate
    for(unsigned int i=0;i<tempTmapVec.size();++i){
        tempTmapVec[i].caculateTmap();
    }
    double Tmax=0,dTmax=0;
    for(int k=0;k<=channelLayerNum;++k){
        if(tempTmapVec[k].getTmax()>Tmax){
            Tmax=tempTmapVec[k].getTmax();
        }
        if(tempTmapVec[k].getDeltaT()>dTmax){
            dTmax=tempTmapVec[k].getDeltaT();
        }
    }
    std::cout<<"\n My DT IS :"<<dTmax<<std::endl;
    std::cout<<"\n THE last deltaT:"<<resultHistory.getLastDeltaT()<<std::endl;
    // evaluate
    if ((dTmax<resultHistory.getLastDeltaT())){
//    if ((Tmax<TmaxConstraints)&&(dTmax<resultHistory.getLastDeltaT())){
        //accept
        std::cout<<"accept!"<<std::endl;

        for(unsigned int i=0;i<mchannelVec.size();++i){
            mchannelVec[i]=tempMchannelVec[i];
        }
        for(unsigned int i=0;i<TmapVec.size();++i){
            TmapVec[i]=tempTmapVec[i];
        }

        resultHistory.addPressure(resultHistory.getTempPin());
        resultHistory.addMaxT(Tmax);
        resultHistory.addDT(dTmax);
        resultHistory.addCE(CE);

        for(unsigned int k=0;k<mchannelVec.size();++k){
            resultHistory.setBestChannel(_N_h,_N_w,mchannelVec[k].getChannel(),k);
        }

        resultHistory.setBestDeltaT(dTmax);
        resultHistory.setBestPressure(resultHistory.getTempPin());
        resultHistory.setBestTmax(Tmax);
        resultHistory.setBestStep(index);
        resultHistory.setBestCE(CE);
    }
    else{
        //refuse
        std::cout<<"rejected!"<<std::endl;
        resultHistory.copyLastMultiResult();
    }
}

double multilayerchip::searchCE(double CE,double TmaxConst, double dtConst, double Thred,CoolingSystem& sys){
    dvec x0;

    generateTempTmap( CE,sys,x0);
    double dtNow=getTempMaxDT();
    double tmaxNow=getTempMaxT();
    double lowerBoundry,upperBoundry;

    double bestCE=CE,dtNew,stepLimit=20,step=1;
    if(abs(dtNow-dtConst)<Thred&&tmaxNow<TmaxConst) return CE;
    else{
        if(dtNow>dtConst||tmaxNow>TmaxConst){
            lowerBoundry=CE;
            while(dtNow>dtConst+Thred||tmaxNow>TmaxConst){
                lowerBoundry=CE;
                CE=CE*1.2;
                generateTempTmap( CE,sys,x0);
                dtNew=getTempMaxDT();
                if(dtNew<dtNow){
                    bestCE=CE;
                    dtNow=dtNew;
                }
                step++;
                // con't satisfy constraints
                if(step>stepLimit){
                    return bestCE;
                }
                tmaxNow=getTempMaxT();
                if(abs(dtNow-dtConst)<Thred&&tmaxNow<TmaxConst){
                    return CE;
                }
            }
            upperBoundry=CE;
        }
        else{
            upperBoundry=CE;
            while (dtNow<dtConst-Thred&&tmaxNow<TmaxConst) {
                upperBoundry=CE;
                CE=CE*0.8;
                generateTempTmap( CE,sys,x0);
                dtNow=getTempMaxDT();
                tmaxNow=getTempMaxT();
                if(abs(dtNow-dtConst)<Thred&&tmaxNow<TmaxConst){
                    return CE;
                }
            }
            lowerBoundry=CE;
        }
    }
    CE=binarySearchCE(lowerBoundry,upperBoundry,dtConst,TmaxConst,Thred,sys);
    cout<<"We find the CE! CE:"<<CE<<endl;
    return CE;
}

double multilayerchip::binarySearchCE(double lowerBoundry, double upperBoundry, double dtConst, double TmaxConst, double Thred, CoolingSystem& sys){
    double mid,dtNow,tmaxNow;
    dvec x0;

    while(1){
        mid=(lowerBoundry+upperBoundry)/2;
        generateTempTmap( mid,sys,x0);
        dtNow=getTempMaxDT();
        tmaxNow=getTempMaxT();
        if(abs(dtNow-dtConst)<Thred&&tmaxNow<TmaxConst){
            return mid;
        }
        else{
            if(dtNow>dtConst+Thred||tmaxNow>TmaxConst){
                 lowerBoundry=mid;
            }
            else{
                upperBoundry=mid;
            }
        }
        if(abs(lowerBoundry-upperBoundry)<0.00001){
            cout<<"lower==upper,binary search not found !"<<endl;
            return   lowerBoundry;
        }
    }
}

double multilayerchip::getMaxDT(){
    double tempDTmax=0.0;
    for(unsigned int i=0;i<TmapVec.size();++i){
        if(TmapVec[i].getDeltaT()>tempDTmax){
            tempDTmax=TmapVec[i].getDeltaT();
        }
    }
    return tempDTmax;
}

double multilayerchip::getMaxT(){
    double tempTmax=0.0;
    for(unsigned int i=0;i<TmapVec.size();++i){
        if(TmapVec[i].getTmax()>tempTmax){
            tempTmax=TmapVec[i].getTmax();
        }
    }
    return tempTmax;
}

double multilayerchip::getTempMaxDT(){
    double tempDTmax=0.0;
    for(unsigned int i=0;i<tempTmapVec.size();++i){
        if(tempTmapVec[i].getDeltaT()>tempDTmax){
            tempDTmax=tempTmapVec[i].getDeltaT();
        }
    }
    return tempDTmax;
}

double multilayerchip::getTempMaxT(){
    double tempTmax=0.0;
    for(unsigned int i=0;i<tempTmapVec.size();++i){
        if(tempTmapVec[i].getTmax()>tempTmax){
            tempTmax=tempTmapVec[i].getTmax();
        }
    }
    return tempTmax;
}

void multilayerchip::outputBestTmap(CoolingSystem& sys){
    // get best channel from result history
    for( int  k=0;k<channelLayerNum;++k){
        sys.set_channel(k,resultHistory.getBestChannel(k));
    }
    sim.clear_P();
    sim.setup_P();

    double Pin=sim.get_Ps_at(resultHistory.getBestCE());
    CE=sim.eval_P(Pin);

    sim.setup_Tsteady(Pin);
    dvec x0;
    vector<ddmat> Ts;
    //   x0: the initial guess of Tsteady for simulator as input
    //       the solution of Tsteady as output
    //       eval_Tsteady also return the T distribution for each layer in Ts.
    Ts = sim.eval_Tsteady(x0);
    for(int k=0;k<=channelLayerNum;++k){
        tempTmapVec[k].updateTmap(Ts[3*k]);
    }

    std::ofstream fout;
    std::stringstream ss;

    for(unsigned int k=0;k<tempTmapVec.size();++k){
        string fileName("bestTmap");
        string fileIndex;
        ss<<k;
        fileIndex=ss.str();
        ss.str("");
        fileName=fileName+fileIndex;
        fout.open(fileName.c_str());
        fout<<tempTmapVec[k];
        fout.close();
    }
}

void multilayerchip::beginFilling(CoolingSystem& sys){

    dvec x0;
    sim.set(sys);
//    ch=Channel(_N_h,_N_w);

    initialChannel(myconfig.getInitialIndex());
   // initialChannelFromFile("channel5.dat",0);
   // initialChannelFromFile("channel5.dat",1);
    CE=myconfig.getCE();
    std::ofstream debug_channel1;
    debug_channel1.open("beforeinitial.dat");
    debug_channel1<<mchannelVec[0];

    //optimizeBarrier(0.002,sys);
    std::ofstream debug_channel;
    debug_channel.open("debugChannel.dat");
    debug_channel<<mchannelVec[0];
//    generateTempTmap(0.02,sys);
    initialResult(CE,sys);

    // test fill hot area
    //double searchedCE=searchCE(0.022,400.0,15.0,0.1,sys);
    //cout<<"The CE is:"<<searchedCE<<endl;
    std::ofstream fouttt;
    fouttt.open("topTmap");
    fouttt<<tempTmapVec[0];
    //channel.fillHotArea(topLayer.getTmap(),bottomLayer.getTmap(),0.2);
    for(int i=2;i<myconfig.totalFillStep();i++){

        std::ofstream debug_channel2;
        debug_channel2.open("beforechooseCell.dat");
        debug_channel2<<tempMchannelVec[0];
       // removeDeadCell(0.5);
        fillColdCell(1);
        generateTempTmap(0.01,sys,x0);
        evaluateTempTmap(i);
        //keepResult();
    }
    //removeDeadCell(0.1);
    std::ofstream foutt;
    foutt.open("removeRIngChannel");
    foutt<<mchannelVec[0];
    resultHistory.outputMultiResult();
    outputBestTmap(sys);
}


