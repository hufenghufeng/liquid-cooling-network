#include "../inc/mchannel.h"

void mchannel::read_from_file(size_t N_h, size_t N_w,
			     const char* channel_template_file)
{
  clear();
  string line;
  string delims(" \n\t\r");
  _N_h = N_h;
  _N_w = N_w;
  
  _array_ptr = new int[_N_h*_N_w];
  memset(_array_ptr, 0, _N_h*_N_w*sizeof(int));
  // read from file.
  // todo !!!!!!!!!!!!!!!!
  
  std::ifstream channel_file;
  channel_file.open(channel_template_file);
  for (size_t i = 0; i < _N_h; i++) {
    getline(channel_file, line);
    for (size_t j = 0; j < _N_w; j++) {
      _array_ptr[i*_N_w + j] = atoi(tokenizer(line, delims).c_str());
    }
  }
  //show
  //for (int i = 0; i < _N_h; i++) {
  //	  for (int j = 0; j < _N_w; j++) {
  //		  std::cout << _array_ptr[i*_N_w + j] << " ";
  //	  }
  //	  std::cout << std::endl;
  //  }
  channel_file.close();

  _array_mark = new int[_N_h*_N_w];
  memcpy(_array_mark, _array_ptr, _N_h*_N_w*sizeof(int));
}

void mchannel::correct()
{
  size_t cur_index, nei_index;
  assert((_N_h>0)&&(_N_w>0));
  assert(_array_ptr!=NULL);
  
  // correct inner io
  for(size_t i=1; i<_N_h-1; ++i)
    for (size_t j=1; j<_N_w-1; ++j)
      if ((_array_ptr[i*_N_w+j]==2)||(_array_ptr[i*_N_w+j]==3))
	_array_ptr[i*_N_w+j] = 1;
  
  int *flag1 = new int[_N_h*_N_w];
  memset(flag1, 0, _N_h*_N_w);
  int *flag2 = new int[_N_h*_N_w];
  memset(flag2, 0, _N_h*_N_w);
  // check connectivity
  stack<size_t> index_stack;
  // from inlet
  for (size_t j=0; j<_N_w-1; ++j)
    if (_array_ptr[j]==2)
      {
	index_stack.push(j);
	flag1[j] = 1;
      }
  for (size_t i=0; i<_N_h-1; ++i)
    if (_array_ptr[i*_N_w+_N_h-1]==2)
      {
	index_stack.push(i*_N_w+_N_h-1);
	flag1[i*_N_w+_N_h-1] = 1;
      }
  for (size_t j=0; j<_N_w-1; ++j)
    if (_array_ptr[(_N_h-1)*_N_w+j]==2)
      {
	index_stack.push((_N_h-1)*_N_w+j);
	flag1[(_N_h-1)*_N_w+j] = 1;
      }
  for (size_t i=0; i<_N_h-1; ++i)
    if (_array_ptr[i*_N_w]==2)
      {
	index_stack.push(i*_N_w+_N_h-1);
	flag1[i*_N_w] = 1;
      }
  while(!index_stack.empty())
    {
      cur_index = index_stack.top();
      index_stack.pop();
      for (size_t k=0;k<4;++k)
	{
	  nei_index = neighbor_index(cur_index, k);
	  if (nei_index<_N_h*_N_w)
	    {
	      if ((_array_ptr[nei_index]>0) // for 1,2,3
		  &&(flag1[nei_index]==0))
		{
		  index_stack.push(nei_index);
		  flag1[nei_index]=1;
		}
	    }
	}
    }

  // from outlet  
  for (size_t j=0; j<_N_w-1; ++j)
    if (_array_ptr[j]==3)
      {
	index_stack.push(j);
	flag2[j] = 1;
      }
  for (size_t i=0; i<_N_h-1; ++i)
    if (_array_ptr[i*_N_w+_N_h-1]==3)
      {
	index_stack.push(i*_N_w+_N_h-1);
	flag2[i*_N_w+_N_h-1] = 1;
      }
  for (size_t j=0; j<_N_w-1; ++j)
    if (_array_ptr[(_N_h-1)*_N_w+j]==3)
      {
	index_stack.push((_N_h-1)*_N_w+j);
	flag2[(_N_h-1)*_N_w+j] = 1;
      }
  for (size_t i=0; i<_N_h-1; ++i)
    if (_array_ptr[i*_N_w]==3)
      {
	index_stack.push(i*_N_w+_N_h-1);
	flag2[i*_N_w] = 1;
      }
  while(!index_stack.empty())
    {
      cur_index = index_stack.top();
      index_stack.pop();
      for (size_t k=0;k<4;++k)
	{
	  nei_index = neighbor_index(cur_index, k);
	  if (nei_index<_N_h*_N_w)
	    {
	      if ((_array_ptr[nei_index]>0) // for 1,2,3
		  &&(flag2[nei_index]==0))
		{
		  index_stack.push(nei_index);
		  flag2[nei_index]=1;
		}
	    }
	}
    }
  // correct channel
  for (size_t k=0;k<_N_h*_N_w;++k)
    if (_array_ptr[k]>0) // for 1,2,3
      if ((flag1[k]==0)||(flag2[k]==0)) // unreachable
	_array_ptr[k]=0;

}

