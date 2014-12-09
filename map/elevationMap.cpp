/************************************
Jeremiah Sheehy C09229858
EEN 318 - Dr. Murrell
United States Directions
December, 9, 2012
************************************/
#include "library.h"
#include "hashTable.h"
#include "vHeap.h"
#define PI 3.14159265

struct CoverageInfo{
	int latTop;
	int latBot;
	int longLeft;
	int longRight;
	char fileName[18];
};

struct FileData{
	string fileName;
	int rows;
	int cols;
	int bpp; //bytesperpixel
	int spp; //secondsperpixel
	int lls; //leftlongseconds
	int tls; //toplatseconds
	int min;
	int max;
	int sv; //special value marine code=-500
	short *d;//data
};

struct RoadSection{
	char name[32];
	char type[4]; //"T--" highway, "L--" Limited access highway, "F-T" Toll-Ferry Crossing
	int locIndexA;
	int locIndexB;
	float length; //.100=6.9miles
	char c;
};

void init_hashTable(HashTable*);//Create HashTable
void read_alphaplace_data(string, HashTable*);//read data from alphaplaces file and store in hashtable
void extractData(entry*, string);//extract data from 1 line of alphaplaces file and store it into an entry strucute
void lookUp(HashTable*,entry**);//look up 2 places form the hashtable and store them into a new entry structure
string getHashable(string);//takes a string and returns the hash value
string readLine(void);//reads a line from the stdin

char *get_coverage_file(float*, float*);//finds the minimum file that covers both places
void read_file(FileData *); //read binary data from elevation file
void load_locations_and_roads(FileData *,Vertex *,RoadSection *);//load locations and roads, update Current location and desitnation indices

int* get_color_map();//creates a color pallet
void draw_map(FileData*, int*);//draws the map 

void find_shortest_path(Vertex*, RoadSection*, int*, bool);//find shortest path between two places (Using indexes from locations.txt file)
void draw_shortest_path(Vertex*, RoadSection*, int*);//draws the shortest path
void print_directions(Vertex*, vector<RoadSection*>);//prints directions of shortest calculated path
string get_compas_direction(float); //converets degree direction to compas direction

void main(){
	int tableSize=25375;//number of lines in alphaplaces.txt file
	HashTable hTable(tableSize);//create hashtable 
	init_hashTable(&hTable);

	entry *places[2]; //structure for start and end location entries, defined in hashTable.h
	lookUp(&hTable,places);
	
	vector<CoverageInfo>dFileList;
	float x[2]={places[0]->longitude, places[1]->longitude};//put longitude data into x cordinate structure
	float y[2]={places[0]->latitude,  places[1]->latitude};//put latitude data into y coordinate structure
	int locIndices[2]={places[0]->closestLocI,places[1]->closestLocI};//closest location indices from closests.txt file

	char coverageFile[18];//Name of file that minimally covers both locations
	strcpy(coverageFile,get_coverage_file(x,y));//find the file that covers both and return the name as a char *

	//string s="geo/";
	//s+=coverageFile;	
	string s = coverageFile;
	FileData *fd = new FileData;
	fd->fileName=s;
	read_file(fd);

	//Location locations[29147]; //number of lines in locations.txt file
	RoadSection sections[47015]; //number of lines in majorroads.txt file
	Vertex v[29147];//array of vertices with pointers to locations (defined in vHeap.h)
	load_locations_and_roads(fd,v,sections); //extract location and road section info

	char yesOrNo;
	bool drawPaths=false;
	cout << "Do you want to watch the sortest path search? (y/n): ";
	cin >> yesOrNo;
	if(toupper(yesOrNo)=='Y')
		drawPaths=true;

	hide_console();
	system("cls");
	cout << "Location: \t" << places[0]->name << ", " << places[0]->state << endl;
	cout << "Destination: \t" << places[1]->name << ", " << places[1]->state << endl;
	cout << "-----------------------------------------------------------------\n";

	int * colormap=get_color_map();
	make_window(fd->cols,fd->rows);
	set_caption("United States Directions -- Jeremiah Sheehy");
	draw_map(fd,colormap);

	set_pen_width_color(5,color::black);
	draw_point(v[locIndices[0]].l->x,v[locIndices[0]].l->y);
	draw_point(v[locIndices[1]].l->x,v[locIndices[1]].l->y);

	find_shortest_path(v,sections,locIndices,drawPaths);

	system("PAUSE");
	exit(1);
}//end main

