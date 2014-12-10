/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
vHeap Class File
November 29, 2012
************************************/
#include <iostream>
#include "vHeap.h"
using namespace std;

vHeap::vHeap(){
	heap.clear();
}

int vHeap::Left(int index) {
	return ((index << 1) + 1); //(2k+1)
}

int vHeap::Right(int index) {
	return ((index << 1) + 2);//(2k+2)
}

int vHeap::Parent(int index) {
	return ((index - 1) >> 1);//(x-1)/2
}

int vHeap::hSize(){
	return heap.size();
}

void vHeap::Swap(Vertex& a, Vertex& b) {
	Vertex temp = a;
	a = b;
	b = temp;
}

int vHeap::SwapWithChild(int index) {
	int iSmallest=index;
	int iLeft	= Left(index);
	int iRight	= Right(index);
	int size = heap.size();
	if(iRight < size){
		if(heap[iLeft].shortest > heap[iRight].shortest){
			iSmallest = iRight;
		}
		else{
			iSmallest = iLeft;
		}
		if(heap[index].shortest < heap[iSmallest].shortest){
			iSmallest = index;
		}
	}
	else if(iLeft < size){
		if(heap[index].shortest > heap[iLeft].shortest){
			iSmallest = iLeft;
		}
	}
	if (heap[index].shortest > heap[iSmallest].shortest) {
		Swap(heap[index], heap[iSmallest]);
	}
	return iSmallest;
}

Vertex vHeap::RemoveRoot() {
	Vertex root = heap[0];
	if(heap.size()>1){
		heap[0] = heap[heap.size() - 1];
		heap.pop_back();
		int last = 0;
		int i = SwapWithChild(0);
		while (i != last) {
			last = i;
			i = SwapWithChild(i);
		}
	}
	else
		heap.pop_back();
	return root;
}

int vHeap::SwapWithParent(int i){
	if (i < 1)
		return i;
	int iParent = Parent(i);
	if(heap[i].shortest < heap[iParent].shortest) //switch <
	{
		Swap(heap[i], heap[iParent]);
		return iParent;
	} 
	else 
		return i;
}

void vHeap::AddElement(Vertex newElement){
	heap.push_back(newElement);
	int last = heap.size()-1;
	int i = (SwapWithParent(last));
	while(last != i)
	{
		last = i;
		i = SwapWithParent(i);
	}
}

void vHeap::OutputArray() {
	int hIndex=heap.size()-1;
	for (int i = 0; i < hIndex; ++i) {
		cout << heap[i].shortest << "  ";
	}
	cout << endl;
}