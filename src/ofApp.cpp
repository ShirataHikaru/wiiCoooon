#include "ofApp.h"
#include "ofxMaxim.h"

void ofApp::setup(){
    ofBackground(0, 0, 0);
    receiver.setup( PORT );
    
    AButtonState,BButtonState = 0;
    pitch, roll, yaw, accel = 0;
    
    ofSetVerticalSync(true);
    
    sampleRate = 44100; /* Sampling Rate */
    initialBufferSize = 512;	/* Buffer Size. you have to fill this buffer with sound*/
    
    ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);
    
    lAudio.assign(initialBufferSize, 0.0);
    rAudio.assign(initialBufferSize, 0.0);
    
    mode = 0;
}

void ofApp::update(){
    
}

void ofApp::draw(){
    ofSetColor(225);
    ofDrawBitmapString("MAXIMILIAN OSCILATORS", 32, 32);
    ofDrawBitmapString("0:sin, 1:saw, 2:pulse, 3:phasor, 4:triangle, 5:noise", 32, 48);
    ofDrawBitmapString(freq, 32, 64);
    
    ofNoFill();
    
    // draw the left channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 150, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, 900, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < lAudio.size(); i++){
        float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
        ofVertex(x, 100 -lAudio[i]*100.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    // draw the right channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 350, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Right Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, 900, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < rAudio.size(); i++){
        float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
        ofVertex(x, 100 -rAudio[i]*100.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(1000, 0,0);

    ofDrawRectangle(0, 0, 100, ofGetHeight());
    
    ofPushMatrix();
        for (int i ; i < 36 ; i++) {
            ofTranslate(0,ofGetHeight() / 36);
            ofDrawRectangle(0, 0, 100, ofGetHeight()/36);
            cout << "" << endl; //何故かこれが無いと動かないw
        }
    ofPopMatrix();
    

    ofFill();
    ofSetColor(245, 58, 135);
    
    ofTranslate(0, 10, 0);
    
    ofDrawRectangle(0, ofGetHeight()-(ofGetHeight()*pitch), 100, ofGetHeight() / 36);
    
    
    ofPopMatrix();
    ofPopStyle();
    
}


void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    for (int i = 0; i < bufferSize; i++){
        
        freq = ofMap(pitch,0,1,130.813,1046.502);
        float pan = 0.5;
        
        while(receiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            if (m.getAddress() == "/wii/1/accel/pry"){
                pitch = ofMap(m.getArgAsFloat(0), 0.15, 0.85, 0.0, 1.0);
                
                //小数点４桁以下を切り捨て
                int tmp = pitch * 1000;
                pitch = (float)tmp / 1000;
                
                pitch = MAX(pitch, 0);  //最大で1
                pitch = MIN(pitch, 1);  //最小で0
                
                roll = m.getArgAsFloat(1);
                yaw = m.getArgAsFloat(2);
                accel = m.getArgAsFloat(3);
                
            }
            else if (m.getAddress() == "/wii/1/button/B"){
                BButtonState = m.getArgAsFloat(0);;
            }else if (m.getAddress() == "/wii/1/button/A"){
                AButtonState = m.getArgAsFloat(0);;
            }else if (m.getAddress() == "/wii/1/button/Minus"){
                minusButtonState = m.getArgAsFloat(0);;
            }else if (m.getAddress() == "/wii/1/button/Plus"){
                pulsButtonState = m.getArgAsFloat(0);;
            }
            
        }
        
        
        if(minusButtonState == 1){
            mode--;
            mode = MAX(mode, 0);
        }
        
        if(pulsButtonState == 1){
            mode++;
            mode = MIN(mode, 1);
            
        }
        
        
        if(AButtonState == 1){
            int i = ofMap(pitch,0,1,0,21);
            
            switch (mode) {
                case 0:
                    wave = osc.sinewave(cBluesPenta[i]);
                    break;
                case 1:
                    wave = osc.saw(cBluesPenta[i]);
                    break;
                case 2:
                    wave = osc.pulse(cBluesPenta[i], 0.9);
                    break;
                case 3:
                    wave = osc.phasor(cBluesPenta[i]);
                    break;
                case 4:
                    wave = osc.triangle(cBluesPenta[i]);
                    break;
                case 5:
                    wave = osc.noise();
                    break;
                default:
                    wave = osc.sinewave(cBluesPenta[i]);
                    break;
            }
        }else{
            switch (mode) {
                case 0:
                    wave = osc.sinewave(freq);
                    break;
                case 1:
                    wave = osc.saw(freq);
                    break;
                case 2:
                    wave = osc.pulse(freq, 0.9);
                    break;
                case 3:
                    wave = osc.phasor(freq);
                    break;
                case 4:
                    wave = osc.triangle(freq);
                    break;
                case 5:
                    wave = osc.noise();
                    break;
                default:
                    wave = osc.sinewave(freq);
                    break;
            }
        }
        
        
        
        if(BButtonState == 1){
            mymix.stereo(wave, outputs, pan);
            lAudio[i] = output[i*nChannels    ] = outputs[0];
            rAudio[i] = output[i*nChannels + 1] = outputs[1];
        }
    }}

void ofApp::keyPressed(int key){
    if (key == '0') {
        mode = 0;
    }
    if (key == '1') {
        mode = 1;
    }
    if (key == '2') {
        mode = 2;
    }
    if (key == '3') {
        mode = 3;
    }
    if (key == '4') {
        mode = 4;
    }
    if (key == '5') {
        mode = 5;
    }   
}


void ofApp::dumpOSC(ofxOscMessage m) {
    string msg_string;
    msg_string = m.getAddress();
    for (int i=0; i<m.getNumArgs(); i++ ) {
        msg_string += " ";
        if(m.getArgType(i) == OFXOSC_TYPE_INT32)
            msg_string += ofToString( m.getArgAsInt32(i));
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT)
            msg_string += ofToString( m.getArgAsFloat(i));
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING)
            msg_string += m.getArgAsString(i);
    }
    cout << msg_string << endl;
}