void init_hashTable(HashTable *hashT){
	string fileName;
	ifstream file;
	file.open("alphaplaces.txt",ios::in);
	if(file.fail())			
		fileName="/home/www/geographical/alphaplaces.txt"; //use absoulte path on rabbit
	else
		fileName="alphaplaces.txt"; //file is in working directory, use relative path
	file.close();
	read_alphaplace_data(fileName,hashT);
}//end init_hashTable

void read_alphaplace_data(string fileName, HashTable *hashT){
	cout << "Loading alphaplaces data..." << endl;
	ifstream inputFile;
	inputFile.open(fileName.c_str(),ios::in);
	if(inputFile.fail()){
		cerr << "Failed to open alphaplaces file.\n";
		exit(1);
	}
	FILE *fclosest = fopen("closests.txt", "r+");
	if (fclosest == NULL){
		cerr << "Closest file error.\n";
		exit(1);
	}
	string line;
	while(!inputFile.eof()){
		getline(inputFile,line);
		if(!line.empty()){
			entry *e = new entry;
			extractData(e,line);//extract data from alphaplaces.txt file
			int n = fscanf(fclosest, "%d %f",&e->closestLocI,&e->distanceToLocI);//extract data from closests.txt ifle
			if(n==EOF){
				fclose(fclosest);
			}
			hashT->insert(e);
		}
	}
	inputFile.close();
}//end read_alphaplace_data

void extractData(entry *e, string line){
	e->state=line.substr(0,2);
	e->stateFIP=line.substr(2,2);
	e->placeFIP=line.substr(4,5);
	int pos=9; //pos = line position
	e->name="";
	while(line[pos]<=47 || line[pos]>57){//char is not a digit (0-9)
		if(line[pos]>0){
				if(!(isspace(line[pos]) && isspace(line[pos+1])))
					e->name+=line[pos];
		}
		else
			e->name+=line.substr(pos,1); //for special characters, cctpye functions dont work for negative char values
		pos++;
	}
	//pos = start of first digit of population
	e->population=atoi(line.substr(pos,9).c_str());
	pos+=9; 
	e->housingUnits=atoi(line.substr(pos,9).c_str());
	pos+=9;
	e->landAreaMeters=atoi(line.substr(pos,14).c_str());
	pos+=14;
	e->waterAreaMeters=atoi(line.substr(pos,14).c_str());

	pos=143; //latitude marker always starts at 143
	if(line[pos]=='-'){ //'-' denotes [S]outh
		e->lati='S';
	}
	else
		e->lati='N';
	pos++;
	e->latitude=atof(line.substr(pos,8).c_str());

	pos=153;
	while(!isdigit(line[pos+1]))//move position to longitude marker
		pos++;
	if(line[pos]=='-'){ //'-' denotes [W]est
		e->longi='W';
	}
	else
		e->longi='E';
	e->longitude=atof(line.substr(pos,8).c_str());

	e->hashString=getHashable(e->state+e->name);//generates string to be used for hashing: state abreviation+place name ALL CAPS.
}//end extractData

void lookUp(HashTable *hashT, entry **places){
	string abrev="",str="",hashStr="";
	int counter=0;
	while(counter<2){
		cout << "Place [" << (counter+1) << "] "<< "Enter State Abreviation: ";
		abrev=readLine();
		cout << "Enter Place Name: ";
		str=readLine();
		if(str=="done" || abrev=="done"){
			system("PAUSE");
			exit(1);
		}
		else{
			hashStr=getHashable(abrev+str);//get hash string of place entered by user
			entry *e=hashT->getEntry(hashStr);//get entry assosciated with that hash string
			if(e!=NULL){
				places[counter]=e;
				counter++;
			}
		}
	}
}//end lookUp

string getHashable(string str){
	int pos=0;
	string hashStr="";
	while(pos<str.length()){
		if(str[pos]>0){ //toupper doesn't work on special chars, so they are left out of the hashString
				if(!isdigit(str[pos]) && !isspace(str[pos]))
					hashStr+=toupper(str[pos]);
		}
		pos++;
	}
	return hashStr;
}//end getHashable

string readLine(){
	string r="";
	while (true){
		int c = getc(stdin);
		if (c=='\n' || c==EOF)
			break;
		r+=(char)c;
	}
	return r; 
}//end readLine

