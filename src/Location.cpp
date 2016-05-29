#include "Location.h"

Location::Location(){
    row=-1;
    col=-1;
}

Location::Location(int r, int c){
    row=r;
    col=c;
}

Location::Location(const Location &rhs){
    row=rhs.row;
    col=rhs.col;
}

Location& Location::operator=(const Location& rhs){
    row=rhs.row;
    col=rhs.col;

    return (*this);
}

