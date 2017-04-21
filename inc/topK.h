#ifndef TOPK_H
#define TOPK_H
#include<iostream>
using std::iostream;
class topK{
public:
    topK(){}

    void maxHeapify(double* array, const int size, int i){
        int left =2*i+1;
        int right=2*i+2;
        int largest=i;
        double temp;

        if((left<size)&&(array[left]>array[i])){
            largest=left;
        }

        if((right<size)&&(array[right])>array[largest]){
            largest=right;
        }

        if(largest!=i){
            temp=array[largest];
            array[largest]=array[i];
            array[i]=temp;
            maxHeapify(array, size, largest);
        }
    }

    void buildMaxHeap(double* array, const int size){
        int index=size/2-1;
        for(int i=index; i>=0; i--){
            maxHeapify(array,size,i);

            for(int i=0;i<5;i++){
                std::cout<<array[i]<<' ';
            }
        }
    }

    void heapSort(double* array, const int size){
        double temp=0;
        int mySize=size;
        buildMaxHeap(array,size);

        for(int i=size-1;i>0;i--){
            //swap
            temp=array[i];
            array[i]=array[0];
            array[0]=temp;

            mySize=mySize-1;
            maxHeapify(array,mySize,0);
        }

    }

    void minimunN(double* array, const int length, const int n){
        double temp=0;
        buildMaxHeap(array,n);

        // compare the rest number with the top number in heap
        for(int i=n;i<length;i++){
            if (array[i]<array[0]){
                //swap
                temp=array[0];
                array[0]=array[i];
                array[i]=temp;
                maxHeapify(array,n,0);
            }
        }
    }

    template <typename T> void swap( T&a, T&b){
        T temp;
        temp=a;
        a=b;
        b=temp;
    }

};

#endif // TOPK_H