void mchannel::newCorrect(){
    int **matrix;
    matrix = (int**)calloc(_N_h, sizeof(int*));
    for (size_t i = 0;i < _N_w;i++) {
        matrix[i] = (int*)calloc(_N_w, sizeof(int));
        if (!matrix[i])fprintf(stderr, "memory alloction fail!\n");
    }

    for(size_t  i=0;i<_N_h;++i){
        for(size_t j=0;j<_N_w;++j){
            matrix[i][j]=_array_ptr[i*_N_w+j];
        }
    }

    sizeT size={_N_h,_N_w};

    matrix=ChooseIOAndCheckConnect(matrix,size);

    for(size_t  i=0;i<_N_h;++i){
        for(size_t j=0;j<_N_w;++j){
            _array_ptr[i*_N_w+j]=matrix[i][j];
        }
    }

    for (size_t i = 0;i < _N_w;i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int mchannel::adjancentLiquidCellNumber(size_t row, size_t col){
    int number=0;
    if (row>0){
        if(_array_ptr[(row-1)*_N_w+col]>0){
            number++;
        }
    }
    if (row<_N_w-1){
        if(_array_ptr[(row+1)*_N_w+col]>0){
            number++;
        }
    }
    if(col>0){
        if(_array_ptr[row*_N_w+col-1]>0){
            number++;
        }
    }
    if(col<_N_h-1){
        if(_array_ptr[row*_N_w+col+1]>0){
            number++;
        }
    }
    return number;
}

Location mchannel::adjancentLiquidCellLocation(size_t row, size_t col){

    int location_r=0;
    int location_c=0;
    // up
    if (row>0){
        if(_array_ptr[(row-1)*_N_w+col]>0){
            location_r=row-1;
            location_c=col;
        }
    }
    // down
    if(row<_N_h){
        if(_array_ptr[(row+1)*_N_w+col]>0){
            location_r=row+1;
            location_c=col;
        }
    }
    // left
    if (col>0){
        if(_array_ptr[row*_N_w+col-1]>0){
            location_r=row;
            location_c=col-1;
        }
    }
    //right
    if (col<_N_w){
        if(_array_ptr[row*_N_w+col+1]>0){
            location_r=row;
            location_c=col+1;
        }
    }
    assert((location_c!=-1)&&(location_r!=-1));
    Location location(location_r,location_c);

    return location;
}

void mchannel::checkConnect(size_t row, size_t col){
    if (_array_ptr[row*_N_w+col]>0){
        if (adjancentLiquidCellNumber(row,col)==0){
            _array_ptr[row*_N_w+col]=0;
        }
        else if (adjancentLiquidCellNumber(row,col)==1){
            _array_ptr[row*_N_w+col]=0;
            Location location=Location();
            location=adjancentLiquidCellLocation(row,col);
            checkConnect(location.getRow(),location.getCol());
        }
    }
}

void mchannel::fillChannel(size_t row, size_t col){

    if(_array_ptr[row*_N_w+col]!=-1){
        if((row>0)&&(row<_N_h-1)&&(col>0)&&(col<_N_w-1)){
            _array_ptr[row*_N_w+col]=0;
            checkConnect(row  ,col+1);
            checkConnect(row-1,col  );
            checkConnect(row  ,col-1);
            checkConnect(row+1,col  );
        }
        else if ((row==0)&&(col>0)&&(col<_N_w-1)){
            _array_ptr[row*_N_w+col]=0;
            checkConnect(row+1,col);
        }
        else if((row==_N_h-1)&&(col>0)&&(col<_N_w-1)){
            _array_ptr[row*_N_w+col]=0;
            checkConnect(row-1,col);
        }
        else if((col==0)&&(row>0)&&(row<_N_h-1)){
            _array_ptr[row*_N_w+col]=0;
            checkConnect(row,col+1);
        }
        else if((col==_N_w-1)&&(row>0)&&(row<_N_h)){
            _array_ptr[row*_N_w+col]=0;
            checkConnect(row,col-1);
        }
    }
    else {
        if ((row==0)&&(col==0)){
            _array_ptr[(row+1)*_N_w+col]=0;
            _array_ptr[row*_N_w+col+1]=0;
            checkConnect(row+1,col+1);
        }
        else if((row==0)&&(col==_N_w-1)){
            _array_ptr[(row+1)*_N_w+col]=0;
            _array_ptr[row*_N_w+col-1]=0;
            checkConnect(row+1,col-1);
        }
        else if((row==_N_h-1)&&(col==0)){
            _array_ptr[(row-1)*_N_w+col]=0;
            _array_ptr[row*_N_w+col+1]=0;
            checkConnect(row-1,col+1);
        }
        else if((row==_N_h-1)&&(col==_N_w-1)){
            _array_ptr[(row-1)*_N_w+col]=0;
            _array_ptr[row*_N_w+col-1]=0;
            checkConnect(row-1,col-1);
        }
        else{
            fillSolid(row,col);
        }
    }
}

void mchannel::fillSolid(size_t row, size_t col){
    if(((row>col)&&(row>_N_w-1-col))||((row<col)&&(row<_N_w-col))){
        if ((row>0)&&(row<_N_h-1)){
            _array_ptr[(row+1)*_N_w+col]=0;
            _array_ptr[(row-1)*_N_w+col]=0;
            checkConnect(row+1,col+1);
            checkConnect(row+1,col-1);
            checkConnect(row-1,col+1);
            checkConnect(row-1,col-1);
        }
    }
    else{
        if((col>0)&&(col<_N_w-1)){
            _array_ptr[row*_N_w+col-1]=0;
            _array_ptr[row*_N_w+col+1]=0;
            checkConnect(row+1,col+1);
            checkConnect(row+1,col-1);
            checkConnect(row-1,col+1);
            checkConnect(row-1,col-1);
        }
    }

    if (row==0){
        _array_ptr[(row+1)*_N_w+col]=0;
        checkConnect(row+1,col+1);
        checkConnect(row+1,col-1);
    }
    else if (row==_N_h-1){
        _array_ptr[(row-1)*_N_w+col]=0;
        checkConnect(row-1,col+1);
        checkConnect(row-1,col-1);
    }

    if(col==0){
        _array_ptr[row*_N_w+col+1]=0;
        checkConnect(row+1,col+1);
        checkConnect(row-1,col+1);
    }
    else if(col==_N_w-1){
        _array_ptr[row*_N_w+col-1]=0;
        checkConnect(row+1,col-1);
        checkConnect(row-1,col-1);
    }
}

std::vector<int> mchannel::chooseCell(const double* Tmap,int num){
    int size=_N_h*_N_w;
    node candidate[size];
    std::vector<int> chosenCellIndex;
    // judge whether size of Tmap is right
 //   std::cout<<"sizeof(Tmap)/sizeof(Tmap[0]"<<sizeof(Tmap)/sizeof(Tmap[0]);
    //assert(sizeof(Tmap)/sizeof(Tmap[0])==_N_h*_N_w);
//    for (size_t i=0;i<_N_h*_N_w;i++){
//        std::cout<<" "<<_array_ptr[i];
//    }
    for(size_t i=0;i<_N_h*_N_w;i++){
        candidate[i].temperature=Tmap[i];
        //std::cout<<candidate[i].temperature<<std::endl;

        candidate[i].cell=_array_ptr[i];
        //std::cout<<candidate[i].cell<<std::endl;

        candidate[i].mark=_array_mark[i];
        candidate[i].index=i;
    }
    std::sort(candidate,candidate+_N_h*_N_w,compare);
/*    std::cout<<"after sort:"<<std::endl;
    for (size_t i=0;i<_N_h*_N_w;i++){
        std::cout<<"candidate[i].temperature:"<<candidate[i].temperature<<"i: "<<candidate[i].index<<std::endl;

    }*/
    //choose and mark
    int chosenNum=0;
//    int number=0;
    for (size_t i=0;i<_N_h*_N_w;i++){
//    for (size_t i=0;i<_N_h*_N_w;i++){
        /*
        for(size_t i=0;i<_N_h*_N_w;i++){
            if (candidate[i].mark==0)number++;
        }
        std::cout<<"number of 0 marks:"<<number;
        number=0;
        for(size_t i=0;i<_N_h*_N_w;i++){
            if (candidate[i].cell==0)number++;
        }
        std::cout<<"number of 0 cells:"<<number;
        number=0;
        */
        if ((candidate[i].mark!=0)&&(candidate[i].cell!=0)){
            chosenCellIndex.push_back(candidate[i].index);
            std::cout<<"choose index from Tmap:"<<candidate[i].index<<std::endl;
            markCell(candidate[i].index);
            chosenNum++;
        }
        if (chosenNum>=num)break;
    }
    std::cout<<"lenght of chosenCellIndex"<<chosenCellIndex.size()<<std::endl;
    for(vector<int>::iterator iter=chosenCellIndex.begin();
                              iter!=chosenCellIndex.end();
                              iter++){
        std::cout<<"chosen cell"<<(*iter)<<std::endl;
    }

    return chosenCellIndex;
}

void mchannel::fillHotArea(const double* Tmap1,const double* Tmap2,double percent){
    int size=_N_h*_N_w;
    node candidate[size];
    std::vector<int> chosenCellIndex;
    // judge whether size of Tmap is right
 //   std::cout<<"sizeof(Tmap)/sizeof(Tmap[0]"<<sizeof(Tmap)/sizeof(Tmap[0]);
    //assert(sizeof(Tmap)/sizeof(Tmap[0])==_N_h*_N_w);
//    for (size_t i=0;i<_N_h*_N_w;i++){
//        std::cout<<" "<<_array_ptr[i];
//    }
    for(size_t i=0;i<_N_h*_N_w;i++){
        candidate[i].temperature=std::max(Tmap1[i],Tmap2[i]);
        //std::cout<<candidate[i].temperature<<std::endl;

        candidate[i].cell=_array_ptr[i];
        //std::cout<<candidate[i].cell<<std::endl;

        candidate[i].mark=_array_mark[i];
        candidate[i].index=i;
    }
    std::sort(candidate,candidate+_N_h*_N_w,compare);
    /*
    % divide into the four area
    %   \ 1  /
    %    \  /
    %  2  \/  4
    %     /\
    %    /  \
    %   / 3  \
    %  in 1,3 fill by :
    %     0
    %     -1
    %     0
    %  in 2,4 fill by : 0 -1 0


    % which part？
    % row>col && row> size-col ::1
    % row>col && row< size-col ::2

    % row<col && row> size-col ::4
    % row<col && row< size-col ::3
    */
    //choose and mark
    int row=-1;
    int col=-1;
    for (size_t i=_N_h*_N_w-1;i>_N_h*_N_w*(1-percent);i--){
// to do : I think I should add TSV mark
        if ((candidate[i].mark!=0)&&(candidate[i].cell!=0)){
            row=candidate[i].index/_N_w;
            col=candidate[i].index%_N_w;
            // one is odd and one is enen
            if ((row+col)&1){
                // 1,3 fill if col is even
                if((row>col&&row>_N_h-1-col)||(row<col&&row<_N_h-col)){
                    if(!(col&1)){
                        fillChannel(row,col);
                    }
                }
                else{
                    // 2,4 fill if row is even
                    if(!(row&1)){
                        fillChannel(row,col);
                    }
                }
            }
        }
    }
    std::ofstream fouttttt;
    fouttttt.open("fillHotArea");
    fouttttt<<(*this);
}

void mchannel::fillOutputArea(int size){
    int testPoint1=_N_h/2-11;
    int testPoint2=testPoint1-10;
    int testPoint3=testPoint2-10;
    int halfSize=_N_h/2;

    int beginPoint=_N_w/2-1-size;//odd
    // part 1 ,detect output side
    if((_array_ptr[testPoint1*_N_w+0]==OUTPUT)
            &&(_array_ptr[testPoint2*_N_w+0]==OUTPUT)
            &&(_array_ptr[testPoint3*_N_w+0]==OUTPUT)){
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(beginPoint+i)*_N_w+j]=SOLID;
            }
        }
    }
    else{
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(beginPoint+i)+j*_N_w]=SOLID;
            }
        }
    }

    //part 2
    if((_array_ptr[(halfSize+testPoint1)*_N_w+0]==OUTPUT)&&
          (_array_ptr[(halfSize+testPoint2)*_N_w+0]==OUTPUT)&&
          (_array_ptr[(halfSize+testPoint2)*_N_w+0]==OUTPUT)){
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(_N_h-1-(beginPoint+i))*_N_w+j]=SOLID;
            }
        }
    }
    else{
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(_N_h-1-j)*_N_w+(beginPoint+i)]=SOLID;
            }
        }
    }

    // part 3 ,detect output side
    if((_array_ptr[testPoint1*_N_w+_N_w-1]==OUTPUT)
            &&(_array_ptr[testPoint2*_N_w+_N_w-1]==OUTPUT)
            &&(_array_ptr[testPoint3*_N_w+_N_w-1]==OUTPUT)){
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(beginPoint+i)*_N_w+_N_w-1-j]=SOLID;
            }
        }
    }
    else{
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[_N_w-1-(beginPoint+i)+j*_N_w]=SOLID;
            }
        }
    }

    //part 4
    if((_array_ptr[(halfSize+testPoint1)*_N_w+_N_w-1]==OUTPUT)&&
          (_array_ptr[(halfSize+testPoint2)*_N_w+_N_w-1]==OUTPUT)&&
          (_array_ptr[(halfSize+testPoint2)*_N_w+_N_w-1]==OUTPUT)){
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(_N_h-1-(beginPoint+i))*_N_w+_N_w+1-j]=SOLID;
            }
        }
    }
    else{
        for(int i=1;i<size;i=i+2){
            for(int j=1;j<i;j=j+2){
                _array_ptr[(_N_h+1-j)*_N_w+_N_h-1-(beginPoint+i)]=SOLID;
            }
        }
    }
}

