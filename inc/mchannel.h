/*-*-c++-*-*/
/***************************************************************************
 **                                                                       **
 **                        Copyright (c) 2015                             **
 **              Lab. CAD, Fudan University, Shanghai, China.             ** 
 **                        All rights reserved.                           **
 **                                                                       **
 ***************************************************************************
  ==========================================================================
  ============================= File Info ==================================

  Author: Feng Hu
  
  Description:

  Resources:

  See also:

  const static char cvsid[] = "";

  ==========================================================================
*/

#ifndef _MCHANNEL_H_
#define _MCHANNEL_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <vector>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include "util.h"

using std::vector;
using std::stack;
using std::ostream;
using std::sort;
using std::istream;
/*! \class
 *
 *  : -1 = (fixed) tsv-region
             0  = filled silicon
             1  = mchannel fluid
             2  = mchannel inlet
             3  = mchannel outlet
 */
const double PI=3.1415926;
const int INPUT=2;
const int OUTPUT=3;
const int TSV=-1;
const int SOLID=0;
const int LIQUID=1;

typedef struct {
    double temperature;
    int cell;
    int mark;
    int index;
}node;

class Location{
public:
    Location(){
        row=-1;
        col=-1;
    }
    Location(int r,int c){
        row=r;
        col=c;
    }
    Location(const Location& rhs){
        row=rhs.row;
        col=rhs.col;
    }
    Location& operator=(const Location& rhs){
        row=rhs.row;
        col=rhs.col;

        return (*this);
    }

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

#define CHANNELSIZE 101
#define LINE_SIZE   1024

#define SOLID       0
#define CHANNEL     1
#define INLET    2
#define OUTLET   3

#define MARKEDCHANNEL     11
#define MARKEDINLET    12
#define MARKEDOUTLET   13

#define REMARKEDCHANNEL     21
#define REMARKEDINLET    22
#define REMARKEDOUTLET   23

//#define PRINTMATRIX 0
//#define PRINTLOCATION 0

typedef int** channelT;
typedef int rowT;
typedef int colT;

typedef struct{
    int row;
    int column;
}locationT;

typedef struct{
    int row_size;
    int column_size;
}sizeT;

typedef enum{
    InletUntip,
    InletTiped,
    OutletUntip,
    OutletTiped
}IOkindT;



class mchannel
{
public:
  mchannel(){
      _array_ptr = new int[101*101];
      memset(_array_ptr, 0, 101*101*sizeof(int));

      _array_mark = new int[101*101];
      memcpy(_array_mark, _array_ptr, 101*101*sizeof(int));
  }
  /// \brief construct function
  mchannel(size_t N_h, size_t N_w, const char* channel_template_file)
    : _N_h(N_h), _N_w(N_w), _array_ptr(NULL),_array_mark(NULL)
  {
    read_from_file(N_h, N_w, channel_template_file);
    _array_mark = new int[101*101];
    memcpy(_array_mark, _array_ptr, 101*101*sizeof(int));
  }
  /// \brief construct function
  mchannel(size_t N_h, size_t N_w)
  {
      _N_h=N_h;
      _N_w=N_w;
    _array_ptr = new int[_N_h*_N_w];
    memset(_array_ptr, 0.0, _N_h*_N_w*sizeof(int));

    _array_mark = new int[_N_h*_N_w];
    memcpy(_array_mark, _array_ptr, _N_h*_N_w*sizeof(int));
/*
    std::ofstream debug_channel1;
    debug_channel1.open("afterChannel.dat");
    debug_channel1<<(*this);
    */
  }

  mchannel operator =(const mchannel& rhs){
        _N_h=rhs._N_h;
        _N_w=rhs._N_w;
//        memcpy(_array_ptr, rhs._array_ptr, _N_h*_N_w*sizeof(int));
//        memcpy(_array_mark, _array_ptr, _N_h*_N_w*sizeof(int));
        for (size_t i=0;i<_N_h*_N_w;i++){
            _array_ptr[i]=rhs._array_ptr[i];
            _array_mark[i]=rhs._array_mark[i];
        }
        return (*this);
  }

