#include "Channel.h"

void Channel::read_from_file(size_t N_h, size_t N_w, 
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
}

void Channel::correct()
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
