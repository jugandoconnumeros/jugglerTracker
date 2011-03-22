#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	//kinect.init(true);  //shows infrared image
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	//grayThresh.allocate(kinect.width, kinect.height);
	//grayThreshFar.allocate(kinect.width, kinect.height);
	
	videoColorHSVCvImage.allocate(kinect.width, kinect.height);
	
	videoGrayscaleHueImage.allocate(kinect.width, kinect.height);
	videoGrayscaleSatImage.allocate(kinect.width, kinect.height);
	videoGrayscaleBriImage.allocate(kinect.width, kinect.height);
	
	videoGrayscaleCvImage.allocate(kinect.width, kinect.height);
	videoGrayscaleCvImage2.allocate(kinect.width, kinect.height);
	
	grayPixels = new unsigned char [kinect.width * kinect.height];
	grayPixels2 = new unsigned char [kinect.width * kinect.height];
	
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);
	
	//green ball
	hueRange = 11;
	satRange = 85;
	valRange = 57;
	hue = 90;
	sat = 101;
	val = 254;
	
	//red ball
	hueRange2 = 43;
	satRange2 = 129;
	valRange2 = 31;
	hue2 = 170;
	sat2 = 253;
	val2 = 239;
	
	//opnecv example vars
	bLearnBakground = true;
	threshold = 80;
	
	//min area for a ball
	areaTreshold = 100;
	
	//nearThreshold = 230;
	//farThreshold  = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(30);

	// zero the tilt on startup
	angle = 15;
	kinect.setCameraTiltAngle(angle);
	tempArea = 0.0f;
	// start from the front
	//pointCloudRotationY = 180;
	
	drawPC = false;
	drawDepth = false;
	showCountour = true;
	showVideo = true;
	
	ofxControlPanel::setBackgroundColor(simpleColor(15, 15, 15, 150));
	ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 255));
	
	//control panel
	panel.loadFont("MONACO.TTF", 8);
	panel.setup("cv settings", 640, 0, 400, 760);
	panel.addPanel("control",2,false);
	panel.setWhichPanel("control");
	panel.setWhichColumn(0);
	panel.addToggle("drawDepth", "VIDEO_DRAW_DEPTH", 0);
	panel.addSlider("hue range ", "HUERANGE", 20, 0, 255, true);
	panel.addSlider("sat range ", "SATRANGE", 30, 0, 255, true);
	panel.addSlider("val range ", "VALRANGE", 25, 0, 255, true);
	
	panel.setWhichColumn(1);
	panel.addToggle("drawVideo", "VIDEO_DRAW_VIDEO", 1);
	panel.addSlider("hue range2 ", "HUERANGE2", 20, 0, 255, true);
	panel.addSlider("sat range2 ", "SATRANGE2", 30, 0, 255, true);
	panel.addSlider("val range2 ", "VALRANGE2", 25, 0, 255, true);
	
	panel.loadSettings("cvSettings.xml");
	
	
	
	//how far a blob has to move between frames before it is treated as a new blob
	//you usually have to adjust this based on the size of the video and the people within it.
	//basically this is 'how far do you think someone would normally move between frames measured in pixels'
	distanceThresh = 30;
	
	//we say that we are interested in previous blobs that are younger than 500ms
	//this means that if we don't see a stored blob for more than half a second we forget about it.
	//this is essentially the memory in milliseconds of your tracker
	tracker.setup(900);
	tracker2.setup(900);
	
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
		
		
		for (i = 0; i < kinect.width*kinect.height; i++){
			
			int hueDiff = colorHsvPixels[i*3] - hue;
			if (hueDiff < -127) hueDiff += 255;
			if (hueDiff > 127) hueDiff -= 255;
			
			if ( (fabs((float)hueDiff) < hueRange) &&
				(colorHsvPixels[i*3+1] > (sat - satRange) && colorHsvPixels[i*3+1] < (sat + satRange)) &&
				(colorHsvPixels[i*3+2] > (val - valRange) && colorHsvPixels[i*3+2] < (val + valRange))){
				
				grayPixels[i] = 255;
				
			} else {
				
				grayPixels[i] =  0;
			}
			
			hueDiff = colorHsvPixels[i*3] - hue2;
			if (hueDiff < -127) hueDiff += 255;
			if (hueDiff > 127) hueDiff -= 255;
			
			if ( (fabs((float)hueDiff) < hueRange2) &&
				(colorHsvPixels[i*3+1] > (sat2 - satRange2) && colorHsvPixels[i*3+1] < (sat2 + satRange2)) &&
				(colorHsvPixels[i*3+2] > (val2 - valRange2) && colorHsvPixels[i*3+2] < (val2 + valRange2))){
				
				grayPixels2[i] = 255;
				
			} else {
				
				grayPixels2[i] =  0;
			}
			// for each track
			//grayPixels[i] = getGrayColor(i,hue,sat,val,hueRange,satRange,valRange);
			//get
		}
		
		videoGrayscaleCvImage.setFromPixels(grayPixels, kinect.width, kinect.height);
		videoGrayscaleCvImage2.setFromPixels(grayPixels2, kinect.width, kinect.height);
	
		int count = contourFinder.findContours(videoGrayscaleCvImage, areaTreshold, 40*40, 2, false, true);
		int count2 = contourFinder2.findContours(videoGrayscaleCvImage2, areaTreshold, 40*40, 2, false, true);
		
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		bool blobMatched;
		
		tracker.begin();
		if( count > 0) {
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
		
		//test with 2 separate trackers
		tracker2.begin();
		if( count2 > 0) {
			trackedBlobs2.assign(count2, trackedBlob());
			for (i = 0; i < count2; i++) {
				trackedBlobs2[i] = contourFinder2.blobs[i];
				//see if the blob is one known from previous frames
				blobMatched = tracker2.matchTrackedBlob( trackedBlobs2[i], distanceThresh );
				if( blobMatched == false){
					tracker2.addBlob(trackedBlobs2[i]);
				}
			}
		}
		tracker2.end();
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
	//videoColorHSVCvImage.draw(0, 480, 160, 120);
	videoGrayscaleCvImage.draw(0,480,160,120);
	videoGrayscaleCvImage2.draw(160,480,160,120);
	/*videoColorHSVCvImage.draw(0, 480, 160, 120);
	videoGrayscaleHueImage.draw(160,480,160,120);
	videoGrayscaleSatImage.draw(320,480, 160, 120);
	videoGrayscaleBriImage.draw(480,480,160,120);*/
	//vector float distZ;
	
	int i;
	
	if(showCountour) {
		
		if (contourFinder.nBlobs > 0 ) {
			for (i = 0; i < contourFinder.nBlobs; i++) {
				ofSetColor(0xff9000);
				contourFinder.blobs[i].draw(0,0);
				ofCircle(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, 4);
			}
		} 		
				
		if(contourFinder2.nBlobs > 0) {
			for (i = 0; i < contourFinder2.nBlobs; i++) {
				ofSetColor(0xffff00);
				contourFinder2.blobs[i].draw(0,0);
				ofCircle(contourFinder2.blobs[i].centroid.x, contourFinder2.blobs[i].centroid.y, 4);
			}
		}
		
	} else {
		float dist;
		if(trackedBlobs.size() > 0){
			for (i = 0; i < trackedBlobs.size(); i++) {
				ofSetColor(0x0000FF);
				ofFill();
				ofCircle(trackedBlobs[i].centroid.x, trackedBlobs[i].centroid.y, 4);
				ofSetColor(0xFFFFFF);
				dist = getDistance(trackedBlobs[i].centroid.x,trackedBlobs[i].centroid.y);
				ofDrawBitmapString(ofToString(i,0), trackedBlobs[i].centroid.x+10, trackedBlobs[i].centroid.y+20);
				ofDrawBitmapString(ofToString(dist,0), trackedBlobs[i].centroid.x+25, trackedBlobs[i].centroid.y+20);
			}
		}
		if (trackedBlobs2.size() > 0) {
			for (i = 0; i < trackedBlobs2.size(); i++) {
				ofSetColor(0x00FFFF);
				ofFill();
				ofCircle(trackedBlobs2[i].centroid.x, trackedBlobs2[i].centroid.y, 4);
				dist = getDistance(trackedBlobs2[i].centroid.x,trackedBlobs2[i].centroid.y);
				ofSetColor(0xFFFFFF);
				ofDrawBitmapString(ofToString(i+2,0), trackedBlobs2[i].centroid.x+10, trackedBlobs2[i].centroid.y+20);
				ofDrawBitmapString(ofToString(dist,0), trackedBlobs2[i].centroid.x+25, trackedBlobs2[i].centroid.y+20);
			}
		}
	}
	/*grayImage.draw(180,20,160,120);
	grayBg.draw(20,130,160,120);
	grayDiff.draw(180,130,160,120);
	
	// then draw the contours:
	
	ofFill();
	ofSetColor(0x333333);
	ofRect(340,20,640,480);
	ofSetColor(0xffffff);
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		if(contourFinder.blobs[i].area < areaTreshold) {
			contourFinder.blobs[i].draw(340,20);
			tempArea = contourFinder.blobs[i].area;
		}
		
    }
	
	// we could draw the whole contour finder
	//contourFinder.draw(360,540);*/
	
	 
	
	
	
	/*stringstream reportStream;
	reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
								 << ofToString(kinect.getMksAccel().y, 2) << " / " 
								 << ofToString(kinect.getMksAccel().z, 2) << endl
				 << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
				 << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
				 << "set near threshold " << nearThreshold << " (press: + -)" << endl
				 << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
				 	<< ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
				 << "press UP and DOWN to change the tilt angle: " << angle << " degrees";c
	ofDrawBitmapString(reportStream.str(),20,666);*/
	
	
	stringstream reportStream;
	reportStream << "frame rate " << ofGetFrameRate() << endl
	<< " hueRange (z/x)= " << hueRange << " satRange (c/v)= " << satRange << " valRange (b/n)= " << valRange << endl
	<< " HSV (click to change)" << hue << " " << sat << " " << val << endl
	<< "num blobs found:" << contourFinder.nBlobs << " draw depth (p): " << drawDepth << endl
	<< "minArea (a/s)" << areaTreshold << " show countour(q): " << showCountour << endl
	<< "distanceTreshold (d/f)" << distanceThresh << " showVideo (o) " << showVideo << endl 
	<< endl;
	
	
	
	/*reportStream << "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold " << threshold << endl
	<< "num blobs found:" << contourFinder.nBlobs << endl
	<< "frame rate " << ofGetFrameRate()<< endl
	<< "area treshold " << areaTreshold << endl
	<< "using opencv threshold = " << bThreshWithOpenCV << endl
	<< "blob1 area " <<  tempArea << endl;*/
	ofSetColor(0xffffff);
	ofDrawBitmapString(reportStream.str(), 20, 630);
	
	panel.draw();
}

