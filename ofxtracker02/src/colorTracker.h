/*
 *  colorTracker.h
 *  jugglingTracker
 *
 *  Created by admin on 3/26/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofAddons.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxControlPanel.h"
#include "blobTracker.h"
#include "ofxOsc.h"
#include "oscTrackingSender.h"
//#include "oscTrackingReceiver.h"

class colorTracker {
	public:

	void setup(int h, int s, int v, int hR, int sR, int vR, int w, int he);
	void setColor(int h, int s, int v);
	void setRange(int hR, int sR, int vR);
	void updatePixel(int pixHue, int pixSat, int pixVal, int i);
	void update();
	void drawPoints(bool showCountour);
	
	// for tracking...
	int						hue, sat, val;
	int						hueRange, satRange, valRange;
	int						width, height, blobNumber;
	unsigned char *			grayPixels;
	ofxCvGrayscaleImage		videoGrayscaleCvImage;
	int						areaTreshold,maxArea;
	ofxCvContourFinder		contourFinder;
	
	//variables para blob tracking
	blobTracker				tracker;
	
	//vector is like an array - but it is more flexible
	//think of a vector as a c++ array
	vector <trackedBlob>	trackedBlobs;
	int						distanceThresh;
};