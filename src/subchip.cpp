#include "../inc/subchip.h"
using std::fstream;

/*
%  ____________
%  | T1           |        T3        |
%  |_____|______|
%  | T2           |        T4        |
%  |_____|______|
*/
void Subchip::optimizeBarrier(double setCoolingEnergy,CoolingSystem& sys){
    double ThredT=0.5;
    tempChannel=channel;
 //   cleanResult();
 //   initialResult(setCoolingEnergy,sys);
    double bestCost1=999999.9;
    double bestCost2=999999.9;
    double bestCost3=999999.9;
    double bestCost4=999999.9;

    double bestTheta1,bestTheta2,bestTheta3,bestTheta4;
    int bestLength1,bestLength2,bestLength3,bestLength4;
    double minT1,minT2,minT3,minT4;
    double maxT1,maxT2,maxT3,maxT4;

    double beginTheta=0.1;
    double endTheta=0.4;
    int beginLength=20;
    int endLength   =40;

    dvec x0;

    for(double theta=beginTheta;theta<endTheta;theta=theta+0.1){
        for(int length=beginLength;length<endLength;length=length+10){
            tempChannel=mchannel(channel);
            tempChannel.drawFourSameLine(length,theta);
            generateTempTmap(setCoolingEnergy,sys,x0);

            std::ofstream debug_channel1;
            debug_channel1.open("beforeadjust.dat");
            debug_channel1<<channel;
            //evaluate
            maxT1=std::max(tempTopTmap.maxTPart1(minT1),tempBottomTmap.maxTPart1(minT1));
            maxT2=std::max(tempTopTmap.maxTPart2(minT2),tempBottomTmap.maxTPart2(minT2));
            maxT3=std::max(tempTopTmap.maxTPart3(minT3),tempBottomTmap.maxTPart3(minT3));
            maxT4=std::max(tempTopTmap.maxTPart4(minT4),tempBottomTmap.maxTPart4(minT4));
            cout<<"maxT1:"<<maxT1<<endl;
            if(maxT1<bestCost1){
                bestCost1=maxT1;
                bestTheta1=theta;
                bestLength1=length;
            }
            cout<<"maxT2:"<<maxT2<<endl;
            if(maxT2<bestCost2){
                bestCost2=maxT2;
                bestTheta2=theta;
                bestLength2=length;
            }
            cout<<"maxT3:"<<maxT3<<endl;
            if(maxT3<bestCost3){
                bestCost3=maxT3;
                bestTheta3=theta;
                bestLength3=length;
            }
            cout<<"maxT4:"<<maxT4<<endl;
            if(maxT4<bestCost4){
                bestCost4=maxT4;
                bestTheta4=theta;
                bestLength4=length;
            }
        }
    }

        double minMaxT=999999.9;
        double maxT=0.0,newMaxT=0.0;
        int minIndex=0;

        tempChannel=channel;
        tempChannel.drawLine(1 ,1 ,1 +bestLength1*cos(bestTheta1*PI),1+bestLength1*sin(bestTheta1*PI));
        tempChannel.drawLine(99,1 ,99-bestLength2*cos(bestTheta2*PI ),1+bestLength2*sin(bestTheta2*PI));
        tempChannel.drawLine(99,99,99-bestLength3*cos(bestTheta3*PI),99-bestLength3*sin(bestTheta3*PI));
        tempChannel.drawLine(1 ,99,1 +bestLength4*cos(bestTheta4*PI),99-bestLength4*sin(bestTheta4*PI));


        //adjust
        generateTempTmap(setCoolingEnergy,sys,x0);
        while(1){
            maxT1=std::max(tempTopTmap.maxTPart1(minT1),tempBottomTmap.maxTPart1(minT1));
            maxT2=std::max(tempTopTmap.maxTPart2(minT2),tempBottomTmap.maxTPart2(minT2));
            maxT3=std::max(tempTopTmap.maxTPart3(minT3),tempBottomTmap.maxTPart3(minT3));
            maxT4=std::max(tempTopTmap.maxTPart4(minT4),tempBottomTmap.maxTPart4(minT4));
            double Tlist[4]={maxT1,maxT2,maxT3,maxT4};
            for(int i=0;i<4;++i){
                if(Tlist[i]>maxT){
                    maxT=Tlist[i];
                }
                if(Tlist[i]<minMaxT){
                    minMaxT=Tlist[i];
                    minIndex=i+1;
                }
            }
            if(abs(maxT-minMaxT)<ThredT){
                break;
            }
            switch (minIndex){
            case 1:
                bestLength1++;
                break;
            case 2:
                bestLength2++;
                break;
            case 3:
                bestLength3++;
                break;
            case 4:
                bestLength4++;
                break;
            default:
                std::cerr<<"wrong in optimize barrier"<<std::endl;
                break;
            }
            // drawLine
            tempChannel=channel;
            tempChannel.drawLine(1 ,1 ,1 +bestLength1*cos(bestTheta1*PI),1+bestLength1*sin(bestTheta1*PI));
            tempChannel.drawLine(99,1 ,99-bestLength2*cos(bestTheta2*PI ),1+bestLength2*sin(bestTheta2*PI));
            tempChannel.drawLine(99,99,99-bestLength3*cos(bestTheta3*PI),99-bestLength3*sin(bestTheta3*PI));
            tempChannel.drawLine(1 ,99,1 +bestLength4*cos(bestTheta4*PI),99-bestLength4*sin(bestTheta4*PI));

             // evaluate
            generateTempTmap(setCoolingEnergy,sys,x0);
            newMaxT=std::max(tempTopTmap.getTmax(),tempBottomTmap.getTmax());
            if(newMaxT<maxT){
                continue;
            }
            else{
                switch (minIndex){
                case 1:
                    bestLength1--;
                    break;
                case 2:
                    bestLength2--;
                    break;
                case 3:
                    bestLength3--;
                    break;
                case 4:
                    bestLength4--;
                    break;
                default:
                    std::cerr<<"wrong in optimize barrier"<<std::endl;
                    break;
                }
                break;
            }
        }

        tempChannel=channel;
        tempChannel.drawLine(1 ,1 ,1 +bestLength1*cos(bestTheta1*PI),1+bestLength1*sin(bestTheta1*PI));
        tempChannel.drawLine(99,1 ,99-bestLength2*cos(bestTheta2*PI ),1+bestLength2*sin(bestTheta2*PI));
        tempChannel.drawLine(99,99,99-bestLength3*cos(bestTheta3*PI),99-bestLength3*sin(bestTheta3*PI));
        tempChannel.drawLine(1 ,99,1 +bestLength4*cos(bestTheta4*PI),99-bestLength4*sin(bestTheta4*PI));

        channel=tempChannel;




    cout<<"***********Optimization of four barrier*************"<<std::endl;
    cout<<"Theta1:"<<bestTheta1<<"Length1:"<<bestLength1<<endl;
    cout<<"Theta2:"<<bestTheta2<<"Length2:"<<bestLength2<<endl;
    cout<<"Theta3:"<<bestTheta3<<"Length3:"<<bestLength3<<endl;
    cout<<"Theta4:"<<bestTheta4<<"Length4:"<<bestLength4<<endl;

}

