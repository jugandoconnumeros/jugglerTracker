/*
 *  oscTrackingSender.cpp
 *  opencvExample
 *
 *  Created by Chris on 3/6/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "oscTrackingSender.h"

void OscTrackingSender::setup(string _host, int _port, float scaleMeBy)
{
	host = _host;
	port = _port;
	//sender.setup( host, port );
	scaleBy = scaleMeBy;
	
	addSender(_host);
	addSender("localhost");
	addSender("192.168.255.242");
	addSender("192.168.251.102");
	addSender("192.168.251.103");
	addSender("192.168.251.104");
	addSender("192.168.251.105");
	addSender("192.168.251.106");
	addSender("192.168.251.107");
	addSender("192.168.251.108");
	addSender("192.168.251.109");
	addSender("192.168.251.110");
	addSender("192.168.251.113");
	addSender("192.168.251.115");
	addSender("192.168.251.111");
	
	string a;
	string b;
	
	/*for (int i = 1; i < 255; i++) {
		a = "192.168.255.";
		b = ofToString(i, 0);
		a += b;
		addSender(a);
	}*/
	
}

void OscTrackingSender::addSender( string hostip )
{
	senders.push_back(new ofxOscSender());
	senders[ senders.size()-1 ]->setup(hostip,port);
	ofxOscMessage m;
	m.setAddress( "/gotIP" );
	m.addIntArg( 0 );
	senders[ senders.size()-1 ]->sendMessage(m);
}

void OscTrackingSender::send(vector<trackedBlob> blobs1, vector<trackedBlob> blobs2, vector<trackedBlob> blobs3, vector<trackedBlob> blobs4)
{
	int totalBlobs = blobs1.size() + blobs2.size();
	
	ofxOscMessage m;
	m.setAddress( "/blobs" );
	m.addIntArg( totalBlobs );
	int i;
	for(i = 0; i < blobs1.size(); i++)
	{
		m.addFloatArg( blobs1[i].centroid.x / scaleBy); // x
		m.addFloatArg( blobs1[i].centroid.y / scaleBy ); // y
		m.addFloatArg( blobs1[i].z ); // z
		m.addFloatArg( blobs1[i].area / scaleBy); // area
		m.addFloatArg( blobs1[i].boundingRect.x / scaleBy); 
		m.addFloatArg( blobs1[i].boundingRect.y / scaleBy); 
		m.addFloatArg( blobs1[i].boundingRect.width / scaleBy); 
		m.addFloatArg( blobs1[i].boundingRect.height / scaleBy);
		m.addFloatArg( blobs1[i].length / scaleBy);  
	}
	
	int j;
	for(j = 0; j < blobs2.size(); j++)
	{
		m.addFloatArg( blobs2[j].centroid.x / scaleBy); // x
		m.addFloatArg( blobs2[j].centroid.y / scaleBy ); // y
		m.addFloatArg( blobs2[i].z ); // z
		m.addFloatArg( blobs2[j].area / scaleBy); // area
		m.addFloatArg( blobs2[j].boundingRect.x / scaleBy); 
		m.addFloatArg( blobs2[j].boundingRect.y / scaleBy); 
		m.addFloatArg( blobs2[j].boundingRect.width / scaleBy); 
		m.addFloatArg( blobs2[j].boundingRect.height / scaleBy);
		m.addFloatArg( blobs2[j].length / scaleBy); 
		
	}
	
	for(i = 0; i < blobs3.size(); i++)
	{
		m.addFloatArg( blobs3[i].centroid.x / scaleBy); // x
		m.addFloatArg( blobs3[i].centroid.y / scaleBy ); // y
		m.addFloatArg( blobs3[i].z ); // z
		m.addFloatArg( blobs3[i].area / scaleBy); // area
		m.addFloatArg( blobs3[i].boundingRect.x / scaleBy); 
		m.addFloatArg( blobs3[i].boundingRect.y / scaleBy); 
		m.addFloatArg( blobs3[i].boundingRect.width / scaleBy); 
		m.addFloatArg( blobs3[i].boundingRect.height / scaleBy);
		m.addFloatArg( blobs3[i].length / scaleBy);  
	}
	
	for(j = 0; j < blobs4.size(); j++)
	{
		m.addFloatArg( blobs4[j].centroid.x / scaleBy); // x
		m.addFloatArg( blobs4[j].centroid.y / scaleBy ); // y
		m.addFloatArg( blobs4[i].z ); // z
		m.addFloatArg( blobs4[j].area / scaleBy); // area
		m.addFloatArg( blobs4[j].boundingRect.x / scaleBy); 
		m.addFloatArg( blobs4[j].boundingRect.y / scaleBy); 
		m.addFloatArg( blobs4[j].boundingRect.width / scaleBy); 
		m.addFloatArg( blobs4[j].boundingRect.height / scaleBy);
		m.addFloatArg( blobs4[j].length / scaleBy); 
		
	}
	
	for( int i = 0; i < senders.size(); i++)
	{
		senders[i]->sendMessage(m);
	}

}

