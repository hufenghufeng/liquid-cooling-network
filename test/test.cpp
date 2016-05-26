
#include <iostream>
#include "Channel.h"
#include "Location.h"
#include "subchip.h"
#include "tmap.h"
#include "util.h"
using namespace std;

int main(){
    Location location(1,1);
    Location l2=Location();
    l2=location;
    cout<<location.getRow()<<' '<<location.getCol()<<endl;
    cout<<l2.getRow()<<' '<<l2.getCol()<<endl;
    cout<<"hello"<<endl;
 //   Channel testChannel(5,5,"channel.dat");
 //   cout<<testChannel<<endl;
//
//    ofstream fout;

//    fout.open("testChannelOutput.txt");
//    fout<<testChannel<<endl;
 /*   testChannel.fillChannel(1,1);
    cout<<testChannel<<endl;
    testChannel.fillChannel(3,0);
    cout<<testChannel<<endl;
    testChannel.fillChannel(4,0);
    cout<<testChannel<<endl;*/

/*    Tmap tmap(5,5,"Tmap.dat");
    cout<<"Tmax:"<<tmap.getTmax()<<" Tmin:"<<tmap.getTmin()<<endl;
    cout<<tmap;
    vector<int> cell;
    cell=testChannel.chooseCell(tmap.getTmap(),1);
    cout<<"fill:"<<cell.at(0);*/


 //   std::ifstream fin;
//    std::string line;
//    float coolingEnergy;
    //fin.open("/home/hf/new-ice/bin/cooling_energy.dat");

//    fin.open("/home/hf/new-ice/bin/cooling_energy.dat");
//    if(!fin.is_open()){
//        std::cerr<<"open cooling energy file ERROR!"<<std::endl;
 //   }
 //   fin.close();
 //   getline(fin,line);
 //   coolingEnergy=atof(line.c_str());

 //   std::cout<<coolingEnergy;

    Subchip chip(101,101,"/home/hf/new-ice/bin/output1.txt","/home/hf/new-ice/bin/output2.txt","/home/hf/new-ice/bin/case1/test_case_01.stk");
    std::ofstream debug_channel1;
    debug_channel1.open("afterSubchip.dat");
    debug_channel1<<chip.getTempChannel();
//    string a("3e+02");
//    std::cout<<std::endl;
//    std::cout<<"a:"<<atof(a.c_str());

    chip.beginFilling();

    return 0;
}