void Subchip::fillColdCell(int num){
    std::vector<int> fillIndex;

    if (topLayer.getDeltaT()>bottomLayer.getDeltaT()){
        const double* tmap=topLayer.getTmap();
        std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
        fillIndex=channel.chooseCell(topLayer.getTmap(),num);

        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
            tempChannel.newCorrect();
            channel.markCell(*iter);
            tempChannel.markCell(*iter);
            std::ofstream debug_channel2;
            debug_channel2.open("afterCorrect.dat");
            debug_channel2<<tempChannel;
            std::cout<<"fill location is :"<<*iter;
        }
    }
    else{
        fillIndex=channel.chooseCell(bottomLayer.getTmap(),num);
        const double* tmap=topLayer.getTmap();
        std::cout<<"tmap1:"<<tmap[10000]<<std::endl;
        for(vector<int>::iterator iter=fillIndex.begin();
                                  iter!=fillIndex.end();
                                  iter++){
            // todo: add index -> location
            tempChannel.fillChannel((*iter)/_N_w,(*iter)%_N_w);
            tempChannel.newCorrect();
            channel.markCell(*iter);
            tempChannel.markCell(*iter);
            std::ofstream debug_channel2;
            debug_channel2.open("afterCorrect.dat");
            debug_channel2<<tempChannel;
            std::cout<<"fill location is :"<<*iter;
        }
    }
}
/*
void Subchip::generateTempTmap(double setCoolingEnergy){
    /// how to run simulation under the same cooling energy
    double testPin=3000.0;

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

    double testCoolingEnenrgy=getCoolingEnergy();
    double setpin=0.0;
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
*/
void Subchip::generateTempTmap(double setCoolingEnergy, CoolingSystem& sys,dvec& x0){
    double Pin;

    int* tc=tempChannel.getChannel();

    for(size_t i=0;i<_N_h;++i){
        for(size_t j=0;j<_N_w;++j){
            ch.set(i,j,tc[i*_N_w+j]);
        }
    }

    std::ofstream debug_channel1;
    debug_channel1.open("beforeSysSetChannel.dat");
    debug_channel1<<ch;
    //ch=Channel(_N_h,_N_w,tempChannel.getChannel());
    sys.set_channel(0,ch);
//    sim.set(sys);
    sim.clear_P();
    sim.setup_P();

    double CE=sim.eval_P(5962);
    Pin=sim.get_Ps_at(setCoolingEnergy);
    std::cout<<"the set pressure is :"<<Pin<<std::endl;
    CE=sim.eval_P(Pin);
    std::cout<<"\n!!!Cooling Energy = "<<CE<<std::endl;

    sim.setup_Tsteady(Pin);
    dvec x1;
    vector<ddmat> Ts;
    //   x0: the initial guess of Tsteady for simulator as input
    //       the solution of Tsteady as output
    //       eval_Tsteady also return the T distribution for each layer in Ts.
    Ts = sim.eval_Tsteady(x1);
    std::cout<<"TS's size:"<<Ts.size()<<std::endl;
    tempTopTmap.updateTmap(Ts[0]);

    tempBottomTmap.updateTmap(Ts[3]);

    resultHistory.setTempPin(Pin);

//    std::ofstream foutt;
//    foutt.open("x0.dat");
//    std::ofstream fouttt;
//    fouttt.open("Ts.dat");

//    for(size_t k=0;k<5;++k){
//     for(size_t i=0;i<_N_h;++i){
//        for(size_t j=0;j<_N_w;++j){
//            foutt<<x0[k*_N_w*_N_h+i*_N_w+j]<<'\t';
//         }
//        foutt<<'\n';
//        }
//    }

//    for(size_t k=0;k<5;++k){
//     for(size_t i=0;i<_N_h;++i){
//        for(size_t j=0;j<_N_w;++j){
//            fouttt<<Ts[k](i,j)<<'\t';
//         }
//        fouttt<<'\n';
//        }
//    }
//    for(size_t k=0;k<5;++k){
//        for(size_t i=0;i<_N_h;++i){
//          for(size_t j=0;j<_N_w;++j){

//              x0[k*_N_w*_N_h+j*_N_w+i]=Ts[k](i,j);
//          }
//        }
//    }

}


