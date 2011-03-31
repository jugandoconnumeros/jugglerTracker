#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {

	ofSetFrameRate(60);
	//kinect.init(true);  //shows infrared image
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();
	
	captureColor = 0;
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	//grayThresh.allocate(kinect.width, kinect.height);
	//grayThreshFar.allocate(kinect.width, kinect.height);
	
	videoColorHSVCvImage.allocate(kinect.width, kinect.height);
	
	videoGrayscaleHueImage.allocate(kinect.width, kinect.height);
	videoGrayscaleSatImage.allocate(kinect.width, kinect.height);
	videoGrayscaleBriImage.allocate(kinect.width, kinect.height);
	
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);
	
	//luminosu balls, green and red
	cTrack0.setup(90,101,254,11,85,57,kinect.width, kinect.height);
	cTrack1.setup(170,253,239,43,129,131,kinect.width, kinect.height);
	cTrack2.setup(0,0,0,0,0,0,kinect.width, kinect.height);
	cTrack3.setup(0,0,0,0,0,0,kinect.width, kinect.height);
	
	/*cTrack0.setup(21,207,151,26,45,26,kinect.width, kinect.height);
	cTrack1.setup(172,245,106,35,58,43,kinect.width, kinect.height);
	cTrack2.setup(110,198,112,21,39,35,kinect.width, kinect.height);
	cTrack3.setup(0,0,0,0,0,0,kinect.width, kinect.height);*/
	
	//opnecv example vars
	bLearnBakground = true;
	threshold = 80;
	
	//nearThreshold = 230;
	//farThreshold  = 70;
	bThreshWithOpenCV = true;

	// zero the tilt on startup
	angle = 15;
	kinect.setCameraTiltAngle(angle);
	//tempArea = 0.0f;
	// start from the front
	//pointCloudRotationY = 180;
	
	drawPC = false;
	drawDepth = false;
	showCountour = true;
	showVideo = true;
	drawBlobs = true;
	captureColor = 0;
	oscConnected = false;
	normalizaData = true;
	
	//how far a blob has to move between frames before it is treated as a new blob
	//you usually have to adjust this based on the size of the video and the people within it.
	//basically this is 'how far do you think someone would normally move between frames measured in pixels'

	ofxControlPanel::setBackgroundColor(simpleColor(15, 15, 15, 150));
	ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 255));
	
	//control panel
	panel.loadFont("MONACO.TTF", 8);

	panel.setup("cv settings", 640, 0, ofGetWidth()-kinect.width, 760);
	
	panel.addPanel("control",2,false);
	panel.setWhichPanel("control");
	panel.setWhichColumn(0);
	panel.addSlider("hue range ", "HUERANGE", cTrack0.hueRange, 0, 255, true);
	panel.addSlider("sat range ", "SATRANGE", cTrack0.satRange, 0, 255, true);
	panel.addSlider("val range ", "VALRANGE", cTrack0.valRange, 0, 255, true);
	panel.addSlider("hue ", "HUE", cTrack0.hue, 0, 255, true);
	panel.addSlider("sat ", "SAT", cTrack0.sat, 0, 255, true);
	panel.addSlider("val ", "VAL", cTrack0.val, 0, 255, true);
	panel.addSlider("minimunArea ", "MIN_AREA", cTrack0.areaTreshold, 0, 1600, true);
	panel.addSlider("distanceTreshold ", "DISTANCE_TRESHOLD", cTrack0.distanceThresh, 0, 100, true);
	panel.addSlider("number of blobs", "BLOB_QTY", cTrack0.blobNumber, 0, 4,true);
	//panel.addToggle("useThisTracker", "TRACKER_USE", 1);
	panel.addToggle("drawDepth", "VIDEO_DRAW_DEPTH", 0);
	panel.addToggle("toggleContour", "CONTOUR_MODE", 1);
	panel.addSlider("captureColor", "CAPTURE_MODE", captureColor, 0,4,true);

	panel.setWhichColumn(1);
	panel.addSlider("hue range2 ", "HUERANGE2", cTrack1.hueRange, 0, 255, true);
	panel.addSlider("sat range2 ", "SATRANGE2", cTrack1.satRange, 0, 255, true);
	panel.addSlider("val range2 ", "VALRANGE2", cTrack1.valRange, 0, 255, true);
	panel.addSlider("hue2 ", "HUE2", cTrack1.hue, 0, 255, true);
	panel.addSlider("sat2 ", "SAT2", cTrack1.sat, 0, 255, true);
	panel.addSlider("val2 ", "VAL2", cTrack1.val, 0, 255, true);
	panel.addSlider("minimunArea2 ", "MIN_AREA2", cTrack1.areaTreshold, 0, 1600, true);
	panel.addSlider("distanceTreshold2 ", "DISTANCE_TRESHOLD2", cTrack1.distanceThresh, 0, 100, true);
	panel.addSlider("number of blobs2", "BLOB_QTY2", cTrack1.blobNumber, 0, 4,true);
	//panel.addToggle("useThisTracker", "TRACKER_USE2", 1);
	panel.addToggle("drawVideo", "VIDEO_DRAW_VIDEO", 1);
	panel.addToggle("drawBlobImages", "VIDEO_DRAW_BLOB", 1);
	
	panel.addPanel("control2",2,false);
	panel.setWhichPanel("control2");
	panel.setWhichColumn(0);
	panel.addSlider("hue range3 ", "HUERANGE3", cTrack2.hueRange, 0, 255, true);
	panel.addSlider("sat range3 ", "SATRANGE3", cTrack2.satRange, 0, 255, true);
	panel.addSlider("val range3 ", "VALRANGE3", cTrack2.valRange, 0, 255, true);
	panel.addSlider("hue3 ", "HUE3", cTrack2.hue, 0, 255, true);
	panel.addSlider("sat3 ", "SAT3", cTrack2.sat, 0, 255, true);
	panel.addSlider("val3 ", "VAL3", cTrack2.val, 0, 255, true);
	panel.addSlider("minimunArea3 ", "MIN_AREA", cTrack2.areaTreshold, 0, 1600, true);
	panel.addSlider("distanceTreshold3 ", "DISTANCE_TRESHOLD3", cTrack2.distanceThresh, 0, 100, true);
	panel.addSlider("number of blobs3", "BLOB_QTY3", cTrack2.blobNumber, 0, 4,true);
	//panel.addToggle("useThisTracker", "TRACKER_USE", 1);
	panel.addToggle("drawDepth", "VIDEO_DRAW_DEPTH", 0);
	panel.addToggle("toggleContour", "CONTOUR_MODE", 1);
	panel.addSlider("captureColor", "CAPTURE_MODE", captureColor, 0,4,true);
	
	panel.setWhichColumn(1);
	panel.addSlider("hue range4 ", "HUERANGE4", cTrack3.hueRange, 0, 255, true);
	panel.addSlider("sat range4 ", "SATRANGE4", cTrack3.satRange, 0, 255, true);
	panel.addSlider("val range4 ", "VALRANGE4", cTrack3.valRange, 0, 255, true);
	panel.addSlider("hue4 ", "HUE4", cTrack3.hue, 0, 255, true);
	panel.addSlider("sat4 ", "SAT4", cTrack3.sat, 0, 255, true);
	panel.addSlider("val4 ", "VAL4", cTrack3.val, 0, 255, true);
	panel.addSlider("minimunArea4 ", "MIN_AREA4", cTrack3.areaTreshold, 0, 1600, true);
	panel.addSlider("distanceTreshold4 ", "DISTANCE_TRESHOLD4", cTrack3.distanceThresh, 0, 100, true);
	panel.addSlider("number of blobs4", "BLOB_QTY4", cTrack3.blobNumber, 0, 4,true);
	//panel.addToggle("useThisTracker", "TRACKER_USE2", 1);
	panel.addToggle("drawVideo", "VIDEO_DRAW_VIDEO", 1);
	panel.addToggle("drawBlobImages", "VIDEO_DRAW_BLOB", 1);
	
	//osc
	panel.addPanel("osc",1,false);
	panel.setWhichPanel("osc");
	panel.setWhichColumn(0);
	//panel.addTextInput("Myhost", "127.0.0.1", 200 );
	//panel.addTextInput("Myport", "9000", 200);
	guiIpInput = panel.addTextInput("Myhost", "127.0.0.1", 200);
	guiPortInput = panel.addTextInput("Myport", "9000", 200);
	panel.addToggle("connected", "OSC_SEND", 0);
	//panel.addToggle("normalize", "DATA_NORMALIZE", 1);
	//not working
	//panel.loadSettings("cvSettings.xml");
	
	//if you want to use events call this after you have added all your gui elements
	panel.setupEvents();
	panel.enableEvents();
	
	//  -- this gives you back an ofEvent for all events in this control panel object
	ofAddListener(panel.guiEvent, this, &testApp::eventsIn);
	
	//we say that we are interested in previous blobs that are younger than 500ms
	//this means that if we don't see a stored blob for more than half a second we forget about it.

	
	sender.setup("127.0.0.1",9000,kinect.width);
	
}