void mchannel::fillMidArea(int size){
    size_t center=_N_h/2;
    for(int i=2;i<=size/sqrt(2)+2;i=i+2){
        // part 4
        for(int horizon=1;horizon<=size-i;horizon=horizon+2){
            if(_array_ptr[(center+i)*_N_w+center+i+horizon]==1){
                _array_ptr[(center+i)*_N_w+center+i+horizon]=0;
            }
        }

        for(int vertical=1;vertical<=size-i;vertical=vertical+2){
            if(_array_ptr[(center+i+vertical)*_N_w+center+i]==1){
                _array_ptr[(center+i+vertical)*_N_w+center+i]=0;
            }
        }
        // part 3
        for(int horizon=1;horizon<=size-i;horizon=horizon+2){
            if(_array_ptr[(center+i)*_N_w+center-i-horizon]==1){
                _array_ptr[(center+i)*_N_w+center-i-horizon]=0;
            }
        }

        for(int vertical=1;vertical<=size-i;vertical=vertical+2){
            if(_array_ptr[(center+i+vertical)*_N_w+center-i]==1){
                _array_ptr[(center+i+vertical)*_N_w+center-i]=0;
            }
        }
        // part 2
        for(int horizon=1;horizon<=size-i;horizon=horizon+2){
            if(_array_ptr[(center-i)*_N_w+center+i+horizon]==1){
                _array_ptr[(center-i)*_N_w+center+i+horizon]=0;
            }
        }

        for(int vertical=1;vertical<=size-i;vertical=vertical+2){
            if(_array_ptr[(center-i-vertical)*_N_w+center+i]==1){
                _array_ptr[(center-i-vertical)*_N_w+center+i]=0;
            }
        }
        // part 1
        for(int horizon=1;horizon<=size-i;horizon=horizon+2){
            if(_array_ptr[(center-i)*_N_w+center-i-horizon]==1){
                _array_ptr[(center-i)*_N_w+center-i-horizon]=0;
            }
        }

        for(int vertical=1;vertical<=size-i;vertical=vertical+2){
            if(_array_ptr[(center-i-vertical)*_N_w+center-i]==1){
                _array_ptr[(center-i-vertical)*_N_w+center-i]=0;
            }
        }
    }
}

