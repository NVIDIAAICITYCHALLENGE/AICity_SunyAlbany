/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */

#include <iostream>
#include <string> //for getline()
#include <fstream> //for ifstream
#include <cstring> // for strtok_r strcpy
using namespace std;

#include "config.h"

//Config files
//Parameters initialization.

//Loading image name.
string G_INPUT_DATAFILE_NAME = "";
//Loading file path.
string G_INPUT_VIDEOFILE_PATH = "";
//Loading detection results path
string G_INPUT_DETECTFILE_PATH = "";


//Output file path.
string G_OUTPUT_VIDEOFILE_PATH = "";
string G_OUTPUT_SPEEDFILE_PATH = "";
string G_OUTPUT_TRAJFILE_PATH = "";

//ͼ���С����
int G_IMAGE_WIDTH = 320;
int G_IMAGE_HEIGHT = 240;


//Total frame number
int G_TOTAL_FRAME_NUMBER = 20;

//Car minimal size
int G_CAR_MIN_WIDTH = 5;
int G_CAR_MIN_HEIGHT = 5;

//Segment size, control the number of windows per-segment
int G_SEGMENT_SIZE[2] = {5,6};

//Number of tracklets in each hyperedge.
int G_TRACK_KARY = 3;


//--------------------------------------- Some Parameters Used for Debug -----------------------------------//
int G_DEBUG_MARK = 0;
string G_TRACKSIMAGEPATH_DEBUG = "";
string G_TARGETIMAGE_PATH_DEBUG = "";
string G_OCCUPYIMAGE_PATH_DEBUG = "";


bool LoadConfig(const char* configfile)
{
	cout<<"Loading Config File...."<<endl;

	ifstream fin(configfile);
	vector<string> cfgVariable;
	string Temp;

	//Useful variables.
	char* Seperate = ",";
	char* pCurrent;
	char* pNext = NULL;
	char pLine[1024] = "";

	int nCount = 0;

	if (!fin.fail()) {
		while(getline(fin, Temp)) {
			if (Temp.size() != 0 && Temp.data()[0] != '#') 
				cfgVariable.push_back(Temp);
		}
		fin.close();

		//Check the required variable values, If it is null, return false.
		if ((int)cfgVariable.size() == 0) {
			cout<<"Warning, The Config File Is Null, Please Check It!"<<endl;
			return false;
		}

		//get the variable.
		vector<string>::iterator iter = cfgVariable.begin();

		//###############################################################################################################//
		//##### Main Parameters ######
		//###############################################################################################################//
		//---------------------------- Database Information -------------------------------//
		//Loading image name.
		//The name of the data.
		G_INPUT_DATAFILE_NAME = *(iter++);
		//Loading file path.
		G_INPUT_VIDEOFILE_PATH = *(iter++);
		//Loading detection results path
		G_INPUT_DETECTFILE_PATH = *(iter++);

		G_OUTPUT_VIDEOFILE_PATH = *(iter++);
		G_OUTPUT_TRAJFILE_PATH = *(iter++);
		G_OUTPUT_SPEEDFILE_PATH = *(iter++);
		
		G_IMAGE_WIDTH = atoi((iter++)->data());
		G_IMAGE_HEIGHT = atoi((iter++)->data());

		G_TOTAL_FRAME_NUMBER = atoi((iter++)->data());

		G_CAR_MIN_WIDTH = atoi((iter++)->data());
		G_CAR_MIN_HEIGHT = atoi((iter++)->data());
		
		//Segment size of the video sequence
		string SegTemp;
		SegTemp = *(iter++);

		//Set the parameters for optimization.
		//YI:
		// strcpy_s not supported by linux, replace it with strcpy
		strcpy(pLine, SegTemp.data());
		//strcpy_s(pLine, 1024, SegTemp.data());
		// YI:
		// strtok_s is replaced by strtok in linux
		pCurrent = strtok(pLine, Seperate);
		G_SEGMENT_SIZE[0] = atoi(pCurrent);
		nCount = 1;
		while (1) {
			pCurrent = strtok(NULL, Seperate);
			if ((nCount >= 2)||(pCurrent == NULL))
				break;
			G_SEGMENT_SIZE[nCount] = atoi(pCurrent);
			nCount++;
		}
		//Check whether the parameters are correct or not.
		if (nCount < 2) {
			fprintf(stderr, "Errors, the number of parameter G_SEGMENT_SIZE is not enough!\n");
			return false;
		}

		//The degree variable of the hypergraph in tracking.	
		G_TRACK_KARY = atoi((iter++)->data());
	
		//###########################################################################################################//
		//-- Debug Information
		//##########################################################################################################//
		//--------------------------------------- Some Parameters Used for Debug -----------------------------------//
		//G_DEBUG_MARK = atoi((iter++)->data());
		//G_TRACKSIMAGEPATH_DEBUG = *(iter++);
		//G_TARGETIMAGE_PATH_DEBUG = *(iter++);
		//G_OCCUPYIMAGE_PATH_DEBUG = *(iter++);
		
		cout<<"Loading Config File Successfully!"<<endl;
		return true;
	}

	cout<<"Fail to Load the Config File!"<<endl;
	return false;
}

