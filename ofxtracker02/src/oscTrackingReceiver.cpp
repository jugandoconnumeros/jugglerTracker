/*
 *  oscTrackingReceiver.cpp
 *  opencvExample
 *
 *  Created by Chris on 3/6/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "oscTrackingReceiver.h"

void OscTrackingReceiver::setup(int _port, float scaleMeUpBy)
{
	port = _port;
	receiver.setup(port);
	scaleBy = scaleMeUpBy;
}

void OscTrackingReceiver::receive()
{
	blobs.clear();
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		// check for mouse moved message
		if ( m.getAddress() == "/blobs" )
		{
			int which = 0;
			// both the arguments are int32's
			int totalB = m.getArgAsInt32( which++ );
			//cout << "Total blobs: " << totalB << endl;
			
			ofxCvBlob blob;
			for( int i = 0; i < totalB; i++)
			{
				blob.centroid.x = m.getArgAsFloat(which++) * scaleBy; 
				blob.centroid.y = m.getArgAsFloat(which++) * scaleBy; 
				float z = m.getArgAsFloat(which++) * scaleBy;
				blob.area = m.getArgAsFloat(which++) * scaleBy;
				blob.boundingRect.x = m.getArgAsFloat(which++) * scaleBy; 
				blob.boundingRect.y = m.getArgAsFloat(which++) * scaleBy; 
				blob.boundingRect.width = m.getArgAsFloat(which++) * scaleBy; 
				blob.boundingRect.height = m.getArgAsFloat(which++) * scaleBy;
				blob.length = m.getArgAsFloat(which++) * scaleBy;  
				
				blobs.push_back(blob);
			} 
		}
		else
		{
			// debug message?
		}
		
	}
	
}


void OscTrackingReceiver::draw(float x, float y)
{
	for( int i = 0; i < blobs.size(); i++)
	{
		
		ofRectangle boundingRect = blobs[i].boundingRect;
		
		ofNoFill();
		ofSetColor(0xff0099);
		ofRect(x + boundingRect.x, y + boundingRect.y, boundingRect.width, boundingRect.height);
		ofCircle(blobs[i].centroid.x, blobs[i].centroid.y, 2);
	} 
}