void Subchip::evaluateTempTmap(int index){

    double TmaxConstraints=360.0;
    tempTopTmap.caculateTmap();
    tempBottomTmap.caculateTmap();

    double Tmax=std::max(tempTopTmap.getTmax(),tempBottomTmap.getTmax());
    double dTtop=tempTopTmap.getDeltaT();
    double dTbottom=tempBottomTmap.getDeltaT();
    double dTmax=std::max(dTtop,dTbottom);
    std::cout<<"\n My DT IS :"<<dTmax<<std::endl;
    std::cout<<"\n THE last deltaT:"<<resultHistory.getLastDeltaT()<<std::endl;
    if ((dTmax<resultHistory.getLastDeltaT()+0.08)){
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
        resultHistory.addCE(CE);

        resultHistory.setBestChannel(_N_h,_N_w,tempChannel.getChannel());
        resultHistory.setBestDeltaT(dTmax);
        resultHistory.setBestPressure(resultHistory.getTempPin());
        resultHistory.setBestTmax(Tmax);
        resultHistory.setBestStep(index);
        resultHistory.setBestCE(CE);
    }
    else{
        //refuse
        std::cout<<"rejected!"<<std::endl;
        resultHistory.copyLastResult();
    }
}

void Subchip::initialResult(double setCoolingEnergy,CoolingSystem& sys){
    //(*this).generateTempTmap(setCoolingEnergy);
    //sim.set(sys);
    dvec x0;

    generateTempTmap(setCoolingEnergy,sys,x0);

    topLayer   =tempTopTmap;
    std::cout<<"\n TempTop and top:"<<tempTopTmap(1,15)<<' '<<topLayer(1,15)<<std::endl;

    bottomLayer=tempBottomTmap;

    double IOpressure=resultHistory.getTempPin();
    double Tmax=std::max(topLayer.getTmax(),bottomLayer.getTmax());
    double topDeltaT=topLayer.getDeltaT();
    double bottomDeltaT=bottomLayer.getDeltaT();

    resultHistory.addPressure(IOpressure);
    resultHistory.addMaxT(Tmax);
    resultHistory.addDTTop(topDeltaT);
    resultHistory.addDTBottom(bottomDeltaT);
    resultHistory.addDT(std::max(topDeltaT,bottomDeltaT));
    std::cout<<"\n THE first result deltaT:"<<std::max(topDeltaT,bottomDeltaT)<<std::endl;

    resultHistory.addCE(CE);

    resultHistory.initialBestChannel(_N_h,_N_w);
    resultHistory.setBestChannel(_N_h,_N_w,channel.getChannel());

    resultHistory.setBestPressure(IOpressure);
    resultHistory.setBestTmax(Tmax);
    resultHistory.setBestDeltaT(std::max(topDeltaT,bottomDeltaT));
    resultHistory.setBestCE(CE);
    resultHistory.setBestStep(1);
}

