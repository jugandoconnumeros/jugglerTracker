/*
 *  oscTrackingSender.h
 *  opencvExample
 *
 *  Created by Chris on 3/6/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"
#include "trackedBlob.h"
#include "blobTracker.h"

class OscTrackingSender{
	public:
	OscTrackingSender(){}
	~OscTrackingSender(){}
	
	void setup(string host, int port,float scaleMeBy = 1);
	void send( vector<trackedBlob> blobs1, vector<trackedBlob> blobs2, vector<trackedBlob> blobs3, vector<trackedBlob> blobs4);
	void addSender(string hostip);
	
	string host;
	int port;
	ofxOscSender sender;
	float scaleBy;
	
	vector<string> hosts;
	vector<ofxOscSender*> senders;
	
};
