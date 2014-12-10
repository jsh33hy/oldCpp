/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
MergeSort Implementation (Ascending Order)
September 14, 2012
************************************/
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;
int count = 0;
const int sys_unix=1, sys_windows=2, sys_unknown=0;
#if defined(unix)
	const int sys = sys_unix;
	void randomize(){
		srandomdev();
	}
	int random_in_range(int a, int b){
		if (b<a)
			return random()%(a-b+1)+b;
		else
			return random()%(b-a+1)+a; 
	}
#elif defined (_WIN32)
	const int sys = sys_windows; //using sys, "system" is defined as a function in windows
	void randomize(){
		srand(time(NULL));
	}
	int random_in_range(int a, int b){
		if (b<a)
			return rand()%(a-b+1)+b;
		else
			return rand()%(b-a+1)+a; 
	}
#else
	const int sys = sys_unknown;
#endif

void printProgramInfo(int, char **);
void fillData(int,int,int*,int);
void printData(int*,int);
void swap(int&,int&);
void mergeSort(int*,int,int);
void merge(int*,int,int);

int main(int argc, char * argv[]){
	printProgramInfo(argc,argv);
	if(argc<2){
		cout << "No data size given.\n";
		cout << "Use " << argv[0] << " ######## (number of elements to sort)\n" << endl;
		cout << "Use " << argv[0] << " \"test\" (without quotes) to see before and after for 15 elements.\n";
	
	}
	else{
		randomize(); //seeds rand() or random() depending on system
		if(strcmp(argv[1],"test")==0){
			const int size=15;	
			int *data = new int[size];
			fillData(0,size-1,data,size);//fill data with random numbers ranging from [0,size-1]
			printData(data,size);//show data before sorting
			mergeSort(data,0,size-1);//sort
			printData(data,size);//show data after
		}
		else{
			cout << argv[1] << " Elements" << endl;
			string str = argv[1];
			const int size=atoi(argv[1]);
			if(size==0){
				cout << "Command line arguments not a number. Goodbye.";
				exit(0);
			}
			int *data = new int[size];//create an array of the specified size from CLA
			fillData(0,size-1,data,size);//fills array with random data
			cout << "Sort Started" << endl;
			mergeSort(data,0,size-1);
			cout << "Done" << "Time Elapsed: "<< endl;			
		}
	}
	exit(0);
}//end main

void printProgramInfo(int argc, char * argv[]){
	for(int i = 0; i < argc; i++)
		cout << argv[i] << " ";
	cout << "\nRunning on ";
	if (sys==sys_unix)
		cout << "a Unix ";
	if (sys==sys_windows)
		cout << "a Windows ";
	if (sys==sys_unknown)
		cout << "an Unknown ";
	cout << "System.\n";

	//Print time and date
	time_t t;
	time(&t);
	struct tm * lt = localtime(&t);
	cout << lt->tm_hour << ":" << lt->tm_min << ":" << lt->tm_sec << '\t'; //Hour:Minute:Second [TAB]
	cout << lt->tm_mon+1 << "/" << lt->tm_mday << "/" << lt->tm_year+1900 << endl; // Month/Day/Year
	cout << "--------------------------\n";
	return;
}//end printProgramInfo

void fillData(int lowerBound, int upperBound, int * data, int size){
	for(int i = 0; i < size; i++)
		data[i]=random_in_range(lowerBound,upperBound);
	return;
}//end fillData

void printData(int * data, int size){
	for(int i = 0; i < size; i++)
		cout << "[" << i << "] : " << data[i] << endl;
	cout << "END" << endl;
	return;
}//end printData

void swap(int&a, int&b){
    int temp = a;
    a = b;
    b = temp;
}//end swap

void mergeSort(int*data,int start,int end){
	if((end-start)>1){
		int mid = start+((end-start+1)/2);
		mergeSort(data,start,(mid-1));
		mergeSort(data,mid,end);
		merge(data,start,end);
	}
	else if((end-start)==1){
			if(data[start]>data[end])
				swap(data[start],data[end]);
	}
	return;
}//end mergeSort

void merge(int*data,int start,int end){
	int size=(end-start)+1;
	int mid=start+size/2;
	int li=start, ri=mid, i=0;
	int *sortedData=new int[size];
	while(li<mid && ri<=end){
		if(data[li]<data[ri]){
			sortedData[i]=data[li];
			li++;			
		}
		else{
			sortedData[i]=data[ri];
			ri++;
		}
		i++;
	}
	while(li<mid){
		sortedData[i]=data[li];
		li++; i++;
	}
	while(ri<=end){
		sortedData[i]=data[ri];
		ri++; i++;
	}
	for(int x = start; x <= end; x++)
		data[x]=sortedData[x-start];

	delete[] sortedData;
	return;
}//end merge
