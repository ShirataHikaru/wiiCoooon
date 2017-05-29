#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxMaxim.h"

// OSC通信するポート
#define PORT 9001

using namespace std;

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void dumpOSC(ofxOscMessage m); //OSCの中身を出力する
    void audioOut(float * input, int bufferSize, int nChannels);
    
    ofxOscReceiver receiver;
    
    ofSoundStream soundStream;
    
    int initialBufferSize;
    
    int sampleRate;
    
    double wave,sample,outputs[2];
    
    //周波数
    float freq;
    
    
    ofxMaxiMix mymix;
    //Oscilator
    ofxMaxiOsc osc;
    int mode;
    
    vector <float> lAudio;
    vector <float> rAudio;
    
    //Cマイナーペンタトニックスケール
    float cMpenta[16] = {130.813,146.832,164.814,195.998,220.000,261.626,293.665,329.628,391.995,440.000,523.251,587.330,659.255,783.991,880.000,1046.502};
    
    //Cブルースペンタトニックスケール
    float cBluesPenta[22] = {130.813, 146.832, 155.563, 174.614, 184.997, 195.998, 233.082, 261.626, 293.665, 311.127, 349.228, 369.994, 391.995, 466.164, 523.251, 587.330, 622.254, 698.456, 739.989, 783.991, 932.328, 1046.502};
    
    //pitch, yaw, roll, accel(加速度)
    float pitch,yaw,roll,accel;
    
    //ボタンの状態受け取る
    float AButtonState;
    float BButtonState;
    float pulsButtonState;
    float minusButtonState;
    
};
