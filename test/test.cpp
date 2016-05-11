
#include <iostream>
#include "Channel.h"
#include "Location.h"
#include "tmap.h"
using namespace std;

int main(){
    Location location(1,1);
    Location l2=Location();
    l2=location;
    cout<<location.getRow()<<' '<<location.getCol()<<endl;
    cout<<l2.getRow()<<' '<<l2.getCol()<<endl;
    cout<<"hello"<<endl;
    Channel testChannel(5,5,"channel.dat");
    cout<<testChannel<<endl;
 /*   testChannel.fillChannel(1,1);
    cout<<testChannel<<endl;
    testChannel.fillChannel(3,0);
    cout<<testChannel<<endl;
    testChannel.fillChannel(4,0);
    cout<<testChannel<<endl;*/

    Tmap tmap(5,5,"Tmap.dat");
    cout<<"Tmax:"<<tmap.getTmax()<<" Tmin:"<<tmap.getTmin()<<endl;
    cout<<tmap;
    vector<int> cell;
    cell=testChannel.chooseCell(tmap.getTmap(),1);
    cout<<"fill:"<<cell.at(0);
    return 0;
}