void mchannel::markCell(int index)
{
    _array_mark[index]=0;
}

bool compare(node a, node b){
    return a.temperature<b.temperature;
}

void mchannel::initialChannel(int index){
    std::ofstream fouttttt;
    fouttttt.open("beforefill1");
    fouttttt<<(*this);
    // fill all 1
    for(unsigned int i=0;i<_N_h;i++){
        for(unsigned int j=0;j<_N_w;j++){
            _array_ptr[i*_N_w+j]=1;
        }
    }
 //   channel.set(54,51,0);
 //   channel.set(54,53,0);
 //   channel.set(53,54,0);
  //  channel.set(51,54,0);
/*
    _array_ptr[54*_N_w+51]=0;
    _array_ptr[54*_N_w+53]=0;
    _array_ptr[53*_N_w+54]=0;
    _array_ptr[51*_N_w+54]=0;
*/
    //drawFourSameLine(20,0.25);
    std::ofstream foutttt;
    foutttt.open("beforefill-1");
    foutttt<<(*this);
    // fill all -1
    for(size_t i=0;i<_N_h;i=i+2){
        for(size_t j=0;j<_N_w;j=j+2){
            _array_ptr[i*_N_w+j]=-1;
        }
    }
    // draw two seperating lines
    for(size_t i=1;i<_N_h-1;i=i+2){
        _array_ptr[i*_N_w+(_N_h-1)/2]=0;
        _array_ptr[(_N_h-1)/2*_N_w+i]=0;
    }
    /*
    std::ofstream fouttt;
    fouttt.open("beforeSwitch");
    fouttt<<(*this);
    */
    string line;
    string delims(" \n\t\r");
    std::ifstream channelFile;
    switch(index){
    // case1 212
    case 1:
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[i*_N_w+_N_w-1]=3;
            _array_ptr[(_N_h-1)*_N_w+i]=2;
        }

        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        //std::ofstream fouttt;
       // fouttt.open("beforeSwitch");
       // fouttt<<(*this);
        //for(size_t i=2;i<_N_h;i=i+2){
        //    _array_ptr[i*_N_w+(_N_h-1)/2-1]=1;
       // }

        drawLine(99,1 ,99-26*cos(0.3*PI ),1+26*sin(0.3*PI));
        drawLine(1 ,1 ,1 +31*cos(0.22*PI),1+31*sin(0.22*PI));
        drawLine(99,99,99-30*cos(0.2*PI),99-30*sin(0.2*PI));
        drawLine(1 ,99,1 +37*cos(0.24*PI),99-37*sin(0.24*PI));
        //std::ofstream foutt;
        //foutt.open("beforeDrawLine");
        //foutt<<(*this);
        //fillOutputArea(26);
        //fillMidArea(20);
        //_array_ptr[51*_N_w+54]=0;
        //_array_ptr[53*_N_w+54]=0;
        //_array_ptr[54*_N_w+53]=0;
        //_array_ptr[53*_N_w+50]=1;
        break;
    case 2:
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[i*_N_w+_N_w-1]=3;
            _array_ptr[(_N_h-1)*_N_w+i]=2;
        }

        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        drawLine(99,1 ,99-28*cos(0.26*PI ),1+28*sin(0.26*PI));
        drawLine(1 ,1 ,1 +34*cos(0.24*PI),1+34*sin(0.24*PI));
        drawLine(99,99,99-34*cos(0.24*PI),99-34*sin(0.24*PI));
        drawLine(1 ,99,1 +40*cos(0.22*PI),99-40*sin(0.22*PI));
        fillOutputArea(24);
        fillMidArea(18);

        break;
    case 3:
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[i*_N_w+_N_w-1]=3;
            _array_ptr[(_N_h-1)*_N_w+i]=2;
        }
        for(int i=44;i<57;++i){
            for(int j=22;j<77;++j){
                if(_array_ptr[i*_N_w+j]!=TSV){
                    fillChannel(i,j);
                }
            }
        }
        for(int i=46;i<53;i=i+2){
            for(int j=81;j<86;j=j+2){
                fillChannel(i,j);
            }
        }
        break;
    case 40:
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[i*_N_w+_N_w-1]=3;
            _array_ptr[(_N_h-1)*_N_w+i]=2;
        }

        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        drawLine(99,1 ,99-46*cos(0.25*PI ),1+46*sin(0.25*PI));
        drawLine(1 ,1 ,1 +46*cos(0.25*PI),1+46*sin(0.25*PI));
        drawLine(99,99,99-46*cos(0.25*PI),99-46*sin(0.25*PI));
        drawLine(1 ,99,1 +46*cos(0.25*PI),99-46*sin(0.25*PI));
        break;
    case 41:
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[i*_N_w+_N_w-1]=3;
            _array_ptr[(_N_h-1)*_N_w+i]=2;
        }

        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        drawLine(99,1 ,99-30*cos(0.24*PI ),1+30*sin(0.24*PI));
        drawLine(1 ,1 ,1 +34*cos(0.24*PI),1+34*sin(0.24*PI));
        drawLine(99,99,99-34*cos(0.24*PI),99-34*sin(0.24*PI));
        drawLine(1 ,99,1 +40*cos(0.24*PI),99-40*sin(0.24*PI));
        break;

    case 5:

        channelFile.open("channel5.dat");
        for (size_t i = 0; i < _N_h; i++) {
          getline(channelFile, line);
          for (size_t j = 0; j < _N_w; j++) {
            _array_ptr[i*_N_w + j] = atoi(tokenizer(line, delims).c_str());
          }
        }
        channelFile.close();
        break;