void Subchip::cleanResult(){
    resultHistory.cleanResult();
}

void Subchip::outputBestTmap(CoolingSystem& sys){
    Channel tChannel=resultHistory.getBestChannel();
    sys.set_channel(0,tChannel);

    sim.clear_P();
    sim.setup_P();

    double Pin=sim.get_Ps_at(resultHistory.getBestCE());
    //double Pin=sim.get_Ps_at(0.002);
    CE=sim.eval_P(Pin);

    sim.setup_Tsteady(Pin);
    dvec x0;
    vector<ddmat> Ts;
    //   x0: the initial guess of Tsteady for simulator as input
    //       the solution of Tsteady as output
    //       eval_Tsteady also return the T distribution for each layer in Ts.
    Ts = sim.eval_Tsteady(x0);

    tempTopTmap.updateTmap(Ts[0]);

    tempBottomTmap.updateTmap(Ts[3]);

    std::ofstream fout;

    fout.open("topBestTmap.dat");
    fout<<tempTopTmap;
    fout.close();

    fout.open("bottomBestTmap.dat");
    fout<<tempBottomTmap;
    fout.close();

}

void Subchip::keepResult(){
    double IOpressure=resultHistory.getTempPin();
    double Tmax=std::max(topLayer.getTmax(),bottomLayer.getTmax());
    double topDeltaT=topLayer.getDeltaT();
    double bottomDeltaT=bottomLayer.getDeltaT();
    resultHistory.addPressure(IOpressure);
    resultHistory.addMaxT(Tmax);
    resultHistory.addDTTop(topDeltaT);
    resultHistory.addDTBottom(bottomDeltaT);
    resultHistory.addDT(std::max(topDeltaT,bottomDeltaT));
}

double Subchip::getCoolingEnergy(){
/*
    std::string line;

    std::ifstream ffin;

//        fin.open("/home/hf/new-ice/bin/cooling_energy.dat");



    double coolingEnergy;
    ffin.open("/home/hf/new-ice/bin/cooling_energy.dat",std::ios_base::in);

    if(!ffin.is_open()){
        std::cerr<<"open /home/hf/new-ice/bin/cooling_energy.dat ERROR!"<<std::endl;
        ffin.clear();
    }
    getline(ffin,line);
    coolingEnergy=atof(line.c_str());

    ffin.close();
*/
    return CE;
}

void Subchip::setCE(double ce){
    CE=ce;
}

