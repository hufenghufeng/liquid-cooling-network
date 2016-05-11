#ifndef LOCATION_H
#define LOCATION_H
class Location{
public:
    Location();
    Location(int row,int col);
    Location(const Location& rhs);
    Location operator=(const Location& rhs);

    void setRow(int r){
        row=r;
    }

    void setCol(int c){
        col=c;
    }

    int getRow(){
        return row;
    }

    int getCol(){
        return col;
    }

private:
    int row;
    int col;
};

#endif // LOCATION_H
