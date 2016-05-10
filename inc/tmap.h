#ifndef TMAP_H
#define TMAP_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"

using std::string;
using std::ostream;
using std::fstream;

class Tmap
{
public:
    Tmap();
    /// \brief construct function
    Tmap(size_t N_h, size_t N_w, const char* Tmap_template_file)
      : _N_h(N_h), _N_w(N_w), _array_temperature(NULL)
    {
      read_from_file(N_h, N_w, Tmap_template_file);
      caculateTmap();
    }
    /// \brief construct function
    Tmap(size_t N_h, size_t N_w)
      : _N_h(N_h), _N_w(N_w), _array_temperature(NULL)
    {
      _array_temperature = new float[_N_h*_N_w];
      memset(_array_temperature, 0, _N_h*_N_w*sizeof(float));
      Tmax=0.0;
      Tmin=0.0;
      deltaT=0.0;
      caculateTmap();
    }

    /// \brief deconstruct function
    Tmap(const Tmap& rhs)
//      : _N_h(rhs._N_h), _N_w(rhs._N_w)
    {
      _N_h=rhs.getHeightSize();
      _N_w=rhs.getWidthSize();
      _array_temperature = new float[_N_h*_N_w];
      memcpy(_array_temperature, rhs._array_temperature, _N_h*_N_w*sizeof(float));
      Tmax=rhs.getTmax();
      Tmin=rhs.getTmin();
      deltaT=Tmax-Tmin;
    }

    /// \brief deconstruct function
    ~Tmap()
    {
      clear();
    }

    /// \brief clear
    void clear() {
      _N_h = 0;
      _N_w = 0;
      if (_array_temperature!=NULL)
        {
      delete [] _array_temperature;
      _array_temperature = NULL;
        }
      Tmax=0.0;
      Tmin=0.0;
      deltaT=0.0;
    }
    /// \brief get Tmap pattern from file
    void read_from_file(size_t N_h, size_t N_w, const char* Tmap_template_file);

    /// \brief Tmap size
    size_t getHeightSize() const{
        return _N_h;
    }

    /// brief Tmap size of width
    size_t getWidthSize() const{
        return _N_w;
    }

    const float* getTmap(){
        return _array_temperature;
    }

    float getTmax() const{
        return Tmax;
    }

    float getTmin() const{
        return Tmin;
    }

    float getDeltaT() const{
        return deltaT;
    }

    /// \brief get temperaturn in (i,j)
    float operator()(size_t i, size_t j) const {
      assert((i<_N_h)&&(j<_N_w));
      return _array_temperature[i*_N_w+j];
    }

    /// \brief get Tmin, Tmax, and deltaT
    void caculateTmap();

    /// \brief turn over matrix
    void turnOverMatrix();

    /// \brief output
    friend ostream& operator<<(ostream& os, const Tmap& rhs);

private:
    /// \brief number of grids
    size_t _N_h, _N_w;
    /// \brief Tmap array, fliud network template
    float *_array_temperature;
    /// \brief Tmax
    float Tmax;
    /// \brief Tmin
    float Tmin;
    /// \brief Tmax-Tmin
    float deltaT;
};

inline ostream& operator<<(ostream& os, const Tmap& rhs)
{
  os<<"Tmap: "<<rhs.getHeightSize()<<" * "<<rhs.getWidthSize()<<std::endl;
  os<<" [ ";
  for (size_t i=0;i<rhs.getHeightSize();++i)
    {
      for (size_t j=0;j<rhs.getWidthSize();++j)
    os<<rhs(i,j)<<" ";
      if (i+1<rhs.getHeightSize())
    os<<std::endl;
      else os<<"] "<<std::endl;
    }
  return os;
}

#endif // TMAP_H