/*    case 110:
        //   2  2
        //2       3
        //2       3
        //   3  3
        //3232
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[i*_N_w+_N_w-1]=3;
            _array_ptr[(_N_h-1)*_N_w+i]=2;
        }
        break;
    case 111:
        //   3  3
        //3       2
        //3       2
        //   2  2
        //2323
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[i*_N_w+_N_w-1]=2;
            _array_ptr[(_N_h-1)*_N_w+i]=3;
        }
        break;*
    case 112:
        //   2  2
        //3       2
        //3       2
        //   3  3
        //3322
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;//south
            _array_ptr[i*_N_w+0]=3;//west
            _array_ptr[i*_N_w+_N_w-1]=2;//east
            _array_ptr[(_N_h-1)*_N_w+i]=2;//north
        }
        break;
    case 113:
        //   3  3
        //2       3
        //2       3
        //   2  2
        //2233
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=2;//south
            _array_ptr[i*_N_w+0]=2;//west
            _array_ptr[i*_N_w+_N_w-1]=3;//east
            _array_ptr[(_N_h-1)*_N_w+i]=3;//north
        }
        break;*/
    /*case 114:
        //   3  3
        //2       2
        //2       2
        //   3  3
        //3223
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=3;//south
            _array_ptr[i*_N_w+0]=2;//west
            _array_ptr[i*_N_w+_N_w-1]=2;//east
            _array_ptr[(_N_h-1)*_N_w+i]=3;//north
        }
        break;
    case 115:
        //   2  2
        //3       3
        //3       3
        //   2  2
        //2332
        for(size_t i=1;i<_N_h-1;i=i+2){
            _array_ptr[0*_N_w+i]=2;//south
            _array_ptr[i*_N_w+0]=3;//west
            _array_ptr[i*_N_w+_N_w-1]=3;//east
            _array_ptr[(_N_h-1)*_N_w+i]=2;//north
        }
        break;
    case 116:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;
        */
    /*case 117:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;*/
 /*   case 118:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;*/