char *get_coverage_file(float* x, float * y){
	float xMin, xMax, yMin, yMax;
	if(x[0]<x[1]){
		xMin=x[0];xMax=x[1];
	}else{
		xMin=x[1]; xMax=x[0];
	}
	if(y[0]<y[1]){
		yMin=y[0]; yMax=y[1];
	}else{
		yMin=y[1]; yMax=y[0];
	}

	FILE *f = fopen("coverage.txt", "r+"); //"r+" for reading
	if (f == NULL){
		cerr << "Can't create coverage file.\n";
		exit(1);
	}

	int numFiles=0;//number of files that cover both locations
	float distance_x=0, distance_y=0;//dimensions of the smallest file covering both locations
	char fName[18];//FileData name assosciated with the file

	while(true){
		CoverageInfo ci;//struct to hold data from coverage.txt file
		int n = fscanf(f,"%d %d %d %d %s", &ci.latTop, &ci.latBot, &ci.longLeft, &ci.longRight, ci.fileName);//load data into struct
		if(n==EOF)
		{
			fclose(f);
			break;
		}
		if(xMin>ci.longLeft && xMax<ci.longRight && yMin>ci.latBot && yMax<ci.latTop){
			numFiles++;
			//for first file found set inital dimensions, otherwise compare new dimensions to the previous best, update if necessary
			if(numFiles==1 || abs(ci.longLeft-ci.longRight)*(ci.latTop-ci.latBot)<(distance_x*distance_x)){
				distance_x=abs(ci.longLeft-ci.longRight);
				distance_y=(ci.latTop-ci.latBot);
				strcpy(fName,ci.fileName);//update fileName
			}
		}
	}
	if(numFiles>0)//at least one file was found
		return fName;
	else{
		cout << "NO COVERAGE FILE COVERS BOTH LOCATIONS" << endl;
		system("PAUSE");
		return "NULL";
	}
}//end get_coverage_file

void read_file(FileData * fd){
	FILE *fdat = fopen(fd->fileName.c_str(), "rb"); //"rb" for reading binary
	if (fdat == NULL){
		cerr << "Can't create file.\n";
		exit(1);
	}
	char h[16];//holder
	int n = fscanf(fdat,"%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d", h, &fd->rows, h, &fd->cols, h, &fd->bpp, h, &fd->spp, h, &fd->lls, h, &fd->tls, h, &fd->min, h, &fd->max, h, &fd->sv);
	if(n!=18)
		cerr << "Meta Data extraction error.\n";
	short *data = new short[fd->cols*fd->rows];//structure to hold the short int data
	fseek(fdat,sizeof(short)*fd->cols,SEEK_SET); //seek to the start of data
	fread(data,sizeof(short),fd->cols*fd->rows,fdat);//extract the data
	fclose(fdat);
	fd->d=data;//store data in FileData structure
}//end read_file

void load_locations_and_roads(FileData *fd, Vertex *v, RoadSection * sections){
	float leftLong=fd->lls, topLat=fd->tls, secpp=fd->spp, x=0 ,y=0;
	string str="";

	FILE *floc = fopen("locations.txt", "r+");
	if (floc == NULL){
		cerr << "Can't create file.\n";
		exit(1);
	}
	int i = 0;
	while(true){
		Location *l = new Location;
		int n = fscanf(floc, "%f %f %f %d %s %s %[^\n]", &l->longitude, &l->latitude, &l->distanceTo, &l->pop, l->type, l->abv, l->name);//extract necessary data from file
		if(n==EOF){
			fclose(floc);
			break;
		}
		//calculate grid cords given long/lat cords and map info from filedata
		x=l->longitude*3600, 
		y=l->latitude*3600;
		x=abs((leftLong-x)/(secpp)); 
		y=abs((topLat-y)/(secpp));
		l->x=x; l->y=y;
		v[i].l=l;//assign location point to vertex
		v[i].index=i;//assign location index to vertex
		i++;
	}

	FILE *froad = fopen("majorroads.txt", "r+");
	if (froad == NULL){
		cerr << "Can't create file.\n";
		exit(1);
	}
	i = 0;
	while(true){
		int n = fscanf(froad, "%s %s %d %d %f", sections[i].name, sections[i].type, &sections[i].locIndexA, &sections[i].locIndexB, &sections[i].length);//extract necessary data from file
		if(n==EOF){
			fclose(froad);
			break;
		}
		i++;
	}
}//end load_locations_and_roads

