#ifndef TMAP_H
#define TMAP_H

//#include <iostream>
#include <ostream>
//#include <ifstream>
//#include <ofstream>
#include <fstream>
//#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Simulator.h"
#include "util.h"

using std::string;
using std::ostream;
using std::fstream;
//using std::ifstream;
//using std::sstream;
//using namespace std;

class Tmap
{
public:
    Tmap(){
        _array_temperature = new double[101*101];
        memset(_array_temperature, 0, 101*101*sizeof(double));
        Tmax=0.0;
        Tmin=0.0;
        deltaT=0.0;
//        caculateTmap();
    }

    /// \brief construct function
    Tmap(size_t N_h, size_t N_w, const char* Tmap_template_file)
      : _N_h(N_h), _N_w(N_w), _array_temperature(NULL)
    {
      read_from_file(N_h, N_w, Tmap_template_file);
      caculateTmap();
    }
    /// \brief construct function from ddmat
    Tmap(size_t N_h, size_t N_w,ddmat Ts)
        : _N_h(N_h), _N_w(N_w), _array_temperature(NULL)
    {
        _array_temperature = new double[_N_h*_N_w];
        for(size_t i=0;i<N_h;++i){
            for(size_t j=0;j<N_w;++j){
                _array_temperature[i*N_w+j]=Ts(i,j);
            }
        }
        Tmax=0.0;
        Tmin=0.0;
        deltaT=0.0;
        caculateTmap();
    }

    /// \brief construct function
    Tmap(size_t N_h, size_t N_w)
      : _N_h(N_h), _N_w(N_w), _array_temperature(NULL)
    {
      _array_temperature = new double[_N_h*_N_w];
      memset(_array_temperature, 0, _N_h*_N_w*sizeof(double));
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
      _array_temperature = new double[_N_h*_N_w];
      memcpy(_array_temperature, rhs._array_temperature, _N_h*_N_w*sizeof(double));
      Tmax=rhs.getTmax();
      Tmin=rhs.getTmin();
      deltaT=Tmax-Tmin;
    }

    Tmap operator =(const Tmap& rhs){
        _N_h=rhs._N_h;
        _N_w=rhs._N_w;
        Tmax=rhs.Tmax;
        Tmin=rhs.Tmin;
        deltaT=rhs.deltaT;
//        memcpy(_array_temperature, rhs._array_temperature, _N_h*_N_w*sizeof(double));
        for(size_t i=0;i<_N_h*_N_w;i++){
            _array_temperature[i]=rhs._array_temperature[i];
        }
        return (*this);
    }

    /// \brief deconstruct function
    ~Tmap()
    {
      clear();
    }

    /// \brief clear
    void clear() {
//      _N_h = 0;
//      _N_w = 0;
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

    const double* getTmap(){
        return _array_temperature;
    }

    double getTmax() const{
        return Tmax;
    }

    double getTmin() const{
        return Tmin;
    }

    double getDeltaT() const{
        return deltaT;
    }

    /// \brief get temperaturn in (i,j)
    double operator()(size_t i, size_t j) const {
      assert((i<_N_h)&&(j<_N_w));
      return _array_temperature[i*_N_w+j];
    }

    /// \brief get Tmin, Tmax, and deltaT
    void caculateTmap();

    /// \brief update Tmap
//    void updateTmap(const char *Tmap_template_file);

    /// \brief update Tmap from ddmat
    void updateTmap(ddmat ts);

    /// \brief turn over matrix
    void turnOverMatrix();
    /*
    %  ____________
    %  | T1           |        T3        |
    %  |_____|______|
    %  | T2           |        T4        |
    %  |_____|______|
    */
    /// \brief Max T in part 1
    double maxTPart1(double& partMinT);
    /// \brief Max T in part 2
    double maxTPart2(double& partMinT);
    /// \brief Max T in part 3
    double maxTPart3(double& partMinT);
    /// \brief Max T in part 4
    double maxTPart4(double& partMinT);

    /// \brief output
    friend ostream& operator<<(ostream& os, const Tmap& rhs);

private:
    /// \brief number of grids
    size_t _N_h, _N_w;
    /// \brief Tmap array, fliud network template
    double *_array_temperature;
    /// \brief Tmax
    double Tmax;
    /// \brief Tmin
    double Tmin;
    /// \brief Tmax-Tmin
    double deltaT;
};
/*
inline ostream& operator<<(ostream& os, const Tmap& rhs)
{
  os<<"%Tmap: "<<rhs.getHeightSize()<<" * "<<rhs.getWidthSize()<<std::endl;
  os<<"% [ ";<<std::endl;
  for (size_t i=0;i<rhs.getHeightSize();++i)
    {
      for (size_t j=0;j<rhs.getWidthSize();++j)
    os<<rhs(i,j)<<" ";
      if (i+1<rhs.getHeightSize())
    os<<std::endl;
      else os<<"%] "<<std::endl;
    }
  return os;
}*/

inline ostream& operator<<(ostream& os, const Tmap& rhs)
{
  for (size_t i=0;i<rhs.getHeightSize();++i)
    {
      for (size_t j=0;j<rhs.getWidthSize();++j){
         os<<rhs(i,j)<<" ";
         if(j==(rhs.getWidthSize()-1)){
             os<<std::endl;
         }
      }
    }
  return os;
}
#endif // TMAP_H
