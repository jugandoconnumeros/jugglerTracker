#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

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
		ofxCvGrayscaleImage		videoGrayscaleCvImage;
	
		// for tracking...
		int hue, sat, val;
	
		int hueRange;
		int satRange;
		int valRange;
	
		
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
	
		int 				threshold;
		bool				bLearnBakground;
	
	// create a variable of the typSe ofImage
	//ofxControlPanel			panel;
};