//this captures all our control panel events - unless its setup differently in testApp::setup
//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){
	
	// print to terminal if you want to 
	//this code prints out the name of the events coming in and all the variables passed
	/*printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
	if( data.getDisplayName() != "" ){
		printf(" element name is %s \n", data.getDisplayName().c_str());
	}*/
	for(int k = 0; k < data.getNumValues(); k++){
		/*if( data.getType(k) == CB_VALUE_FLOAT ){
			printf("%i float  value = %f \n", k, data.getFloat(k));
		}
		else if( data.getType(k) == CB_VALUE_INT ){
			printf("%i int    value = %i \n", k, data.getInt(k));
		}
		else if( data.getType(k) == CB_VALUE_STRING ){
			printf("%i string value = %s \n", k, data.getString(k).c_str());
		}*/
		int a;
		string v = data.getXmlName();
		if (v == "HUERANGE") {
			cTrack0.hueRange = (int) data.getFloat(k);
		} else if(v == "SATRANGE") {
			cTrack0.satRange = (int) data.getFloat(k);
		} else if(v == "VALRANGE") {
			cTrack0.valRange = (int) data.getFloat(k);
		} else if(v == "HUE") {
			cTrack0.hue = (int) data.getFloat(k);
		}  else if(v == "SAT") {
			cTrack0.sat = (int) data.getFloat(k);
		} else if(v == "VAL") {
			cTrack0.val = (int) data.getFloat(k);
		} else if (v == "HUERANGE2") {
			cTrack1.hueRange = (int) data.getFloat(k);
		} else if(v == "SATRANGE2") {
			cTrack1.satRange = (int) data.getFloat(k);
		} else if(v == "VALRANGE2") {
			cTrack1.valRange = (int) data.getFloat(k);
		} else if(v == "HUE2") {
			cTrack1.hue = (int) data.getFloat(k);
		}  else if(v == "SAT2") {
			cTrack1.sat = (int) data.getFloat(k);
		} else if(v == "VAL2") {
			cTrack1.val = (int) data.getFloat(k);
		} else if(v == "MIN_AREA") {
			cTrack0.areaTreshold = (int) data.getFloat(k);
		} else if(v == "MIN_AREA2") {
			cTrack1.areaTreshold = (int) data.getFloat(k);
		} else if(v == "DISTANCE_TRESHOLD") {
			cTrack0.distanceThresh = (int) data.getFloat(k);
		} else if(v == "DISTANCE_TRESHOLD2") {
			cTrack1.distanceThresh = (int) data.getFloat(k);
		} else if(v == "BLOB_QTY") {
			cTrack0.blobNumber = (int) data.getFloat(k);
		}  else if(v == "BLOB_QTY2") {
			cTrack1.blobNumber = (int) data.getFloat(k);
		} if (v == "HUERANGE3") {
			cTrack2.hueRange = (int) data.getFloat(k);
		} else if(v == "SATRANGE3") {
			cTrack2.satRange = (int) data.getFloat(k);
		} else if(v == "VALRANGE3") {
			cTrack2.valRange = (int) data.getFloat(k);
		} else if(v == "HUE3") {
			cTrack2.hue = (int) data.getFloat(k);
		}  else if(v == "SAT3") {
			cTrack2.sat = (int) data.getFloat(k);
		} else if(v == "VAL3") {
			cTrack2.val = (int) data.getFloat(k);
		} else if (v == "HUERANGE4") {
			cTrack3.hueRange = (int) data.getFloat(k);
		} else if(v == "SATRANGE4") {
			cTrack3.satRange = (int) data.getFloat(k);
		} else if(v == "VALRANGE4") {
			cTrack3.valRange = (int) data.getFloat(k);
		} else if(v == "HUE4") {
			cTrack3.hue = (int) data.getFloat(k);
		}  else if(v == "SAT4") {
			cTrack3.sat = (int) data.getFloat(k);
		} else if(v == "VAL4") {
			cTrack3.val = (int) data.getFloat(k);
		} else if(v == "MIN_AREA3") {
			cTrack2.areaTreshold = (int) data.getFloat(k);
		} else if(v == "MIN_AREA4") {
			cTrack3.areaTreshold = (int) data.getFloat(k);
		} else if(v == "DISTANCE_TRESHOLD3") {
			cTrack2.distanceThresh = (int) data.getFloat(k);
		} else if(v == "DISTANCE_TRESHOLD4") {
			cTrack3.distanceThresh = (int) data.getFloat(k);
		} else if(v == "BLOB_QTY3") {
			cTrack2.blobNumber = (int) data.getFloat(k);
		}  else if(v == "BLOB_QTY4") {
			cTrack3.blobNumber = (int) data.getFloat(k);
		}else if(v == "CAPTURE_MODE") {
			captureColor = (int) data.getFloat(k);
		} else if(v == "VIDEO_DRAW_DEPTH") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				drawDepth = true;
			} else {
				drawDepth = false;
			}
		} else if(v == "VIDEO_DRAW_VIDEO") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				showVideo = true;
			} else {
				showVideo = false;
			}
		} else if(v == "CONTOUR_MODE") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				showCountour = true;
			} else {
				showCountour = false;
			}
		} else if(v == "VIDEO_DRAW_BLOB") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				drawBlobs = true;
			} else {
				drawBlobs = false;
			}
		} else if(v == "CAPTURE_MODE") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				captureColor = 1;
			} else {
				captureColor = 0;
			}
		} else if(v == "OSC_SEND") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				connectOsc();
				oscConnected = true;
			} else {
				oscConnected = false;
			}
		} else if(v == "DATA_NORMALIZE") {
			a = (int) data.getFloat(k);
			if(a == 1) {
				normalizaData = true;
			} else {
				normalizaData = false;
			}
		}

		
		
	}
	//printf("\n");
}