  /// \brief deconstruct function
  mchannel(const mchannel& rhs)
    : _N_h(rhs._N_h), _N_w(rhs._N_w)
  {
    _array_ptr = new int[_N_h*_N_w];
    memcpy(_array_ptr, rhs._array_ptr, _N_h*_N_w*sizeof(int));

    _array_mark = new int[_N_h*_N_w];
    memcpy(_array_mark, rhs._array_mark, _N_h*_N_w*sizeof(int));
  }

  /// \brief deconstruct function
  ~mchannel()
  {
    clear();
  }

  /// \brief clear
  void clear() {
//    _N_h = 0;
//    _N_w = 0;
    if (_array_ptr!=NULL)
      {
	delete [] _array_ptr;
	_array_ptr = NULL;
      }

    if (_array_mark!=NULL)
      {
    delete [] _array_mark;
    _array_mark = NULL;
      }
  }
  /// \brief get channel pattern from file
  void read_from_file(size_t N_h, size_t N_w, const char* channel_template_file);
  
  /// \brief array size
  size_t Nh() const {
    return _N_h; 
  }
  /// \brief array size
  size_t Nw() const {
    return _N_w; 
  }

  /// \brief get array number
  int operator()(size_t i, size_t j) const {
    assert((i<_N_h)&&(j<_N_w));
    return _array_ptr[i*_N_w+j];
  }
  /// \brief get array number
  int& operator()(size_t i, size_t j) {
    assert((i<_N_h)&&(j<_N_w));
    return _array_ptr[i*_N_w+j];
  }
  /// \brief neighbor
  size_t neighbor_index(size_t k, size_t ind) const {
    assert(k<_N_h*_N_w);
    assert(ind<4);
    size_t i = k / _N_w;
    size_t j = k % _N_w;
    switch (ind)
      {
         case 0:
        if (i==0)
          return _N_h*_N_w+1;
        else
          return (i-1)*_N_w+j;
        break;
        case 1:
        if (i==_N_h-1)
          return _N_h*_N_w+1;
        else
          return (i+1)*_N_w+j;
        break;
        case 2:
        if (j==0)
          return _N_h*_N_w+1;
        else
          return i*_N_w+j-1;
        break;
        case 3:
        if (j==_N_w-1)
          return _N_h*_N_w+1;
        else
          return i*_N_w+j+1;
        break;
        break;
        default:
        std::cout<<"Error neighbor index!"<<std::endl;
        exit(-1);
      }
    
  }
  /// \brief set array
  void set(size_t i, size_t j, int v) {
    assert((i<_N_h)&&(j<_N_w));
    assert(_array_ptr[i*_N_w+j]!=-1);
    _array_ptr[i*_N_w+j]=v;
  }    
  
  /// \brief correct the channel
  void correct();

  /// \brief new correct
  void newCorrect();

  /// \brief output
  friend ostream& operator<(ostream& os, const mchannel& rhs);

  /// \brief output to file, these two function can be combined, do it later
  friend ostream& operator<<(ostream& os, const mchannel& rhs);

  /// \brief get the total number of adjancent "1" "2" "3" cell number
  int adjancentLiquidCellNumber(size_t row,size_t col);

  /// \brief if one liquid cell has only one adjancent liquid cell, return the location of the adjancent cell
  Location adjancentLiquidCellLocation(size_t row, size_t col);

  /// \brief check connect by recursive
  void checkConnect(size_t row, size_t col);

  /// \brief fill channel cell
  void fillChannel(size_t row,size_t col);

  /// \brief fill channel cell near solid TSV
  void fillSolid(size_t row, size_t col);

  /// \brief choose and mark cell
  vector<int> chooseCell(const double *Tmap, int num);

