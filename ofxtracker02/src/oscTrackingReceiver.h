/*
 *  oscTrackingReceiver.h
 *  opencvExample
 *
 *  Created by Chris on 3/6/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"

class OscTrackingReceiver{
public:
	OscTrackingReceiver(){}
	~OscTrackingReceiver(){}
	
	void setup(int port, float scaleMeUpBy = 1);
	void receive();
	void draw(float x=0, float y=0);
	
	int port;
	ofxOscReceiver receiver;
	float scaleBy;
	
	vector<ofxCvBlob> blobs;
	
};

