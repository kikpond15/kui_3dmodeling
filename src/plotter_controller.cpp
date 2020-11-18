//
//  plotter_controller.cpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/04.
//

#include "plotter_controller.hpp"

PlotterController::PlotterController(){
    arduino.setup("/dev/cu.usbmodem142401", 115200);
    
}
//--------------------------------------------------------------

void PlotterController::setLimitPos(ofPoint _minPos, ofPoint _maxPos){
    maxPos = _maxPos;
    minPos = _minPos;
}
//--------------------------------------------------------------

void PlotterController::update(ofVec2f pos, float dia){
    
    string xyStr = convertPos2XY(pos);
    string zSter = convertPinch2Z(dia);
    sendSerial(xyStr+zSter);
}
//--------------------------------------------------------------

string PlotterController::convertPos2XY(ofVec2f pos){
    int xVal = ofMap(pos.x, minPos.x, maxPos.x, 0,255);
    if(xVal < 0) xVal = 0;
    else if(xVal > 255) xVal = 255;
    int yVal = ofMap(pos.y, minPos.y, maxPos.y, 0,255);
    if(yVal < 0) yVal = 0;
    else if(yVal > 255) yVal = 255;
    
    string sx,sy;
    if(xVal >= 100) sx = ofToString(xVal);
    else if(100 > xVal && xVal >= 10) sx = '0'+ofToString(xVal);
    else if(10 > xVal) sx = "00"+ofToString(xVal);
    
    if(yVal >= 100) sy = ofToString(yVal);
    else if(100 > yVal && yVal >= 10) sy = '0'+ofToString(yVal);
    else if(10 > yVal) sy = "00"+ofToString(yVal);
    
    return sx+sy;
}
//--------------------------------------------------------------

string PlotterController::convertPinch2Z(float dia){
    int zVal = ofMap(dia, minPos.z, maxPos.z, 0,255);
    if(zVal < 0) zVal = 0;
    else if(zVal > 255) zVal = 255;
    string sz;
    
    if(zVal >= 100) sz=ofToString(zVal);
    else if(100 > zVal && zVal >= 10) sz='0'+ofToString(zVal);
    else if(10 > zVal) sz="00"+ofToString(zVal);
    
    return sz;
}
//--------------------------------------------------------------
void PlotterController::sendSerial(string str){
    string sendStr = 'a'+str;
    
    //cout << "sendStr= " << sendStr << endl;
    
//    /* ------------------Serial-------------------------
    unsigned char* buf = (unsigned char*)sendStr.c_str();

    arduino.writeBytes(buf, sendStr.size());
    arduino.flush();
//     ---------------------------------------------------*/
    
}
//--------------------------------------------------------------




