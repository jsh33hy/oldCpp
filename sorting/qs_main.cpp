/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
QuickSort Implementation (Ascending Order)
September 12, 2012
************************************/
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

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
int pickPivot(int&,int&,int&);
void quickSort(int*,int*);
void insertionSort(int*,int);

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
			int data[size];
			fillData(0,size-1,data,size);//fill data with random numbers ranging from [0,size-1]
			printData(data,size);//show data before sorting
			quickSort(data,&data[size-1]);//sort
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
			quickSort(data,&data[size-1]);
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

int pickPivot(int&a,int&b,int&c){
	if(a>b){
		if(b>c)//a>b>c
			swap(a,c);
		else{
			//a>c>b double swap or c>a>b single swap
			if(a>c)			
				swap(a,c);		
			swap(a,b);
		}
	}
	else{
		if(b>c){
			//b>a>c double swap or b>c>a single swap
			if(a>c)
				swap(a,c);
			swap(b,c);
		}
	}
	//sortd c>b>a
	return b;
}//end pickPivot

void quickSort(int *start, int *end){
	int size =((end-start)+1); //find size of array or partition
	if(size>=3){
		int *head=start;
		int *tail=end;
		int pivot = pickPivot(*head,*(head+(size/2)),*tail); //find median of 3 and put them in correct spots
		int *lptr=head+1; //less than pointer
		int *gptr=head+1; //greater than pointer
		while(gptr!=tail){
			if(*gptr<=pivot){
				if(gptr!=lptr)
					swap(*gptr,*lptr); //if they dont point at the same memory location, swap values
				lptr++;
			}
			gptr++;
		}
		quickSort(lptr,gptr); //recursive call to sort partition [lptr,gptr]
		quickSort(start,--lptr);//recursive call to sort partition [start,lptr) lptr no included
	}
	if(size==2) //case for 2 elements
		if(*start>*end)
			swap(*start,*end);			
	return;
}//end quickSort

void insertionSort(int x[],int length)//entry d, int data size
{
  int key,i; //node key, it i
  for(int j=1;j<length;j++)
  {
     key=x[j];//newNode->info = d(j)
     i=j-1;//i = (j-1)
     while(x[i]>key && i>=0) //while d(i)>key ad i<=0
     {
               x[i+1]=x[i];//insert aftter 
         i--;
     }
     x[i+1]=key;
  }
}//end insertion

