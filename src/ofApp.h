#pragma once

#include "ofMain.h"
#include "ir_touch_panel.hpp"
#include "gesture_manager.hpp"

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();
        void exit();
        void keyPressed(int key);
        void keyReleased(int key);

    TouchPanel touchPanel;
    GestureManager gestureManager;
    vector<ofVec2f> fingerTipPos;
    
    bool viewShadowArea, viewMoveObject;
    //vector<ofIcoSpherePrimitive> spheres;
    
    ofEasyCam cam;
    ofLight light;
    float x, y;
    
};
