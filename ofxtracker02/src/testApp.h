#pragma once

#include "ofMain.h"
#include "ofAddons.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxControlPanel.h"
#include "blobTracker.h"

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void drawPointCloud();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void eventsIn(guiCallbackData & data);

		float getDistance(int x, int y);
		//int getGrayColor(int i, int h, int s, int v, int hRange, int sRange, int vRange);
		
	
		ofxKinect kinect;

		ofxCvColorImage		colorImg;

		ofxCvGrayscaleImage 	grayImage;
		//ofxCvGrayscaleImage 	grayThresh;
		//ofxCvGrayscaleImage 	grayThreshFar;
		//ofxCvContourFinder 	contourFinder;
	
		ofxCvColorImage			videoColorHSVCvImage;
	
		ofxCvGrayscaleImage		videoGrayscaleHueImage;
		ofxCvGrayscaleImage		videoGrayscaleSatImage;
		ofxCvGrayscaleImage		videoGrayscaleBriImage;
	
		unsigned char *			grayPixels;
		unsigned char *			grayPixels2;
	
		ofxCvGrayscaleImage		videoGrayscaleCvImage;
		ofxCvGrayscaleImage		videoGrayscaleCvImage2;
	
		// for tracking...
		int hue, sat, val;
		int hueRange, satRange, valRange;
	
		// for tracking2nd color...
		int hue2, sat2, val2;
		int hueRange2, satRange2, valRange2;
	
		
		bool				bThreshWithOpenCV;
		bool				drawPC;
		bool				drawDepth;

		int 				nearThreshold;
		int					farThreshold;

		int					angle;
		int					areaTreshold;
		double				tempArea;
	
		
		int 				pointCloudRotationY;
	
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
		ofxCvContourFinder		contourFinder;
		ofxCvContourFinder		contourFinder2;
	
		int 				threshold;
		bool				bLearnBakground;
		bool				showCountour;
		bool				showVideo;
	
		
	
		ofxControlPanel		panel;
		ofTrueTypeFont		TTF;
	
		//variables para blob tracking
		blobTracker tracker;
		blobTracker tracker2;
	
		//vector is like an array - but it is more flexible
		//think of a vector as a c++ array
		vector <trackedBlob> trackedBlobs;
		vector <trackedBlob> trackedBlobs2;
		int distanceThresh;
	
		// create a variable of the typSe ofImage
		//ofxControlPanel			panel;

		
};
