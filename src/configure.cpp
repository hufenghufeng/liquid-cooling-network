#include "../inc/configure.h"

configure::configure()
{
}

configure::configure(std::string casename, int steps, int channelIndex, double ce){
    totalStep=steps;
    fillNum=1;
    TmaxConstraint=365.0;
    caseName=casename;
    initialIndex=channelIndex;
    CE=ce;
    startFromBegin=false;
}

configure::configure( const char* ini_name){
    dictionary  *   ini ;
    /* Some temporary variables to hold query results */
    const char  *   s ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
    }
    iniparser_dump(ini, stderr);

    totalStep = iniparser_getint(ini, ":step", -1);
    printf("totalStep:      [%d]\n", totalStep);

    fillNum = iniparser_getint(ini, ":fillNum", -1);
    printf("fillNum:      [%d]\n", fillNum);

    s = iniparser_getstring(ini, ":caseName", NULL);
    printf("caseName:     [%s]\n", s ? s : "UNDEF");
    caseName=std::string(s);

    initialIndex = iniparser_getint(ini, ":initialIndex", -1);
    if(initialIndex<=0){
        for(size_t i=0;i<caseName.size();++i){
            if(isdigit(caseName[i])){
                initialIndex=caseName[i]-'0';
                break;
            }
        }
    }
    printf("initialIndex:      [%d]\n", initialIndex);

    CE = iniparser_getdouble(ini, ":CE", -1.0);
    printf("CE:   [%g]\n", CE);

    TmaxConstraint=365.0;

    startFromBegin = iniparser_getboolean(ini, ":startFromBegin", 0);
    printf("startFromBegin: [%d]\n", startFromBegin);

    changeCE = iniparser_getboolean(ini, ":changeCE", 0);
    printf("changeCE: [%d]\n", changeCE);

    findIO = iniparser_getboolean(ini, ":findIO", 0);
    printf("findIO: [%d]\n", findIO);

    placeStraightPattern = iniparser_getboolean(ini, ":placeStraightPattern", 0);
    printf("placeStraightPattern: [%d]\n", placeStraightPattern);

    startFromBegin = iniparser_getboolean(ini, ":startFromBegin", 0);
    printf("startFromBegin: [%d]\n", startFromBegin);

    fillTinyFlowCell = iniparser_getboolean(ini, "tinyFlow:fillTinyFlowCell", 0);
    printf("fillTinyFlowCell: [%d]\n", fillTinyFlowCell);

    pthreld = iniparser_getdouble(ini, "tinyFlow:pthreld", -1.0);
    printf("pthreld:   [%g]\n", pthreld);

    tinyFlowCheckInterval = iniparser_getint(ini, "tinyFlow:tinyFlowCheckInterval", -1);
    printf("tinyFlowCheckInterval:      [%d]\n", tinyFlowCheckInterval);

    iniparser_freedict(ini);
}
