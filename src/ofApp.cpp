#include "ofApp.h"
#include <stdlib.h>
#include <iostream>

const int N = 256;		//Number of bands in spectrum
float spectrum[ N ];	//Smoothed spectrum values
float Rad = 500;		//Cloud raduis parameter
float Vel = 0.1;		//Cloud points velocity parameter
int bandRad = 2;		//Band index in spectrum, affecting Rad value
int bandVel = 100;		//Band index in spectrum, affecting Vel value

const int n = 300;		//Number of cloud points

//Offsets for Perlin noise calculation for points
float tx[n], ty[n];
ofPoint p[n];			//Cloud's points positions
float bg_transparent = 120;
float time0 = 0;		//Time value, used for dt computing
int t;
int seed = 325;
int pos = 0;
int sign = 0;
int a[3] = { 0, 51, 102};
//--------------------------------------------------------------
void testApp::setup(){
	//Set up sound sample
	bg.loadImage("new.jpg");
	sound.loadSound( "new.mp3", true );
	sound.setLoop( true );
	sound.play();

	//Set spectrum values to 0
	for (int i=0; i<N; i++) {
		spectrum[i] = 0.0f;
	}

	//Initialize points offsets by random numbers
	for ( int j=0; j<n; j++ ) {
		tx[j] = ofRandom( 0, 1000 );
		ty[j] = ofRandom( 0, 1000 );
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//Update sound engine
	ofSoundUpdate();

	//Get current spectrum with N bands
	float *val = ofSoundGetSpectrum( N );
	//We should not release memory of val,
	//because it is managed by sound engine

	//Update our smoothed spectrum,
	//by slowly decreasing its values and getting maximum with val
	//So we will have slowly falling peaks in spectrum
	for ( int i=0; i<N; i++ ) {
		spectrum[i] *= 0.97;	//Slow decreasing
		spectrum[i] = max( spectrum[i], val[i] );
	}

	//Update particles using spectrum values

	//Computing dt as a time between the last
	//and the current calling of update()
	float time = ofGetElapsedTimef();
	float dt = time - time0;
	dt = ofClamp( dt, 0.0, 0.1 );
	time0 = time; //Store the current time

	//Update Rad and Vel from spectrum
	//Note, the parameters in ofMap's were tuned for best result
	//just for current music track
	Rad = ofMap( spectrum[ bandRad ], 1, 3, 400, 800, true );
	Vel = ofMap( spectrum[ bandVel ], 0, 0.1, 0.05, 0.5 );

float d = 3;
if (fabs((time / d) - round(time / d)) < 0.01) {
	srand(seed);
	t = 0;
	pos = rand() % 3;
	sign = rand() % 2;
	seed = rand();
	sign == 0 ? a[pos] += 1 : a[pos] -= 1;
	if (a[pos] < 0) {
		a[pos] = 0;
		sign = (sign + 1) % 2;
	} else if (a[pos] > 255) {
		a[pos] = 255;
		sign = (sign + 1) % 2;
	}
	t++;
} else if (t < 51) {
	sign == 0 ? a[pos] += 1 : a[pos] -= 1;
	if (a[pos] < 0) {
		a[pos] = 0;
		sign = (sign + 1) % 2;
	} else if (a[pos] > 255) {
		a[pos] = 255;
		sign = (sign + 1) % 2;
	}
			t++;
	}




	//Update particles positions
	for (int j=0; j<n; j++) {
		tx[j] += Vel * dt;	//move offset
		ty[j] += Vel * dt;	//move offset
		//Calculate Perlin's noise in [-1, 1] and
		//multiply on Rad
		p[j].x = ofSignedNoise( tx[j] ) * Rad;
		p[j].y = ofSignedNoise( ty[j] ) * Rad;
	}

	if (bg_transparent > 0){
			 bg_transparent = 160 - time * 3.5;
	 }
	 else{
			 bg_transparent = 0;
	 }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground( 0, 0, 0 );	//Set up the background
	bg.draw(0, 0,ofGetWidth(), ofGetHeight());

	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, bg_transparent);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofDisableAlphaBlending();

	ofSetHexColor(0xffffff);
	ofDrawBitmapString("NOIZE MC", ofGetWidth() / 2,60);


	//Draw background rect for spectrum


	//Draw spectrum
	ofSetColor( 0, 0, 0 );
	int offset = (ofGetWidth() - 10) / N;
	for (int i=0; i<N; i++) {
		//Draw bandRad and bandVel by black color,
		//and other by gray color
		if ( i == bandRad || i == bandVel ) {
			ofSetColor( 250, 250, 250 ); //
		}
		else {
			ofSetColor( 0 ,204 ,255, 50); //Gray color
		}
		ofRect( 10 + i * offset, ofGetHeight(),offset - 2, -spectrum[i] * 100 );
	}

	//Draw cloud

	//Move center of coordinate system to the screen center
	ofPushMatrix();
	ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );

	//Draw points
	ofEnableAlphaBlending();
	ofSetColor( 160, 160, 160, fabs(130 - bg_transparent) );
	ofFill();
	for (int i=0; i<n; i++) {
		ofCircle( p[i], 2 );
	}


	ofSetColor(a[0], a[1], a[2], 75);
	//Draw lines between near points
	float dist = 40;	//Threshold parameter of distance
	for (int j=0; j<n; j++) {
		for (int k=j+1; k<n; k++) {
			if ( ofDist( p[j].x, p[j].y, p[k].x, p[k].y )
				< dist ) {
					ofLine( p[j], p[k] );
			}
		}
	}
	ofSetColor( 160, 160, 160, fabs(130 - bg_transparent) );
	ofDisableAlphaBlending();

	//Restore coordinate system
	ofPopMatrix();

	//  test for saving
  //  string fileName = "screen1.png";
  //  ofSaveScreen(fileName);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
