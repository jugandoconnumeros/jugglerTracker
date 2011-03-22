/*
 *  blobTracker.h
 *  jugglingTracker
 *
 *  Created by adminmp on 11/03/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVec2f.h"

#include "trackedBlob.h"

class blobTracker {
	
public:
	
	blobTracker();
	
	~blobTracker();
	
	void setup(int timeToLiveMs);
	void resetCounter();
	
	//needs to be called before you can check trackedBlobs
	void begin();
	
	bool matchTrackedBlob(trackedBlob & trackedBlobIn, int maxDist, float activityThresh = 2.0, float cacheRate = 0.85); //this returns true if the trackedBlob matches - it also updates the trackedBlob
	int addBlob(trackedBlob & trackedBlobIn);  //this will assign a new id
	
	//once you are done checking trackedBlobs
	//this kills trackedBlobs that need to die!
	void end();
	
	
	int numStoredBlobs;
	
	
	//protected:
	int width;
	int height;
	int lastNumFound;
	int genID;
	int lastTime;
	int deathTime;
	
	vector <trackedBlob> storedBlobs;
	
};