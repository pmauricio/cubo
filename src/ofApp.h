#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "ofxLibwebsockets.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
    
public:
    void setup();
    void update();
    ofMesh createGeoSphere(int stacks, int slices);
    void addColors(ofMesh &mesh) ;
    void draw();
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    ofMesh sphere;
    ofEasyCam cam;
    double i;
    double j;
    
    ofParameterGroup parameters;
    ofParameter<ofColor> color;
    ofxPanel gui;
    ofxOscParameterSync sync;
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    stringstream text;
    ofxMidiIn midiIn;
    ofxMidiOut midiOut;
    ofxMidiMessage midiMessage;
};
