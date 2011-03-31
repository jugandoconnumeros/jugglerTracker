/*
 *  colorTracker.cpp
 *  jugglingTracker
 *
 *  Created by admin on 3/26/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "colorTracker.h"

void colorTracker::setup(int h, int s, int v, int hR, int sR, int vR, int w, int he)
{
	//color and rage setting
	hue = h;
	sat = s;
	val = v;
	hueRange = hR;
	satRange = sR;
	valRange = vR;
	
	//images for track
	width = w;
	height = he;
	videoGrayscaleCvImage.allocate(width, height);
	grayPixels = new unsigned char [width * height];
	
	//min and max area for a ball
	areaTreshold = 100;
	maxArea = 40*40;
	
	//qty of blobs
	blobNumber = 1;
	
	//for multiple blob trackers
	distanceThresh = 30;
	
	tracker.setup(500);
}

void colorTracker::updatePixel(int pixHue, int pixSat, int pixVal, int i) {
	int hueDiff;
	hueDiff = pixHue - hue;
	if (hueDiff < -127) hueDiff += 255;
	if (hueDiff > 127) hueDiff -= 255;
	if ( (fabs((float)hueDiff) < hueRange) &&
		(pixSat > (sat - satRange) && pixSat < (sat + satRange)) &&
		(pixVal > (val - valRange) && pixVal < (val + valRange))){
		
		grayPixels[i] = 255;
		
	} else {
		
		grayPixels[i] =  0;
	}
}


void colorTracker::update() {
	videoGrayscaleCvImage.setFromPixels(grayPixels, width, height);
	int count = contourFinder.findContours(videoGrayscaleCvImage, areaTreshold, maxArea, blobNumber, false, true);
	int i;
	//matching tracked blobs
	bool blobMatched;
	tracker.begin();
	if(count > 0) {
		trackedBlobs.assign(count, trackedBlob());
		for (i = 0; i < count; i++) {
			trackedBlobs[i] = contourFinder.blobs[i];
			//see if the blob is one known from previous frames
			blobMatched = tracker.matchTrackedBlob( trackedBlobs[i], distanceThresh );
			if( blobMatched == false){
				tracker.addBlob(trackedBlobs[i]);
			}
		}
	}
	tracker.end();
}

void colorTracker::drawPoints(bool showCountour) {
	int i;
	if(showCountour) {
		if (contourFinder.nBlobs > 0 ) {
			for (i = 0; i < contourFinder.nBlobs; i++) {
				ofSetColor(0xff9000);
				contourFinder.blobs[i].draw(0,0);
				ofCircle(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, 4);
			}
		} 
	} else {
		if(trackedBlobs.size() > 0){
			for (i = 0; i < trackedBlobs.size(); i++) {
				ofSetColor(0x0000FF);
				ofFill();
				ofCircle(trackedBlobs[i].centroid.x, trackedBlobs[i].centroid.y, 4);
				ofSetColor(0xFFFFFF);
				ofDrawBitmapString(ofToString(i,0), trackedBlobs[i].centroid.x+10, trackedBlobs[i].centroid.y+20);
				ofDrawBitmapString(ofToString(trackedBlobs[i].z,0), trackedBlobs[i].centroid.x+25, trackedBlobs[i].centroid.y+20);
			}
		}
	}
	/*VERDADERO TRACKER
	if(tracker.numStoredBlobs > 0){
		for (i = 0; i < tracker.numStoredBlobs; i++) {
			ofSetColor(0x0000FF);
			ofFill();
			ofCircle(tracker.storedBlobs[i].centroid.x, tracker.storedBlobs[i].centroid.y, 4);
			ofSetColor(0xFFFFFF);
			dist = getDistance(tracker.storedBlobs[i].centroid.x,tracker.storedBlobs[i].centroid.y);
			ofDrawBitmapString(ofToString(i,0), tracker.storedBlobs[i].centroid.x+10, tracker.storedBlobs[i].centroid.y+20);
			//ofDrawBitmapString(ofToString(dist,0), tracker.storedBlobs[i].centroid.x+25, tracker.storedBlobs[i].centroid.y+20);
		}
	}*/
}