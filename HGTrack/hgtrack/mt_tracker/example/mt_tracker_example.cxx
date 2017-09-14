/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */

#include <hgtracker/hgtracker.h>

int main(int argc, char* argv[])
{
	//char* pConfig = argv[1];
	char* pConfig = "GraphMultiCarTracker_Config.txt";
	CHyperGraphTracker* psgtracker = new CHyperGraphTracker(pConfig);
	psgtracker->Tracker_API();

	if (psgtracker) {
		RELEASE(psgtracker);
		psgtracker = 0;
	}

	//system("pause");
	return 0;
}