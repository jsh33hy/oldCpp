/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
HashTable Header File
October 4, 2012
************************************/
#pragma once

#include <string>
using namespace std;

#ifndef HASHTABLE_H
#define HASHTABLE_H

struct entry{
	string state;//0-1
	string stateFIP; //2-3
	string placeFIP; //4-8
	string name; // 9-72
	string hashString;
	int population; //73-81
	int housingUnits; //82-90
	double landAreaMeters; //119-130
	double waterAreaMeters; //131-142
	char lati; //143
	double latitude; //143-152
	char longi; //153
	double longitude; //153-163
	int closestLocI;
	double distanceToLocI;
};

struct node{
	entry* e;
	node* next;
};

struct chain{
	int length;
	node* start;
	//could add an end node
};

class HashTable{
protected:
	int size;
	int elements;
	double ops;
	chain *table;
public:
	HashTable();
	HashTable(int);;
	int hash(string);
	void insert(entry *);
	void printEntry(string);
	void tester();
	entry *getEntry(string);
};
#endif
