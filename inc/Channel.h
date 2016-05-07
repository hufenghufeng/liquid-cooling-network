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

  Author: Hengliang Zhu
  
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
#include "util.h"

using std::vector;
using std::stack;
using std::ostream;

/*! \class Channel
 *
 *  Channel: -1 = (fixed) tsv-region
             0  = filled silicon
             1  = channel fluid
             2  = channel inlet
             3  = channel outlet
 */
class Channel
{
public:
  /// \brief construct function
  Channel(size_t N_h, size_t N_w, const char* channel_template_file)
    : _N_h(N_h), _N_w(N_w), _array_ptr(NULL)
  {
    read_from_file(N_h, N_w, channel_template_file);
  }
  /// \brief construct function
  Channel(size_t N_h, size_t N_w)
    : _N_h(N_h), _N_w(N_w), _array_ptr(NULL)
  {
    _array_ptr = new int[_N_h*_N_w];
    memset(_array_ptr, 0, _N_h*_N_w*sizeof(int));
  }

  /// \brief deconstruct function
  Channel(const Channel& rhs)
    : _N_h(rhs._N_h), _N_w(rhs._N_w)
  {
    _array_ptr = new int[_N_h*_N_w];
    memcpy(_array_ptr, rhs._array_ptr, _N_h*_N_w*sizeof(int));
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
  friend ostream& operator<<(ostream& os, const Channel& rhs);

private:
  /// \brief number of grids 
  size_t _N_h, _N_w;
  /// \brief Channel array, fliud network template
  int *_array_ptr;
  
}; // class Channel

inline ostream& operator<<(ostream& os, const Channel& rhs)
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


#endif // _CHANNEL_H_
