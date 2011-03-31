#pragma once
#include "ofMain.h"
//#include "ofAddons.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxControlPanel.h"
#include "blobTracker.h"
#include "ofxOsc.h"
#include "oscTrackingSender.h"
//#include "oscTrackingReceiver.h"
#include "colorTracker.h"

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
		void connectOsc();
		void sendOscPacket();
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
		
		bool				bThreshWithOpenCV;
		bool				drawPC;
		bool				drawDepth;
		bool				drawBlobs;
		int					captureColor;
		bool				useTracker1;
		bool				useTracker2;
		bool				useTracker3;
		bool				useTracker4;
		bool				oscConnected;
		bool				normalizaData;

		int 				nearThreshold;
		int					farThreshold;


		int					angle;
		
		double				tempArea;
	
		
		int 				pointCloudRotationY;
	
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
		

	
		int 				threshold;
		bool				bLearnBakground;
		bool				showCountour;
		bool				showVideo;
	
		
	
		ofxControlPanel		panel;
		ofTrueTypeFont		TTF;
	
		
		OscTrackingSender	sender;
	
	// CHRIS GUI STUFF
	guiTypeTextInput * guiIpInput;
	guiTypeTextInput * guiPortInput;
	
	colorTracker		cTrack0;
	colorTracker		cTrack1;
	colorTracker		cTrack2;
	colorTracker		cTrack3;
	
	
};
