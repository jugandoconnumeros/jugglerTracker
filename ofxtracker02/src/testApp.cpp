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
	
	grayPixels = new unsigned char [kinect.width * kinect.height];
	
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);
	
	hueRange = 20;
	satRange = 30;
	valRange = 25;
	
	hue = 180;
	sat = 253;
	val = 171;
	
	//opnecv example vars
	bLearnBakground = true;
	threshold = 80;
	
	areaTreshold = 300;
	
	//nearThreshold = 230;
	//farThreshold  = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	tempArea = 0.0f;
	// start from the front
	//pointCloudRotationY = 180;
	
	drawPC = false;
	drawDepth = false;
}

//--------------------------------------------------------------
void testApp::update() {
	//ofSetBackgroundAuto(false);
	ofBackground(0, 0, 0);
	kinect.update();
	bool bNewFrame = kinect.isFrameNew();
	if(kinect.isFrameNew())	// there is a new frame and we are connected
	{
		//from opencv example
		colorImg.setFromPixels(kinect.getPixels(), kinect.width,kinect.height);
		//grayImage = colorImg;
		videoColorHSVCvImage = colorImg;
		videoColorHSVCvImage.convertRgbToHsv();
		videoColorHSVCvImage.convertToGrayscalePlanarImages(videoGrayscaleHueImage, videoGrayscaleSatImage, videoGrayscaleBriImage);
		
		unsigned char * colorHsvPixels = videoColorHSVCvImage.getPixels();
		
		for (int i = 0; i < kinect.width*kinect.height; i++){
			// since hue is cyclical:
			int hueDiff = colorHsvPixels[i*3] - hue;
			if (hueDiff < -127) hueDiff += 255;
			if (hueDiff > 127) hueDiff -= 255;
			
			if ( (fabs(hueDiff) < hueRange) &&
				(colorHsvPixels[i*3+1] > (sat - satRange) && colorHsvPixels[i*3+1] < (sat + satRange)) &&
				(colorHsvPixels[i*3+2] > (val - valRange) && colorHsvPixels[i*3+2] < (val + valRange))){
				
				grayPixels[i] = 255;
				
			} else {
				
				grayPixels[i] = 0;
			}
		}
		
		videoGrayscaleCvImage.setFromPixels(grayPixels, kinect.width, kinect.height);
		
		contourFinder.findContours(videoGrayscaleCvImage, 10, (kinect.width*kinect.height)/2, 20, false);
		
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		/*if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (kinect.width*kinect.height)/3, 10, true);
		
		//grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
			
		//we do two thresholds - one for the far plane and one for the near plane
		//we then do a cvAnd to get the pixels which are a union of the two thresholds.	
		/*if( bThreshWithOpenCV ){
			grayThreshFar = grayImage;
			grayThresh = grayImage;
			grayThresh.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThresh.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		}else{
		
			//or we do it ourselves - show people how they can work with the pixels
		
			unsigned char * pix = grayImage.getPixels();
			int numPixels = grayImage.getWidth() * grayImage.getHeight();

			for(int i = 0; i < numPixels; i++){
				if( pix[i] < nearThreshold && pix[i] > farThreshold ){
					pix[i] = 255;
				}else{
					pix[i] = 0;
				}
			}
		}

		//update the cv image
		grayImage.flagImageChanged();
	
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    	// also, find holes is set to true so we will get interior contours as well....
    	contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);*/
		
		
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofSetColor(255, 255, 255);
	if(drawPC){
		ofPushMatrix();
		ofTranslate(420, 320);
		// we need a proper camera class
		//drawPointCloud();
		ofPopMatrix();
	}else{
		//kinect.drawDepth(10, 10, 400, 300);
		//kinect.draw(420, 10, 400, 300);

		//grayImage.draw(10, 320, 400, 300);
		//contourFinder.draw(10, 320, 400, 300);
	}

	ofSetColor(0xffffff);
	
	if (drawDepth) {
		grayImage.draw(0,0,kinect.width,kinect.height);
	} else {
		colorImg.draw(0,0,kinect.width,kinect.height);
	}

	
	/*videoColorHSVCvImage.draw(0, 480, 160, 120);
	videoGrayscaleHueImage.draw(160,480,160,120);
	videoGrayscaleSatImage.draw(320,480, 160, 120);
	videoGrayscaleBriImage.draw(480,480,160,120);*/
	videoGrayscaleCvImage.draw(640,480,160,120);
	
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofSetColor(0xff9000);
		contourFinder.blobs[i].draw(0,0);
		ofSetColor(0xffff00);
		//ofRect(contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
		ofCircle(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, 4);
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
	<< endl;
	
	
	
	/*reportStream << "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold " << threshold << endl
	<< "num blobs found:" << contourFinder.nBlobs << endl
	<< "frame rate " << ofGetFrameRate()<< endl
	<< "area treshold " << areaTreshold << endl
	<< "using opencv threshold = " << bThreshWithOpenCV << endl
	<< "blob1 area " <<  tempArea << endl;*/
	ofSetColor(0xffffff);
	ofDrawBitmapString(reportStream.str(), 20, 630);
}

void testApp::drawPointCloud() {
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
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
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
		
		case 'a':
			areaTreshold -=10;
			break;
		case 's':
			areaTreshold +=10;
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
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	if (x >= 0 && x < 640 && y >= 0 && y < 480){
		int pixel = y * 640 + x;
		hue = videoGrayscaleHueImage.getPixels()[pixel];
		sat = videoGrayscaleSatImage.getPixels()[pixel];
		val = videoGrayscaleBriImage.getPixels()[pixel];
		
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

