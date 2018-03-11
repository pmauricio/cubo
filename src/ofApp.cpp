#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //MIDI
    midiOut.listPorts();
    midiIn.openPort("SL MkII Port 1");	// by name
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    
    ofEnableAlphaBlending();
    ofSetBackgroundColor(100, 100, 100);
    ofSetFrameRate(60);
    j=0;
    
    parameters.add(color.set("color",ofColor(127),ofColor(0,0),ofColor(255)));
    gui.setup(parameters);
    // by now needs to pass the gui parameter groups since the panel internally creates it's own group
    sync.setup((ofParameterGroup&)gui.getParameter(),6666,"localhost",6667);
    ofSetVerticalSync(true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    sync.update();
    
    if(ofxMidiMessage::getStatusString(midiMessage.status) == "Note On" and midiMessage.pitch ==60)
        ofBackground(255,255,255);
    
    else
        
        ofBackground(color);
    
    gui.draw();
    ofNoFill();
    ofSetColor(255, 100, 100,255);
    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 20);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 34);
    text.str(""); // clear
    
    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 48);
    text.str(""); // clear
    ofDrawRectangle(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 96);
    text.str(""); // clear
    ofDrawRectangle(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 144);
    text.str(""); // clear
    ofDrawRectangle(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 192);
    text.str(""); // clear
    if(midiMessage.status == MIDI_PITCH_BEND) {
        ofDrawRectangle(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    }
    else {
        ofDrawRectangle(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    }
    
    text << "delta: " << midiMessage.deltatime;
    ofDrawBitmapString(text.str(), 20, 240);
    text.str(""); // clear
    
    
    
    
    j++;
    cam.begin();
    //    ofScale(100,100,100);
    //    sphere.drawWireframe();
    //
    //
    ofLight();
    ofFill();
    ofSetColor(255, 10, 10,55);//155*abs(sin((j/4)/25*2*PI)));
    ofDrawBox(0,0,0, 50);
    ofDrawBox(0,0,0, 100);
    //
    //    ofNoFill();
    
    i = 0;
    //    ofDrawAxis(100);
    //    ofDrawBox(0, 0, 1600, 1600 );
    while (i<50){
        ofSetColor(255, 10, 10,5*abs(sin((i-j/4)/50*2*PI)));
        //        ofDrawRectRounded(200-i,200-i, 100+2*i, 200+2*i,200);
        
        ofDrawBox(0,0,0, 100+i);
        i = i+1;
    }
    cam.end();
    //    ofDrawRect
    
    
    
}

//----------------------------------------------------
void ofApp::addColors(ofMesh &mesh) {
    for (int i=0; i<mesh.getNumVertices(); i++) {
        ofVec3f v = mesh.getVertex(i);
        mesh.addColor(ofFloatColor(v.x,v.y,v.z));
    }
}
//-------------------------------------------
ofMesh ofApp::createGeoSphere(int stacks, int slices) {
    ofMesh mesh;
    
    //add vertices
    mesh.addVertex(ofVec3f(0,0,1));
    
    for (int i=1; i<stacks; i++) {
        double phi = PI * double(i)/stacks;
        double cosPhi = cos(phi);
        double sinPhi = sin(phi);
        for (int j=0; j<slices; j++) {
            double theta = TWO_PI * double(j)/slices;
            mesh.addVertex(ofVec3f(cos(theta)*sinPhi, sin(theta)*sinPhi, cosPhi));
        }
    }
    mesh.addVertex(ofVec3f(0,0,-1));
    
    //top row triangle fan
    for (int j=1; j<slices; j++) {
        mesh.addTriangle(0,j,j+1);
    }
    mesh.addTriangle(0,slices,1);
    
    //triangle strips
    for (int i=0; i < stacks-2; i++) {
        int top = i*slices + 1;
        int bottom = (i+1)*slices + 1;
        
        for (int j=0; j<slices-1; j++) {
            mesh.addTriangle(bottom+j, bottom+j+1, top+j+1);
            mesh.addTriangle(bottom+j, top+j+1, top+j);
        }
        
        mesh.addTriangle(bottom+slices-1, bottom, top);
        mesh.addTriangle(bottom+slices-1, top, top+slices-1);
    }
    
    //bottom row triangle fan
    int last = mesh.getNumVertices()-1;
    for (int j=last-1; j>last-slices; j--) {
        mesh.addTriangle(last,j,j-1);
    }
    mesh.addTriangle(last,last-slices,last-1);
    
    return mesh;
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    midiMessage = msg;
    if(msg.control == 35){
        //color.set;
        color.set("color",ofColor(color.get().r,color.get().g,color.get().b,msg.value*2),ofColor(0,0),ofColor(255));
        cout << "37" << color.get().r << " "<<color.get().g << " "<< color.get().b;
    }
    if(msg.control == 37){
        //color.set;
        color.set("color",ofColor(color.get().r,msg.value*2,color.get().b),ofColor(0,0),ofColor(255));
        cout << "37" << color.get().r << " "<<color.get().g << " "<< color.get().b;
    }
    if(msg.control == 36){
        //color.set;
        color.set("color",ofColor(msg.value*2,color.get().g,color.get().b),ofColor(0,0),ofColor(255));
        cout << "37" << color.get().r << " "<<color.get().g << " "<< color.get().b;
    }
    if(msg.control == 38){
        //color.set;
        color.set("color",ofColor(color.get().r,color.get().g,msg.value*2),ofColor(0,0),ofColor(255));
        cout << "37" << color.get().r << " "<<color.get().g << " "<< color.get().b;
    }
    
    if(msg.control == 1){
        cam.setDistance(msg.value*10);
    }
    
    
    if(msg.control == 0){
        cam.setDistance(msg.value*10);
        cam.roll(((msg.value-8192)/8192)*2*PI);
        //        cam.rotate(msg.value, cam.getLookAtDir());
    }
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    update();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