/*    case 119:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;*/
/*    case 120:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;*/
   /* case 121:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;*
    case 122:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;
    case 123:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;*/
    /*case 124:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;
    case 125:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;
    case 126:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;
    case 127:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;
    case 128:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;

            _array_ptr[(_N_h-1)*_N_w+i]=2;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;
    case 129:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=3;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;
        }
        break;
    case 130:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;
            _array_ptr[i*_N_w+0]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;

            _array_ptr[(_N_h-1)*_N_w+i]=3;
            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;
            _array_ptr[i*_N_w+(_N_h-1)]=2;
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;
        }
        break;*/
    case 100:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        break;
    case 101:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }

        break;
    case 102:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }

        break;
    case 103:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }

        break;
    case 104:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        break;
    case 105:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }
        break;
    case 106:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }
        break;
    case 107:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 32 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=2;//1
            _array_ptr[i*_N_w+0]=3;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        break;

    case 108:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        break;
    case 109:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }
        break;
    case 110:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 23 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }
        break;
    case 111:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //23 23 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=2;//4
            _array_ptr[(_N_h-1)*_N_w+i]=3;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        break;
    case 112:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 23 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        break;
    case 113:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 23 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=2;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=3;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }
        break;
    case 114:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 32 32
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=3;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=2;//2
        }
        break;
    case 115:
        //   3  2
        //2       3
        //3       2
        //   2  3
        //32 32 32 23
        for(size_t i=1;i<(_N_w-1)/2;i=i+2){
            _array_ptr[0*_N_w+i]=3;//1
            _array_ptr[i*_N_w+0]=2;//3

            _array_ptr[((_N_w-1)/2+i)*_N_w+0]=3;//4
            _array_ptr[(_N_h-1)*_N_w+i]=2;//5

            _array_ptr[(_N_h-1)*_N_w+(_N_w-1)/2+i]=3;//6
            _array_ptr[((_N_w-1)/2+i)*_N_w+(_N_h-1)]=2;//8

            _array_ptr[i*_N_w+(_N_h-1)]=2;//7
            _array_ptr[0*_N_w+(_N_w-1)/2+i]=3;//2
        }
        //fillOutputArea(26);
        break;
    default:
        fprintf(stderr,"\nThe index of the channel is not defined, choose another one please\n");
        exit(0);
    }

    memcpy(_array_mark, _array_ptr, _N_h*_N_w*sizeof(int));
}

void mchannel::drawLine(int x1, int y1, int x2, int y2){
    assert (x1>=0&&x1<(int)_N_w);
    assert (x2>=0&&x2<(int)_N_w);
    assert (y1>=0&&y1<(int)_N_h);
    assert (y2>=0&&y2<(int)_N_h);

    double k;

    if(x1!=x2){
        k=((float)(y2-y1))/((float)(x2-x1));
        if(x2<x1){
            for(int i=x1;i>=x2;i--){
                if(y2>y1){
                    for(int j=round(k*(i-x1)+y1);j<=round(k*(i-1-x1)+y1);j++){
                      if(_array_ptr[i*_N_w+j]!=-1){
                        fillChannel(i,j);
                      }
                    }
                }
                else{
                    for(int j=round(k*(i-x1)+y1);j>=round(k*(i-1-x1)+y1);j--){
                      if(_array_ptr[i*_N_w+j]!=-1){
                        fillChannel(i,j);
                      }
                    }
                }
            }
        }
        else{
            for(int i=x1;i<x2;i++){
                if(y2>y1){
                    for(int j=round(k*(i-x1)+y1);j<=round(k*(i+1-x1)+y1);j++){
                        if(_array_ptr[i*_N_w+j]!=-1){
                          fillChannel(i,j);
                        }
                    }
                }
                else{
                    for(int j=round(k*(i-x1)+y1);j>=round(k*(i+1-x1)+y1);j--){
                        if(_array_ptr[i*_N_w+j]!=-1){
                          fillChannel(i,j);
                        }
                    }
                }
            }

       }
    }
    else{
        if(y1<y2){
            for(int i=y1;i<=y2;i++){
                if(_array_ptr[x1*_N_w+i]!=-1){
                  fillChannel(x1,i);
                }
            }
        }
        else{
            for(int i=y1;i>=y2;i--){
                if(_array_ptr[x1*_N_w+i]!=-1){
                  fillChannel(x1,i);
                }
            }
        }
    }
}

void mchannel::drawFourSameLine(int length, double theta){
    drawLine(1              ,1               ,1              +length*cos(theta*PI),1               +length*sin(theta*PI));
    drawLine(_N_h-2,1               ,_N_h-2-length*cos(theta*PI),1              +length*sin(theta*PI));
    drawLine(1              ,_N_w-2,1              +length*cos(theta*PI),_N_w-2-length*sin(theta*PI));
    drawLine(_N_w-2,_N_w-2,_N_w-2-length*cos(theta*PI),_N_w-2-length*sin(theta*PI));
}