void testApp::connectOsc() {
	string host = guiIpInput->getValueText();
	int port = ofToInt(guiPortInput->getValueText());
	//string host = panel
	sender.setup(host,port,kinect.width);
}

void testApp::sendOscPacket() {
	//vector <ofxCvBlob> blobs;
	//int i;
	//blobs.assign(trackedBlobs.size()+trackedBlobs2.size(), ofxCvBlob());
	/*for(i=0; i < tracker.storedBlobs; i++) {
		blobs[i] = tracker.storedBlobs[i];
	}
	int j;
	for(j=0; j < tracker2.storedBlobs; j++) {
		blobs[i+j] = tracker2.storedBlobs[j];
	}*/
	/*for(i=0; i < trackedBlobs.size(); i++) {
		blobs[i] = trackedBlobs[i];
	}
	int j;
	for(j=0; j < trackedBlobs2.size(); j++) {
		blobs[i+j] = trackedBlobs2[j];
	}*/
	if(cTrack0.trackedBlobs.size() > 0 || cTrack1.trackedBlobs.size() > 0 || cTrack2.trackedBlobs.size() > 0 || cTrack3.trackedBlobs.size() > 0) {
		sender.send(cTrack0.trackedBlobs,cTrack1.trackedBlobs,cTrack2.trackedBlobs,cTrack3.trackedBlobs);
	}
	
}
//--------------------------------------------------------------
void testApp::update() {
	ofSetBackgroundAuto(true);
	ofBackground(80, 80, 80);

	//get new kinect frame
	kinect.update();
	bool bNewFrame = kinect.isFrameNew();
	if(bNewFrame)	// there is a new frame and we are connected
	{
		int i;
		//get the kinect img and ssign it
		colorImg.setFromPixels(kinect.getPixels(), kinect.width,kinect.height);
		//grayImage = colorImg;
		videoColorHSVCvImage = colorImg;
		videoColorHSVCvImage.convertRgbToHsv();
		videoColorHSVCvImage.convertToGrayscalePlanarImages(videoGrayscaleHueImage, videoGrayscaleSatImage, videoGrayscaleBriImage);
		
		unsigned char * colorHsvPixels = videoColorHSVCvImage.getPixels();
		
		int h, s, v;
		for (i = 0; i < kinect.width*kinect.height; i++){
			h = colorHsvPixels[i*3];
			s = colorHsvPixels[i*3+1];
			v = colorHsvPixels[i*3+2];
			cTrack0.updatePixel(h,s,v,i);
			cTrack1.updatePixel(h,s,v,i);
			cTrack2.updatePixel(h,s,v,i);
			cTrack3.updatePixel(h,s,v,i);
		}
		
		
		cTrack0.update();
		cTrack1.update();
		cTrack2.update();
		cTrack3.update();
		
		//captures the kinect depth
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		

		for (i = 0; i < cTrack0.trackedBlobs.size(); i++) {
			cTrack0.trackedBlobs[i].z = getDistance(cTrack0.trackedBlobs[i].centroid.x,cTrack0.trackedBlobs[i].centroid.y);
		}
		for (i = 0; i < cTrack1.trackedBlobs.size(); i++) {
			cTrack1.trackedBlobs[i].z = getDistance(cTrack1.trackedBlobs[i].centroid.x,cTrack1.trackedBlobs[i].centroid.y);
		}
		for (i = 0; i < cTrack2.trackedBlobs.size(); i++) {
			cTrack2.trackedBlobs[i].z = getDistance(cTrack2.trackedBlobs[i].centroid.x,cTrack2.trackedBlobs[i].centroid.y);
		}
		for (i = 0; i < cTrack3.trackedBlobs.size(); i++) {
			cTrack3.trackedBlobs[i].z = getDistance(cTrack3.trackedBlobs[i].centroid.x,cTrack3.trackedBlobs[i].centroid.y);
		}
	}
	panel.update();
}