double Subchip::getPin(){
    double IOpressure;
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

void Subchip::setPin(double Pin){
    int height=200;
    std::ofstream fout1;
    fout1.open("/home/hf/new-ice/bin/Pin.dat");

    fout1<<Pin<<'\t'<<height;

    fout1.close();
}

void Subchip::beginFilling(CoolingSystem& sys){
    dvec x0;

    std::ofstream debug_channel1;
    debug_channel1.open("beforeinitial.dat");
    debug_channel1<<channel;
    //config("case1",4,1);
    sim.set(sys);
    ch=Channel(_N_h,_N_w);
    CE=myconfig.getCE();

    if(myconfig.isStartFromBegin()){
        int newIndex=chooseBestIO(sys);
        myconfig.setInitialIndex(newIndex);
        initialChannel(myconfig.getInitialIndex());

        if(myconfig.needFindBarrier()){
            optimizeBarrier(CE,sys);
        }
        initialResult(myconfig.getCE(),sys);

        if(myconfig.needPlaceStraightPattern()){
            channel.fillHotArea(topLayer.getTmap(),bottomLayer.getTmap(),CE);
        }
    }
    else{
        initialChannel(myconfig.getInitialIndex());

        std::ofstream debug_channel;
        debug_channel.open("debugChannel.dat");
        debug_channel<<channel;
        //generateTempTmap(0.01,sys);
        initialResult(myconfig.getCE(),sys);
    }

    // test fill hot area
    //double searchedCE=searchCE(0.022,400.0,15.0,0.05,sys);
   // cout<<"The CE is:"<<searchedCE<<endl;
    std::ofstream fouttt;
    fouttt.open("bottomTmap");
    fouttt<<tempBottomTmap;
    //channel.fillHotArea(topLayer.getTmap(),bottomLayer.getTmap(),0.2);
    for(int i=2;i<myconfig.totalFillStep();i++){

        std::ofstream debug_channel2;
        debug_channel2.open("beforechooseCell.dat");
        debug_channel2<<tempChannel;
        // remove cell with little flow
        if(myconfig.needFillTinyFlow()){
            if(i%myconfig.getFlowCheckInterval()==myconfig.getFlowCheckInterval()-1){
                removeDeadCell(myconfig.getTinyFlowThred());
            }
        }
       // removeDeadCell(0.5);
        fillColdCell(myconfig.getFillNum());
        generateTempTmap(myconfig.getCE(),sys,x0);
        evaluateTempTmap(i);
        //keepResult();
    }
    //removeDeadCell(0.1);
//    std::ofstream foutt;
//    foutt.open("removeRIngChannel");
//    foutt<<channel;
    resultHistory.outputResult();
    outputBestTmap(sys);
}

void Subchip::initialChannel(int index){
    channel.initialChannel(index);
    tempChannel=channel;
}

void Subchip::removeDeadCell(double pThred){
    ddmat pressure=(sim.getPressure())[0];
    //remove isolated IO
    for(unsigned int i=1;i<_N_h-1;i=i+2){
        //top
        if(channel(0,i)>LIQUID&&channel(1,i)<LIQUID){
            channel.set(0,i,SOLID);
        }
        if(channel(0,i)>LIQUID&&channel(1,i)==LIQUID){
            if(abs(pressure(0,i)-pressure(1,i))<pThred){
                channel.fillChannel(0,i);
                channel.fillChannel(1,i);
            }
        }
        //bottom
        if(channel(_N_h-1,i)>LIQUID&&channel(_N_h-2,i)<LIQUID){
            channel.fillChannel(_N_h-1,i);
        }
        if(channel(_N_h-1,i)>LIQUID&&channel(_N_h-2,i)==LIQUID){
            if(abs(pressure(_N_h-1,i)-pressure(_N_h-2,i))<pThred){
                channel.fillChannel(_N_h-1,i);
                channel.fillChannel(_N_h-2,i);
            }
        }
        // west
        if(channel(i,0)>LIQUID&&channel(i,1)<LIQUID){
            channel.fillChannel(i,0);
        }
        if(channel(i,0)>LIQUID&&channel(i,1)==LIQUID){
            if(abs(pressure(i,0)-pressure(i,1))<pThred){
                channel.fillChannel(i,0);
                channel.fillChannel(i,1);
            }
        }
        //east
        if(channel(i,_N_w-1)>LIQUID&&channel(i,_N_w-2)<LIQUID){
            channel.fillChannel(i,_N_w-1);
        }
        if(channel(i,_N_w-1)>LIQUID&&channel(i,_N_w-2)==LIQUID){
            if(abs(pressure(i,_N_w-1)-pressure(i,_N_w-2))<pThred){
                channel.fillChannel(i,_N_w-1);
                channel.fillChannel(i,_N_w-2);
            }
        }
    }
    for(unsigned int i=0;i<_N_h;++i){
        for(unsigned int j=0;j<_N_w;++j){
            if(channel(i,j)!=-1){
                checkDeadInnerCell(i,j,pressure,pThred);
            }
        }
    }
}

void Subchip::checkDeadInnerCell(int i, int j, ddmat &pressure, double pThred){
    // -1 1 -1
    // 1  1 1
    // -1 1 -1
    if((i&1)&&(j&1)&&channel(i,j)>SOLID){
        //top
        if(i>0&&channel(i-1,j)>SOLID){
            if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i-1,j))<pThred)){
                channel.fillChannel(i-1,j);
                checkDeadInnerCell(i-1,j,pressure,pThred);
                pressure(i-1,j)=-1;
            }
        }
        //bottom
        if(i<_N_h-1&&channel(i+1,j)>SOLID){
            if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i+1,j))<pThred)){
                channel.fillChannel(i+1,j);
                checkDeadInnerCell(i+1,j,pressure,pThred);
                pressure(i+1,j)=-1;
            }
        }
        //west
        if(j>0&&channel(i,j-1)>SOLID){
            if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i,j-1))<pThred)){
                channel.fillChannel(i,j-1);
                checkDeadInnerCell(i,j-1,pressure,pThred);
                pressure(i,j-1)=-1;
            }
        }
        //east
        if(j<_N_w-1&&channel(i,j+1)>SOLID){
            if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i,j+1))<pThred)){
                channel.fillChannel(i,j+1);
                checkDeadInnerCell(i,j+1,pressure,pThred);
                pressure(i,j+1)=-1;
            }
        }
    }
    else{
    // -1  1  -1
            if((!(i&1))&&channel(i,j)>SOLID){
                //top
                if(i>0&&channel(i-1,j)>SOLID){
                    if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i-1,j))<pThred)){
                        channel.fillChannel(i,j);
                        checkDeadInnerCell(i-1,j,pressure,pThred);
                        pressure(i,j)=-1;
                    }
                }
                //bottom
                if(i<_N_h-1&&channel(i+1,j)>SOLID){
                    if(pressure(i,j)!=-1&&((abs(pressure(i,j)-pressure(i+1,j))<pThred))){
                        channel.fillChannel(i,j);
                        checkDeadInnerCell(i,j-1,pressure,pThred);
                        pressure(i,j)=-1;
                    }
                }
            }
            if((!(j&1))&&channel(i,j)>SOLID){
                //west
                if(j>0&&channel(i,j-1)>SOLID){
                    if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i,j-1))<pThred)){
                        channel.fillChannel(i,j);
                        checkDeadInnerCell(i,j-1,pressure,pThred);
                        pressure(i,j)=-1;
                    }
                }
                //east
                if(j<_N_w-1&&channel(i,j+1)>SOLID){
                    if(pressure(i,j)!=-1&&(abs(pressure(i,j)-pressure(i,j+1))<pThred)){
                        channel.fillChannel(i,j);
                        checkDeadInnerCell(i,j+1,pressure,pThred);
                        pressure(i,j)=-1;
                    }
                }
            }
    }
}

