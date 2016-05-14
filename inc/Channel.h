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

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

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
#include "util.h"
#include "Location.h"

using std::vector;
using std::stack;
using std::ostream;
using std::sort;
using std::istream;
/*! \class Channel
 *
 *  Channel: -1 = (fixed) tsv-region
             0  = filled silicon
             1  = channel fluid
             2  = channel inlet
             3  = channel outlet
 */

typedef struct {
    float temperature;
    int cell;
    int mark;
    int index;
}node;

class Channel
{
public:
  Channel(){}
  /// \brief construct function
  Channel(size_t N_h, size_t N_w, const char* channel_template_file)
    : _N_h(N_h), _N_w(N_w), _array_ptr(NULL),_array_mark(NULL)
  {
    read_from_file(N_h, N_w, channel_template_file);
  }
  /// \brief construct function
  Channel(size_t N_h, size_t N_w)
    : _N_h(N_h), _N_w(N_w), _array_ptr(NULL),_array_mark(NULL)
  {
    _array_ptr = new int[_N_h*_N_w];
    memset(_array_ptr, 0, _N_h*_N_w*sizeof(int));

    _array_mark = new int[_N_h*_N_w];
    memcpy(_array_mark, _array_ptr, _N_h*_N_w*sizeof(int));
  }

  /// \brief deconstruct function
  Channel(const Channel& rhs)
    : _N_h(rhs._N_h), _N_w(rhs._N_w)
  {
    _array_ptr = new int[_N_h*_N_w];
    memcpy(_array_ptr, rhs._array_ptr, _N_h*_N_w*sizeof(int));

    _array_mark = new int[_N_h*_N_w];
    memcpy(_array_mark, rhs._array_mark, _N_h*_N_w*sizeof(int));
  }

  /// \brief deconstruct function
  ~Channel()
  {
    clear();
  }

  /// \brief clear
  void clear() {
    _N_h = 0;
    _N_w = 0;
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

  /// \brief output
  friend ostream& operator<(ostream& os, const Channel& rhs);

  /// \brief output to file, these two function can be combined, do it later
  friend ostream& operator<<(ostream& os, const Channel& rhs);

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
  vector<int> chooseCell(const float *Tmap, int num);
  
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

private:
  /// \brief number of grids 
  size_t _N_h, _N_w;
  /// \brief Channel array, fliud network template
  int *_array_ptr;
  /// \brief channel mark array
  int *_array_mark;
  
}; // class Channel

inline ostream& operator<(ostream& os, const Channel& rhs)
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

inline ostream& operator<<(ostream& os, const Channel& rhs)
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
#endif // _CHANNEL_H_
