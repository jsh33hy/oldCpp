/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
HashTable Class File
October, 4, 2012
************************************/
#include "hashTable.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

HashTable::HashTable(){
	size=2;
	elements=0;
	ops=0;
	table=new chain[size];
}//end HashTable empty constructor

HashTable::HashTable(int s)
{	
	size = s;
	elements=0;
	ops=0;
	table=new chain[size];
	for(int i = 0; i < size; i++){
		table[i].start=NULL;
		table[i].length=0;//added
	}
}//end HashTable contrusctor

int HashTable::hash(string str)
{
  	char c;
	unsigned int value=0;
	for(int i =0; i < str.length(); i++){
		c=str[i];	
		value+=(int)c;
		value*=111;
	}
	return value%size;
}//end hash

void HashTable::insert(entry *e){
	unsigned int val=0;

	node* newNode = new node;
	newNode->e=e;
	newNode->next=NULL;
	
	val = hash(e->hashString);
	if(table[val].start==NULL){
		table[val].start=newNode;
	}
	else{
		node *iter = new node;
		iter=table[val].start;
		while(iter->next!=NULL)
			iter=iter->next;
		iter->next=newNode;
	}
	table[val].length++;
	elements++;
}//end insert

void HashTable::printEntry(string hashStr){
	unsigned int index = hash(hashStr);
	node *iter = new node;
	iter=table[index].start;
	while(iter!=NULL && iter->e->hashString!=hashStr )
			iter=iter->next;
	if(iter==NULL)
		cout << "VALUE NOT IN TABLE" << endl;
	else{
		#if defined(unix)
		string deg="°";
		string squared="²";
		#else
			char deg=(int)248;
			char squared=(int)253;
		#endif
		entry *e = iter->e;
		cout << "Place Name:\t\t" << e->name << endl;
		cout << "State Abreviation:\t" << e->state << endl;
		cout << "State *FIPS code:\t" << e->stateFIP << endl;
		cout << "Place *FIPS code:\t" << e->placeFIP << endl;
		cout << "Population (2000):\t" << (long)e->population << endl;
		cout << "Housing Units (2000):\t" << e->housingUnits << endl;
		cout << "Land Area (meters):\t" << (long)e->landAreaMeters << 'm' << squared << endl;
		cout << "Water Area (meters):\t" << (long)e->waterAreaMeters << 'm' << squared << endl;
		cout << "Latitude (degrees):\t" << e->lati << " " << e->latitude << deg << endl;
		cout << "Longitude (degrees):\t" << e->longi << " " << e->longitude << deg << endl;
		//cout << "HASH STRING: " << e->hashString << endl;
		cout << "*FIPS=Federal Information Processing Standard" << endl;
		cout << endl;
	}
	
}//end printEntry

void HashTable::tester(){
	int max=0;
	for(int i = 0; i < size; i++){
		ops+=table[i].length;
		if(table[i].length>max)
			max = table[i].length;
	}

	int * data = new int[max+1];

	for(int i = 0; i < max+1; i++)
		data[i]=0;
	
	for(int i = 0; i < size; i++)
		data[table[i].length]++;

	cout << "Hash table size:\t" << size << endl;
	cout << "Number of elements:\t" << elements << endl;
	cout << "Table fullness: \t" << (double)elements/(double)size << endl;
	
	for(int i = 0; i < max+1; i++)
		cout << "# of lists size " << i << ": " << data[i] << endl;

	cout << "Avg list size:\t" << ops/(double)size << endl;
	cout << "Largest list:\t" << max << endl;
	
}//end tester

entry * HashTable::getEntry(string hashStr){
	unsigned int index = hash(hashStr);
	node *iter = new node;
	iter=table[index].start;
	while(iter!=NULL && iter->e->hashString!=hashStr )
			iter=iter->next;
	if(iter==NULL){
		cout << "VALUE NOT IN TABLE" << endl;
		return NULL;
	}
	else
		return (iter->e);
}//end getEntry
