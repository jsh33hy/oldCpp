/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
vHeap Header File
November 29, 2012
************************************/
#pragma once
#include <string>
#include <vector>
using namespace std;
#define INF 9999999999

struct Location{
	float longitude;
	float latitude;
	float distanceTo;
	int pop;//population
	char type[5];//type "city, town, ect."
	char abv[3];//place abreviation
	char name[64];//place name
	float x;//grid x-cord
	float y;//grid y-cord
};

struct Vertex{
	Location *l;//pointer to location
	float shortest; //shortest path from initial vertex
	int prev;//index of previous location
	int index;//index of itself
	bool exp;//true if fully explored, else false
	Vertex(){
		shortest=INF;
		prev=-1;
		index=0;
		exp=false;
	}
};

#ifndef VHEAP_H
#define VHEAP_H
//Min-heap Vertex
class vHeap{
protected:
	vector<Vertex> heap;
	int Left(int);
	int Right(int);
	int Parent(int);
	void Swap(Vertex&,Vertex&);
	int SwapWithChild(int iIndex);
	int SwapWithParent(int i);
public:
	vHeap();
	int hSize();
	void AddElement(Vertex newElement);
	Vertex RemoveRoot(void);
	void OutputArray();
};
#endif