int Subchip::chooseBestIO(CoolingSystem& sys){
    //to do: improve here later
    int bestIndex=0;
    double bestIOCost=99999;
    double IOCost=0.0;
    double setCoolingEnergy=0.01;
    double dTtop=0.0;
    double dTbottom=0.0;

    dvec x0;

    sim.set(sys);
    ch=Channel(_N_h,_N_w);

    initialChannel(100);
    initialResult(setCoolingEnergy,sys);

    generateTempTmap(setCoolingEnergy,sys,x0);
    dTtop=tempTopTmap.getDeltaT();
    std::cout<<"dTtop:"<<dTtop<<std::endl;
    dTbottom=tempBottomTmap.getDeltaT();
    bestIOCost=std::max(dTtop,dTbottom);

    for(int i=101;i<115;++i){
        initialChannel(i);
        generateTempTmap(setCoolingEnergy,sys,x0);
        dTtop=tempTopTmap.getDeltaT();
        std::cout<<"dTtop:"<<dTtop<<std::endl;
        dTbottom=tempBottomTmap.getDeltaT();
        IOCost=std::max(dTtop,dTbottom);
        std::cout<<"IOCost:"<<IOCost<<std::endl;
        if(IOCost<bestIOCost){
            bestIOCost=IOCost;
            bestIndex=i;
        }
        keepResult();
    }
    cleanResult();
    initialChannel(bestIndex);
    return bestIndex;
}

double Subchip::searchCE(double CE,double TmaxConst, double dtConst, double Thred,CoolingSystem& sys){
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

double Subchip::binarySearchCE(double lowerBoundry, double upperBoundry, double dtConst, double TmaxConst, double Thred, CoolingSystem& sys){
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

double Subchip::getMaxDT(){
    return std::max(topLayer.getDeltaT(),bottomLayer.getDeltaT());
}

double Subchip::getMaxT(){
    return std::max(topLayer.getTmax(),bottomLayer.getTmax());
}

double Subchip::getTempMaxDT(){
     return std::max(tempTopTmap.getDeltaT(),tempBottomTmap.getDeltaT());
}

double Subchip::getTempMaxT(){
    return std::max(tempTopTmap.getTmax(),tempBottomTmap.getTmax());
}