//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(80, 80, 80);
	ofSetColor(0xffffff);
	if(showVideo) {
		if (drawDepth) {
			grayImage.draw(0,0,kinect.width,kinect.height);
		} else {
			colorImg.draw(0,0,kinect.width,kinect.height);
		}
	}
	
	//Draw the color detections as a thumbnail
	if(drawBlobs) {
		cTrack0.videoGrayscaleCvImage.draw(0,480,160,120);
		cTrack1.videoGrayscaleCvImage.draw(160,480,160,120);
		cTrack2.videoGrayscaleCvImage.draw(320,480,160,120);
		cTrack3.videoGrayscaleCvImage.draw(480,480,160,120);
	}
	/*videoColorHSVCvImage.draw(0, 480, 160, 120);
	videoGrayscaleHueImage.draw(160,480,160,120);
	videoGrayscaleSatImage.draw(320,480, 160, 120);
	videoGrayscaleBriImage.draw(480,480,160,120);*/
	
	int i;
	
	//draws the blobs
	cTrack0.drawPoints(showCountour);
	cTrack1.drawPoints(showCountour);
	cTrack2.drawPoints(showCountour);
	cTrack3.drawPoints(showCountour);

	stringstream reportStream;
	reportStream << "frame rate " << ofGetFrameRate() << endl
	<< "num blobs found (tracker1): " << cTrack0.contourFinder.nBlobs << " num blobs found (tracker2):" << cTrack1.contourFinder.nBlobs
	<< "num blobs found (tracker3): " << cTrack2.contourFinder.nBlobs << " num blobs found (tracker4):" << cTrack3.contourFinder.nBlobs
	<< endl;
	
	ofSetColor(0xffffff);
	ofDrawBitmapString(reportStream.str(), 20, ofGetHeight()-30);
	
	if(oscConnected) {
		sendOscPacket();	
	}
	
	panel.draw();
}


