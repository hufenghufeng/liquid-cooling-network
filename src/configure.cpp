#include "configure.h"

configure::configure()
{
}

configure::configure(std::string casename, int steps, int channelIndex){
    totalStep=steps;
    fillNum=1;
    TmaxConstraint=365.0;
    caseName=casename;
    initialIndex=channelIndex;
}