channelT mchannel::ChooseIOAndCheckConnect(channelT channel,sizeT channelSize){

    //choose an inlet
    locationT IOlocation;
    IOlocation =IOExamine(channel,InletUntip,channelSize);
#ifdef PRINTLOCATION
    fprintf(stderr,"one of the inlet unmarkend location,row:%d column:%d \n",IOlocation.row,IOlocation.column);
#endif
    if ((IOlocation.row+IOlocation.column)==0){
        printf("the channel has no inlet!\n");
        exit(0);
    }
    else{
        do{

            MakeTips(channel,IOlocation.row,IOlocation.column,channelSize);
#ifdef PRINTMATRIX
                fprintf(stderr,"\nthe marked channel:\n");
                PrintMaxtrix(channel,channelSize);
#endif
            // is there an outlet reached?
            IOlocation=IOExamine(channel,OutletTiped,channelSize);
#ifdef PRINTLOCATION
            fprintf(stderr,"one of the outlet markend location,row:%d column:%d \n",IOlocation.row,IOlocation.column);
#endif
            if ((IOlocation.row+IOlocation.column)==0){
                 //no outlet reached
                //FillTips(channel,channelSize);
#ifdef PRINTMATRIX
                fprintf(stderr,"\nthe fill dead end maxtrix size is :%d row* %d column\n",channelSize.row_size,channelSize.column_size);
                PrintMaxtrix(channel,channelSize);
#endif
                IOlocation=IOExamine(channel,InletUntip,channelSize);
            }
            else{
                // mark twice,begin from the outlet
                ReMakeTips(channel,IOlocation.row,IOlocation.column,channelSize);
#ifdef PRINTMATRIX
                fprintf(stderr,"\nthe re mark matrix  is:\n ");
                PrintMaxtrix(channel,channelSize);
#endif
                // Is there any inlet untiped?
                IOlocation=IOExamine(channel,InletUntip,channelSize);

#ifdef PRINTLOCATION
                fprintf(stderr,"one of the inlet unmarked location,row:%d column:%d \n",IOlocation.row,IOlocation.column);
#endif
            }
        }while((IOlocation.row+IOlocation.column)!=0);
    }
    channel=RecoverTips(channel,channelSize);

    return channel;
}

void mchannel::MakeTips(channelT channel,rowT row,colT col,sizeT size){

    int row_size=size.row_size;
    int col_size=size.column_size;

    if (row<row_size && col<col_size && row>=0 && col>=0 && (channel[row][col]==CHANNEL || channel[row][col]==INLET||channel[row][col]==OUTLET)){

        channel[row][col]+=(MARKEDCHANNEL-CHANNEL);

        if (row<row_size)MakeTips(channel,row+1,col  ,size);
        if (row>0)       MakeTips(channel,row-1,col  ,size);
        if (col<col_size)MakeTips(channel,row  ,col+1,size);
        if (col>0)       MakeTips(channel,row  ,col-1,size);

    }

}

void mchannel::ReMakeTips(channelT channel,rowT row,colT col,sizeT size){
    int row_size=size.row_size;
    int col_size=size.column_size;

    if (row<row_size && col<col_size && row>=0 && col>=0 && (channel[row][col]==MARKEDCHANNEL || channel[row][col]==MARKEDINLET||channel[row][col]==MARKEDOUTLET)){

        channel[row][col]+=(REMARKEDCHANNEL-MARKEDCHANNEL);

        if (row<row_size)ReMakeTips(channel,row+1,col  ,size);
        if (row>0)       ReMakeTips(channel,row-1,col  ,size);
        if (col<col_size)ReMakeTips(channel,row  ,col+1,size);
        if (col>0)       ReMakeTips(channel,row  ,col-1,size);

    }
}

channelT mchannel::RecoverTips(channelT channel,sizeT size){
    int row_index;
    int col_index;

    for(row_index=0;row_index<size.row_size;row_index++){
        for(col_index=0;col_index<size.column_size;col_index++){
            if(channel[row_index][col_index]==REMARKEDCHANNEL||channel[row_index][col_index]==REMARKEDINLET||channel[row_index][col_index]==REMARKEDOUTLET){
                channel[row_index][col_index]-=(REMARKEDCHANNEL-CHANNEL);
            }
            else {
                if(channel[row_index][col_index]>0&&channel[row_index][col_index]<REMARKEDCHANNEL)
                    channel[row_index][col_index]=SOLID;
            }
        }
    }
    return channel;
}