  /// \brief fill hot area with straight line
  void fillHotArea(const double *Tmap1, const double *Tmap2, double percent);
  /*
  %   pre-locate straght channel in output area
  %
  %   Detect the output automatically
  %
  %   ____________
  %  | T1  |  T3  |
  %  |_____|______|
  %  | T2  |  T4  |
  %  |_____|______|
  %
  % for example
  % part4:
  %   pattern 1:
  %   |
  %   | |
  %   | | |
  %   | | | |
  %   pattern 2:
  %   ________
  %     ______
  %       ____
  %         __
  %
  */
  void fillOutputArea(int size=26);
  ///brief fill center hot area
  /*
  %fill the middle area as the following show:
   %   2  2   2  2
  % 3       0      3
  % 3     0 0 0    3
  %    0 0 0 0 0 0
  % 2     0 0 0    3
  % 2       0      3
  %    3  3   2  2
  */
  void fillMidArea(int size);
  /// \brief after choose cell, you must mark this cell
  void markCell(int index);

  /// \brief return channel
  int* getChannel(){
      return _array_ptr;
  }

  /// \brief initial Channel and mark
  void initialChannel(int index);

    /// \brief draw line in channel
    void drawLine(int x1, int y1, int x2, int y2);

    void drawFourSameLine(int length,double theta);


    /*
     *channel :The channel to fill
     *size    :The size of the channel
     *
     */

     channelT ChooseIOAndCheckConnect(channelT channel,sizeT size);

    /*
     * channel :The channel to fill
     *(row,col):the beginning of recursive
     * size    :The size of the channel
     *
     *this is part of AndCheckConnect
     *make tip of neighbor block recursively,if the block is 1,2 or 3
     *then change them to 11,12,13
     */

     void MakeTips  (channelT channel,rowT row,colT col,sizeT size);

    /*
     * channel :The channel to fill
     *(row,col):the beginning of recursive
     * size    :The size of the channel
     *
     *this is part of AndCheckConnect
     *make tip of neighbor block recursively,if the block is 11,12 or 13
     *then change them to 21,22,23
     */

     void ReMakeTips(channelT channel,rowT row,colT col,sizeT size);

    /*
     *
     * recover marked block and fill unmarked block
     *
     *
     */

     channelT RecoverTips(channelT channel,sizeT size);

    /*
     *function:  IOlocationT
     *return the one location of the IO you want
     *return (0,0): there is no the IO you want
     */

     locationT IOExamine(channelT channel, IOkindT IOkind, sizeT size);

    /*
     * function : GetMatrixSize
     * return the size of Matrix of channel
     * row   : the row    number
     * column: the column number
     */

     sizeT  GetMatrixSize    (FILE* fp);

    /*
     * function:
     * read channel matrix,allocate memory and return pointer pointing to it
     * you need provide FILE pointer and close by yourself
     */

     channelT ReadChannelMatrix(FILE* fp, int RowNum,int ColNum);

    /*
     * fill the channel that has been tiped
     *
     */

     void FillTips(channelT channel,sizeT size);

    /*
     *
     * get the tiped channel and output to a file
     *
     */
     void PrintMaxtrix(channelT channel,sizeT size);

private:
  /// \brief number of grids 
  size_t _N_h, _N_w;
  /// \brief Channel array, fliud network template
  int *_array_ptr;
  /// \brief channel mark array
  int *_array_mark;
  
}; // class Channel

inline ostream& operator<(ostream& os, const mchannel& rhs)
{
  os<<"Channel: "<<rhs.Nh()<<" * "<<rhs.Nw()<<std::endl;
  os<<" [ ";
  for (size_t i=0;i<rhs.Nh();++i)
    {
      for (size_t j=0;j<rhs.Nw();++j)
	os<<rhs(i,j)<<" ";
      if (i+1<rhs.Nh())
	os<<"; ";// <<std::endl;
      else os<<"] "<<std::endl;
    }
  return os;
}

inline ostream& operator<<(ostream& os, const mchannel& rhs)
{
  for (size_t i=0;i<rhs.Nh();++i)
    {
      for (size_t j=0;j<rhs.Nw();++j){
         os<<rhs(i,j)<<" ";
         if(j==(rhs.Nw()-1)){
             os<<std::endl;
         }
      }
    }
  return os;
}

/// \brief compare function
bool compare(node a, node b);//{}
#endif // _MCHANNEL_H_