//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
float testApp::getDistance(int x, int y) {
	int i, j;
	float tempDist, dist;
	
	dist = 500000.0f;
	for (i = x - 30; i < x + 30; i++) {
		for(j = y - 30; j < y + 30; j++) {
			tempDist = kinect.getDistanceAt(x, y);
			if(tempDist < dist && tempDist > 0) {
				dist = tempDist;
			}
		}
	} 

	return dist;
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	panel.keyPressed( key );
	switch (key) {
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	panel.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	if (x >= 0 && x < 640 && y >= 0 && y < 480){
		int pixel = y * 640 + x;
		if(captureColor == 0) {
			cTrack0.hue = videoGrayscaleHueImage.getPixels()[pixel];
			cTrack0.sat = videoGrayscaleSatImage.getPixels()[pixel];
			cTrack0.val = videoGrayscaleBriImage.getPixels()[pixel];
			panel.setValueF("HUE",cTrack0.hue);
			panel.setValueF("SAT",cTrack0.sat);
			panel.setValueF("VAL",cTrack0.val);
		} else if(captureColor == 1) {
			cTrack1.hue = videoGrayscaleHueImage.getPixels()[pixel];
			cTrack1.sat = videoGrayscaleSatImage.getPixels()[pixel];
			cTrack1.val = videoGrayscaleBriImage.getPixels()[pixel];
			panel.setValueF("HUE2",cTrack1.hue);
			panel.setValueF("SAT2",cTrack1.sat);
			panel.setValueF("VAL2",cTrack1.val);
		} else if(captureColor == 2) {
			cTrack2.hue = videoGrayscaleHueImage.getPixels()[pixel];
			cTrack2.sat = videoGrayscaleSatImage.getPixels()[pixel];
			cTrack2.val = videoGrayscaleBriImage.getPixels()[pixel];
			panel.setValueF("HUE3",cTrack2.hue);
			panel.setValueF("SAT3",cTrack2.sat);
			panel.setValueF("VAL3",cTrack2.val);
		} else if(captureColor == 3) {
			cTrack3.hue = videoGrayscaleHueImage.getPixels()[pixel];
			cTrack3.sat = videoGrayscaleSatImage.getPixels()[pixel];
			cTrack3.val = videoGrayscaleBriImage.getPixels()[pixel];
			panel.setValueF("HUE4",cTrack2.hue);
			panel.setValueF("SAT4",cTrack2.sat);
			panel.setValueF("VAL4",cTrack2.val);
		}
	}
	panel.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	panel.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