int*get_color_map(){
	int cmap[4049]; //Heighest elevation is 4048m
	int i = 0, pos=0; //i is the iterator, pos is the array starting array index for eahc loop
	double r=0,g=0,b=0; //red, green, blue
	while(i<=152){
		r=0.0; g=.25+.75*(i/152.0); b=0.0;
		cmap[pos+i]=make_color(r,g,b);
		i++;
		//r=0.0 g=1.0 b=0.0 (Dark Green)
	}
	pos+=i;
	i=0;
	while(i<=152){
		r=.66*(i/152.0); g=1.0; b=.184*(i/152.0);//interpolating colors
		cmap[pos+i]=make_color(r,g,b);
		i++;
		//r=.66 g=1.0 b=.184 (Green-Yellow)
	}
	pos+=i;
	i=0;
	while(i<=304){
		r=.66+.34*(i/304.0); g=1.0; b=.184-.184*(i/304.0);
		cmap[pos+i]=make_color(r,g,b);
		i++;
		//r=1.0 g=1.0 b=0.0 (Yellow)
	}
	pos+=i;
	i=0;
	while(i<=912){
		r=1.0; g=1.0-.33*(i/912.0); b=0.0;
		cmap[pos+i]=make_color(r,g,b);
		i++;
		//r=1.0 g=.66 b=.0.0 (Orange)
	}
	pos+=i;
	i=0;
	while(i<=1523){
		r=1.0-.46*(i/1523.0); g=.66-.39*(i/1523.0); b=.074*(i/1523.0);
		cmap[pos+i]=make_color(r,g,b);
		i++;
		//r=.54 g=.27 b=.074 (Brown)
	}
	pos+=i;
	i=0;
	while(i<=1000){
		r=.54+.205*(i/1000.0); g=.27+.475*(i/1000.0); b=.074+.671*(i/1000.0);
		cmap[pos+i]=make_color(r,g,b);
		i++;
		//r=.745 g=.745 b=.745 (Gray)
	}
	return cmap;
}//end get_color_map

void draw_map(FileData *fd,int *cmap){
	int cols = fd->cols, rows=fd->rows;
	int col=0,row=0;//iterators for drawing pixels at (row,col)
	short *data = fd->d;
	for(int i = 0; i < (cols*rows); i++){
		//my data is stored in a 1-dimensional array so the row counter needs to be incremented at multiples of the total column length
		if(i%cols==0){
			col=0;
			row++;
		}
		if(data[i]==-500 || data[i]==fd->sv)//-500 is special value for water
			set_pen_color(color::light_blue);
		else{
			if(data[i]<0)//there shouldn't be any negative data
				data[i]=0;
			if(data[i]>4048)//4048 should be heighest elevation possible
				data[i]=4048;
			set_pen_color(cmap[data[i]]);//loads color from pallet to match elevation data		
		}
		draw_point(col,row);
		col++;
	}
	return;
}//end draw_map

void find_shortest_path(Vertex *v, RoadSection *secs, int*indices, bool drawPaths){
	int A=indices[0], B=indices[1];
	vHeap h;//min-heap structure

	set_pen_width_color(1,color::purple);//for drawing roads while searching	
	v[A].shortest=0;//Set starting location shortest path value to 0.
	v[A].exp=true;//Set starting location explored value to true
	int index = A;//Set current index to starting location
	while(v[B].exp==false){
		for(int i=0; i<47015; i++){
			if(secs[i].locIndexA==index && v[secs[i].locIndexB].exp==false){ 
				int j=secs[i].locIndexB;
				if(v[index].shortest+secs[i].length<v[j].shortest){
					v[j].shortest=v[index].shortest+secs[i].length;//update shortest distance to vertex
					v[j].prev=index;//update previous vertex
					if(drawPaths){
						move_to(v[index].l->x,v[index].l->y);
						draw_to(v[j].l->x,v[j].l->y);
					}
					h.AddElement(v[j]);//add vertex to heap
				}
			}
			if(secs[i].locIndexB==index && v[secs[i].locIndexA].exp==false){
				int k=secs[i].locIndexA;
				if(v[index].shortest+secs[i].length<v[k].shortest){
					v[k].shortest=v[index].shortest+secs[i].length;//update shortest distance to vertex
					v[k].prev=index;//update previous vertex
					if(drawPaths){
						move_to(v[index].l->x,v[index].l->y);
						draw_to(v[k].l->x,v[k].l->y);
					}
					h.AddElement(v[k]);//add vertex to heap
				}
			}
		}
		if(h.hSize()>=1){
			index=h.RemoveRoot().index;//pop vertex with shortest path from heap
			while(v[index].exp==true && h.hSize()>=1){//make sure that vertex hasn't already been fully explored and hash table isn't empty
				index=h.RemoveRoot().index;
			}
			v[index].exp=true;//set current vertex index to sortest fully explored, this will be used next
		}else{
			cout << "HEAP ERROR -- NO PATH" << endl;
			system("PAUSE");
			exit(1);
		}
	}
	draw_shortest_path(v,secs,indices);
}//end shortest_path