locationT mchannel::IOExamine(channelT channel,IOkindT IOkind,sizeT size){

    int row_index=0;
    int col_index=0;

    int IOnumber;

    locationT IOlocation;
    IOlocation.row=0;
    IOlocation.column=0;

    switch (IOkind){
        case InletUntip:

            IOnumber=INLET;
            for (col_index=1;col_index<size.column_size;col_index+=2){
                if(channel[0][col_index]==IOnumber){
                    IOlocation.row=0;
                    IOlocation.column=col_index;
                    return (IOlocation);
                }
            }
            for (col_index=1;col_index<size.column_size;col_index+=2){
                if(channel[size.row_size-1][col_index]==IOnumber){
                    IOlocation.row   =size.row_size-1;
                    IOlocation.column=col_index;
                    return (IOlocation);
                }
            }
            for (row_index=1;row_index<size.row_size;row_index+=2){
                if(channel[row_index][0]==IOnumber){
                    IOlocation.row   =row_index;
                    IOlocation.column=0;
                    return (IOlocation);
                }
            }
            for (row_index=1;row_index<size.row_size;row_index+=2){
                if(channel[row_index][size.column_size-1]==IOnumber){
                    IOlocation.row   =row_index;
                    IOlocation.column=size.column_size-1;
                    return (IOlocation);
                }
            }
            break;
        case InletTiped:
        IOnumber=MARKEDINLET;
        for (col_index=1;col_index<size.column_size;col_index+=2){
            if(channel[0][col_index]==IOnumber){
                IOlocation.row=0;
                IOlocation.column=col_index;
                return (IOlocation);
            }
        }
        for (col_index=1;col_index<size.column_size;col_index+=2){
            if(channel[size.row_size-1][col_index]==IOnumber){
                IOlocation.row   =size.row_size-1;
                IOlocation.column=col_index;
                return (IOlocation);
            }
        }
        for (row_index=1;row_index<size.row_size;row_index+=2){
            if(channel[row_index][0]==IOnumber){
                IOlocation.row   =row_index;
                IOlocation.column=0;
                return (IOlocation);
            }
        }
        for (row_index=1;row_index<size.row_size;row_index+=2){
            if(channel[row_index][size.column_size-1]==IOnumber){
                IOlocation.row   =row_index;
                IOlocation.column=size.column_size-1;
                return (IOlocation);
            }
        }
        break;
    case OutletUntip:
        IOnumber=OUTLET;
        for (col_index=1;col_index<size.column_size;col_index+=2){
            if(channel[0][col_index]==IOnumber){
                IOlocation.row=0;
                IOlocation.column=col_index;
                return (IOlocation);
            }
        }
        for (col_index=1;col_index<size.column_size;col_index+=2){
            if(channel[size.row_size-1][col_index]==IOnumber){
                IOlocation.row   =size.row_size-1;
                IOlocation.column=col_index;
                return (IOlocation);
            }
        }
        for (row_index=1;row_index<size.row_size;row_index+=2){
            if(channel[row_index][0]==IOnumber){
                IOlocation.row   =row_index;
                IOlocation.column=0;
                return (IOlocation);
            }
        }
        for (row_index=1;row_index<size.row_size;row_index+=2){
            if(channel[row_index][size.column_size-1]==IOnumber){
                IOlocation.row   =row_index;
                IOlocation.column=size.column_size-1;
                return (IOlocation);
            }
        }
        break;
    case OutletTiped:
        IOnumber=MARKEDOUTLET;
        for (col_index=1;col_index<size.column_size;col_index+=2){
            if(channel[0][col_index]==IOnumber){
                IOlocation.row=0;
                IOlocation.column=col_index;
                return (IOlocation);
            }
        }
        for (col_index=1;col_index<size.column_size;col_index+=2){
            if(channel[size.row_size-1][col_index]==IOnumber){
                IOlocation.row   =size.row_size-1;
                IOlocation.column=col_index;
                return (IOlocation);
            }
        }
        for (row_index=1;row_index<size.row_size;row_index+=2){
            if(channel[row_index][0]==IOnumber){
                IOlocation.row   =row_index;
                IOlocation.column=0;
                return (IOlocation);
            }
        }
        for (row_index=1;row_index<size.row_size;row_index+=2){
            if(channel[row_index][size.column_size-1]==IOnumber){
                IOlocation.row   =row_index;
                IOlocation.column=size.column_size-1;
                return (IOlocation);
            }
        }
        break;
    }//end switch
    return (IOlocation);
}

sizeT  mchannel::GetMatrixSize    (FILE* fp){

    char line[LINE_SIZE], temp[LINE_SIZE], *src;

    sizeT size={0,0};

    if(fgets(line, LINE_SIZE, fp)==NULL){
          fprintf(stderr,"fgets wrong");
          exit(1);
    }
    size.row_size++;
    strcpy(temp, line);
    src = strtok(temp, " \r\t\n");

    while (src != '\0') { size.column_size++; src = strtok(NULL, " \r\t\n"); }

    int col_num=0;
    while(fgets(line, LINE_SIZE, fp)!=NULL){
        size.row_size++;
        //Is every row has the same columns?
        col_num=0;
        strcpy(temp, line);
        src = strtok(temp, " \r\t\n");

        while (src != '\0') { col_num++; src = strtok(NULL, " \r\t\n"); }
        if (col_num!=size.column_size){
            fprintf(stderr,"the matrix is not square in %d rows\n",size.row_size);
            exit(1);
        }
    }
    return size;
}

channelT mchannel::ReadChannelMatrix(FILE* fp, int RowNum,int ColNum){
    int i;

    int row;
    int column;
    char line[LINE_SIZE], temp[LINE_SIZE], *src;
    int** matrix;

    /*************  alloction memory  ***************/
    matrix = (int**)calloc(RowNum, sizeof(int*));
    if (!matrix)fprintf(stderr, "memory alloction fail!\n");
    for (i = 0;i < RowNum;i++) {
        matrix[i] = (int*)calloc(ColNum, sizeof(int));
        if (!matrix[i])fprintf(stderr, "memory alloction fail!\n");
    }

    for (row = 0;row < RowNum;row++) {
        if(fgets(line, LINE_SIZE, fp)==NULL)
          fprintf(stderr,"fgets wrong");
        strcpy(temp, line);
        src = strtok(temp, " \r\t\n");
        for (column = 0;column<ColNum; column++) {
            matrix[row][column] = atoi(src);
            src = strtok(NULL, " \r\t\n");
        }
    }
    return matrix;
}

void mchannel::FillTips(channelT channel,sizeT size){
    int row_index;
    int col_index;

    for(row_index=0;row_index<size.row_size;row_index++){
        for(col_index=0;col_index<size.column_size;col_index++){
            if(channel[row_index][col_index]>10&&channel[row_index][col_index]<20){
                channel[row_index][col_index]=0;
            }
        }
    }
}

void mchannel::PrintMaxtrix(channelT channel,sizeT size){
    int row_index;
    int col_index;

    for (row_index = 0;row_index < size.row_size;row_index++) {
        for (col_index = 0;col_index < size.column_size;col_index++) {
            fprintf(stderr,"%d	", channel[row_index][col_index]);
        }
        fprintf(stderr,"\n");
    }
}
