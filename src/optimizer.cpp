#include "../inc/optimizer.h"

optimizer::optimizer()
{
}

void optimizer::optimize(char* casename){
    Parser parser;
    CoolingSystem system;
    OptSpec spec;
    vector<CoolingSystem> subsys;

    struct timeval start, end;

    int totalSteps;
    double setCoolingEnergy;
    parser.get_ckt(casename, system, spec,subsys);

    std::cout<<"chip length:"<<system.chip_length()<<std::endl;
    std::cout<<"chip width:"<<system.chip_width()<<std::endl;
    std::cout<<"cell_length:"<<system.cell_length()<<std::endl;
    std::cout<<"channel num:"<<system.getChannelNum()<<std::endl;

    std::cout<<"Please input the total fill steps:"<<std::endl;
 //   std::cin>>totalSteps;
    std::cout<<"Please input the cooling energy you set:(W)"<<std::endl;

 //   cin>>setCoolingEnergy;



    std::string rawcase=string(casename);
    string caseName=rawcase.substr(0,rawcase.find_first_of('/'));

    gettimeofday( &start, NULL );

    int initialChannelIndex;
    for(size_t i=0;i<caseName.size();++i){
        if(isdigit(caseName[i])){
            initialChannelIndex=caseName[i]-'0';
            std::cout<<"Initial channel index is :"<<initialChannelIndex<<endl;
            break;
        }
    }

    //Subchip mchip(101,101);
    if(system.getChannelNum()>1){
       multilayerchip mchip(system.chip_length()/system.cell_length(),system.chip_width()/system.cell_width(),system.getChannelNum());
      mchip.config("case.ini");
      mchip.beginFilling(system);
    }
    else{
        Subchip chip(system.chip_length()/system.cell_length(),system.chip_width()/system.cell_width());
        //chip.config(caseName,totalSteps,initialChannelIndex,setCoolingEnergy);
        //int index=chip.chooseBestIO(system);
        //chip.optimizeBarrier(0.005,system);
        chip.config("case.ini");
        chip.beginFilling(system);
    }

    gettimeofday( &end, NULL );

    int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec -start.tv_usec;
    if(system.getChannelNum()>1){
        printf("average time: %d us\n", timeuse/(50+1));
    }
    else{
       printf("average time: %d us\n", timeuse/(50+1));
    }

}
