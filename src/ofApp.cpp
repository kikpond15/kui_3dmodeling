#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    gestureManager.setVirtualShadowParam(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), 100);//pos, area
    //gestureManager.setVirtualShadowParam(ofVec2f(1008.16-447.282, 716.69-209.883), 100);//pos, area
    viewShadowArea = false;
    viewMoveObject = true;
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofEnableDepthTest();
    ofEnableSmoothing();
    x = ofGetWidth()/2;
    y = ofGetHeight()/2;
    cam.setPosition(ofVec3f(x,y-700,700));
    cam.setTarget(ofVec3f(x,y,0));
    
    light.enable();
    light.setDirectional();
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setAmbientColor(ofFloatColor(0.5, 0.5, 0.5, 1.0));
    light.setPosition(x, y-100, 500);
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerTipPos = touchPanel.getFingersPos();
    gestureManager.update2(fingerTipPos);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(51);
    if(viewShadowArea){
        gestureManager.drawVirtualShadow();
        gestureManager.drawFingers();
    } else if(viewMoveObject){
        cam.begin();
        cam.disableMouseInput();
        ofSetColor(255);
        ofDrawRectangle(ofGetWidth()/2, ofGetHeight()/2, 1000,1000);
        gestureManager.drawSphereObjects();
        cam.end();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1'){
        viewShadowArea = false;
        viewMoveObject = true;
    } else if(key == '2'){
        viewShadowArea = true;
        viewMoveObject = false;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::exit(){
    gestureManager.saveSphereSTL();
    //gestureManager.saveCsv();
}