void draw_shortest_path(Vertex *v, RoadSection* secs, int *indices){
	int A=indices[0], B=indices[1];
	int loc1=v[B].index, loc2=v[B].prev;
	
	vector<RoadSection *> roadList;
	set_pen_width_color(2,color::red);
	//go backwards from B to the previous index, repeat until we get to A
	while(loc2!=-1){
		for(int q=0; q < 47015; q++){
				if((secs[q].locIndexA==loc1 && secs[q].locIndexB==loc2)){
					secs[q].c='B';//remember we arrived at this road at index B
					roadList.push_back(&secs[q]);
					break;
				}
				if((secs[q].locIndexB==loc1 && secs[q].locIndexA==loc2)){
					secs[q].c='A';//remember we arrived at this road at index A
					roadList.push_back(&secs[q]);
					break;
				}
		}
		move_to(v[loc1].l->x,v[loc1].l->y);
		draw_to(v[loc2].l->x,v[loc2].l->y);
		loc1=loc2;//move back one road and repeat
		loc2=v[loc2].prev;
	}	
	print_directions(v,roadList);
}//end draw_shortest_path

void print_directions(Vertex *v,vector<RoadSection *>final){
	show_console();
	float distance = 0, dt=0, degree=0, x0=0, x1=0, y0=0, y1=0;
	int a=0, b=0;
	bool newRoad=true;
	//start at end since we compiled our roadList from B to A
	for(int i = final.size()-1; i>=0; i--){
		distance+=final[i]->length;//update distance traveled on current road
		if(newRoad==true){
			if(final[i]->c=='A')//if road section started at locIndexA, we want locIndexA for the start point
				a=final[i]->locIndexA;
			else
				a=final[i]->locIndexB;
		}

		//if there is at least 2 roads to compare and those roads have the same name, nothing needs to be done
		if((i-1)>=0 && strcmp(final[i]->name,final[i-1]->name)==0){
			newRoad=false;
		}else{
			if(final[i]->c=='A')//if road section started at locIndexA, we want locIndexB for the end point
				b = final[i]->locIndexB;
			else
				b = final[i]->locIndexA;
			x0=v[a].l->longitude; y0=v[a].l->latitude;
			x1=v[b].l->longitude; y1=v[b].l->latitude;
			degree=atan2(y1-y0,x1-x0)*180.0/PI;//convert from lat/longitude cords to a direction in degrees
			if(degree<0)
				degree=degree+360.0;//get rid of negative values
			
			cout << "Take " << final[i]->name << "\nHead " << get_compas_direction(degree) << "\t for " << distance*69.0 << " miles" << endl;
			cout << "-----------------------------------------------------------------\n";
			dt+=distance;//update total distance
			distance=0;//reset distance traveled on current road to 0
			newRoad=true;//set new road to true
		}
	}
	cout << "TOTAL DISTANCE:\t     " << dt*69.0 << " miles " << endl;
}//end print_directions

string get_compas_direction(float d){
	//separate 360 degrees into 8 compas directions, each covers 45 degrees.
	string str="";
	if((d>=0.0 && d<22.5) || (d>= 337.5 && d<=360.0))
		str="East";
	else if(d>=22.5 && d<67.5)
		str="North East";
	else if(d>=67.5 && d < 112.5)
		str="North";
	else if (d>=112.5 && d<157.5)
		str="North West";
	else if(d>=157.5 && d<202.5)
		str="West";
	else if(d>=202.5 && d<247.5)
		str="South West";
	else if(d>=247.5 && d<292.5)
		str="South";
	else if (d>=292.5 && d < 337.5)
		str="South East";
	return str;
}//end get_compas_direction
