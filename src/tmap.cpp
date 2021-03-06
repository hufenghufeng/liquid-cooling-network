#include "../inc/tmap.h"

 /*Tmap::Tmap(){
    _N_h=0;
    _N_w=0;
    _array_temperature=NULL;
    Tmax=0.0;
    Tmin=0.0;
    deltaT=0.0;
}*/

/*
%  ____________
%  | T1           |        T3        |
%  |_____|______|
%  | T2           |        T4        |
%  |_____|______|
*/

void Tmap::read_from_file(size_t N_h, size_t N_w,
                 const char* Tmap_template_file)
{
  clear();
  std::string line;
  std::string delims(" \n\t\r");
  _N_h = N_h;
  _N_w = N_w;

  _array_temperature = new double[_N_h*_N_w];
  memset(_array_temperature, 0, _N_h*_N_w*sizeof(double));
  // read from file.
  // todo !!!!!!!!!!!!!!!!

  std::ifstream Tmap_file;
  Tmap_file.open(Tmap_template_file);
  for (size_t i = 0; i < _N_h; i++) {
    getline(Tmap_file, line);
    // remove comment, need to be more roburst
    if (line[0]=='%'){
      getline(Tmap_file, line);
    }

    for (size_t j = 0; j < _N_w; j++) {
      _array_temperature[i*_N_w + j] = atof(tokenizer(line, delims).c_str());
    }
  }
  //show
  //for (double i = 0; i < _N_h; i++) {
  //	  for (double j = 0; j < _N_w; j++) {
  //		  std::cout << _array_temperature[i*_N_w + j] << " ";
  //	  }
  //	  std::cout << std::endl;
  //  }
  Tmap_file.close();

}

void Tmap::caculateTmap(){
    // odd
    if((_N_h%2==1)||(_N_w%2==1)){
        Tmax=_array_temperature[0];
        Tmin=Tmax;

        for(unsigned int i=1;i<_N_h*_N_w-1;i=i+2){
            if(_array_temperature[i]>_array_temperature[i+1]){
                if (_array_temperature[i]>Tmax){
                    Tmax=_array_temperature[i];
                }
                if (_array_temperature[i+1]<Tmin){
                    Tmin=_array_temperature[i+1];
                }
            }
            else{
                if (_array_temperature[i+1]>Tmax){
                    Tmax=_array_temperature[i+1];
                }
                if (_array_temperature[i]<Tmin){
                    Tmin=_array_temperature[i];
                }
            }
        }
    }
    //even
    else{
        for(unsigned int i=0;i<_N_h*_N_w;i=i+2){
            if(_array_temperature[i]>_array_temperature[i+1]){
                if (_array_temperature[i]>Tmax){
                    Tmax=_array_temperature[i];
                }
                if (_array_temperature[i+1]<Tmin){
                    Tmin=_array_temperature[i+1];
                }
            }
            else{
                if (_array_temperature[i+1]>Tmax){
                    Tmax=_array_temperature[i+1];
                }
                if (_array_temperature[i]<Tmin){
                    Tmin=_array_temperature[i];
                }
            }
        }
    }
    deltaT=Tmax-Tmin;
}