/*void testApp::drawPointCloud() {
	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(pointCloudRotationY);
	float* distancePixels = kinect.getDistancePixels();
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint cur = kinect.getWorldCoordinateFor(x, y);
			ofColor color = kinect.getCalibratedColorAt(x,y);
			glColor3ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}*/

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
	for (i = x - 30; i < x; i++) {
		for(j = y - 30; j < y; j++) {
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
	switch (key) {
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		
		case 's':
			areaTreshold--;
			break;
		case 'a':
			areaTreshold++;
			break;
			
		case 'z':
			hueRange++;
			break;
		case 'x':
			hueRange--;
			break;
			
		case 'c':
			satRange++;
			break;
		case 'v':
			satRange--;
			break;
			
		case 'b':
			valRange++;
			break;
		case 'n':
			valRange--;
			break;
			
		case 'p':
			drawDepth = !drawDepth;
			break;
		
		case 'o':
			showVideo = !showVideo;
			break;
			
		case 'q':
			showCountour = !showCountour;
			break;
		
		case 'd':
			distanceThresh++;
			break;
		case 'f':
			distanceThresh--;
			break;
/*		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
		break;
		case'p':
			drawPC = !drawPC;
			break;
	
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
		case '<':		
		case ',':		
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
		case '-':		
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break
		case 'o':
			kinect.setCameraTiltAngle(angle);	// go back to prev tilt
			kinect.open();
			break;
		case 'c':
			kinect.setCameraTiltAngle(0);		// zero the tilt
			kinect.close();
			break;;*/

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
	pointCloudRotationY = x;
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
		hue = videoGrayscaleHueImage.getPixels()[pixel];
		sat = videoGrayscaleSatImage.getPixels()[pixel];
		val = videoGrayscaleBriImage.getPixels()[pixel];
		
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