void Tmap::turnOverMatrix(){
    double temp;
        size_t halfHeight=_N_h/2;
        for(size_t i=0;i<halfHeight;i++){
            for(size_t j=0;j<_N_w;j++){
               temp=_array_temperature[i*_N_w+j];
               _array_temperature[i*_N_w+j]=_array_temperature[(_N_h-1-i)*_N_w+j];
               _array_temperature[(_N_h-1-i)*_N_w+j]=temp;
            }
        }
}
/*
void Tmap::updateTmap(const char* Tmap_template_file){

    std::string line;
    std::string delims(" \n\t\r");
    //std::stringstream str;
    std::ifstream Tmap_file1;

    std::string temp;
    Tmap_file1.open(Tmap_template_file);
    if(!Tmap_file1.is_open()){
        //std::cout<<"can't open file:"<<Tmap_template_file;
        Tmap_file1.clear();
    }

    for (size_t i = 0; i < _N_h; i++) {
      getline(Tmap_file1, line);
      // remove comment, need to be more roburst
      if (line[0]=='%'){
        getline(Tmap_file1, line);
        //std::cout<<line<<std::endl;
      }
      for (size_t j = 0; j < _N_w; j++) {
        //  str<<(tokenizer(line, delims));
        //  str>>_array_temperature[i*_N_w + j];
          temp=tokenizer(line, delims);
          //std::cout<<"before atof: "<<temp;
        _array_temperature[i*_N_w + j] = atof(temp.c_str());
        // Tmap_file1>>_array_temperature[i*_N_w + j];
        //std::cout<<_array_temperature[i*_N_w + j]<<' ';
      }
    }
    //show
    //for (size_t i = 0; i < _N_h; i++) {
    //	  for (size_t j = 0; j < _N_w; j++) {
    //		  std::cout << _array_temperature[i*_N_w + j] << " ";
    //	  }
    //	  std::cout << std::endl;
    //  }

    caculateTmap();
    // waiting to be done: turnoverMatrix while reading
    turnOverMatrix();
//    Tmap_file.clear();
    Tmap_file1.close();
}
*/
void Tmap::updateTmap(ddmat ts){
//    std::cout<<"\n Let's see ts[0](5,5)"<<ts(5,5)<<std::endl;
    for(size_t i=0;i<_N_h;++i){
        for(size_t j=0;j<_N_w;++j){
            _array_temperature[i*_N_w+j]=ts(i,j);
        }
    }
    std::cout<<"\n Let's see _array_temperature(15)"<<_array_temperature[15]<<std::endl;
    caculateTmap();
}
//check is needed, improvement
double Tmap::maxTPart1(double &partMinT){
    double maxT=-1;
    double minT=-1;
    for(unsigned int i=0;i<_N_h/2;++i){
        for(unsigned int j=0;j<_N_w/2;++j){
            if(_array_temperature[i*_N_w+j]<minT){
                minT=_array_temperature[i*_N_w+j];
            }
            if(_array_temperature[i*_N_w+j]>maxT){
                maxT=_array_temperature[i*_N_w+j];
            }
        }
    }
    partMinT=minT;
    return maxT;
}

double Tmap::maxTPart2(double &partMinT){
    double maxT=-1;
    double minT=-1;
    for(unsigned int i=_N_h/2+1;i<_N_h;++i){
        for(unsigned int j=0;j<_N_w/2;++j){
            if(_array_temperature[i*_N_w+j]<minT){
                minT=_array_temperature[i*_N_w+j];
            }
            if(_array_temperature[i*_N_w+j]>maxT){
                maxT=_array_temperature[i*_N_w+j];
            }
        }
    }
    partMinT=minT;
    return maxT;
}

double Tmap::maxTPart3(double &partMinT){
    double maxT=-1;
    double minT=-1;
    for(unsigned int i=0;i<_N_h/2;++i){
        for(unsigned int j=_N_w/2+1;j<_N_w;++j){
            if(_array_temperature[i*_N_w+j]<minT){
                minT=_array_temperature[i*_N_w+j];
            }
            if(_array_temperature[i*_N_w+j]>maxT){
                maxT=_array_temperature[i*_N_w+j];
            }
        }
    }
    partMinT=minT;
    return maxT;
}

double Tmap::maxTPart4(double &partMinT){
    double maxT=-1;
    double minT=-1;
    for(unsigned int i=_N_h/2+1;i<_N_h;++i){
        for(unsigned int j=_N_w/2+1;j<_N_w;++j){
            if(_array_temperature[i*_N_w+j]<minT){
                minT=_array_temperature[i*_N_w+j];
            }
            if(_array_temperature[i*_N_w+j]>maxT){
                maxT=_array_temperature[i*_N_w+j];
            }
        }
    }
    partMinT=minT;
    return maxT;
}
