//
//  ir_touch_panel.hpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/02.
//

#ifndef ir_touch_panel_hpp
#define ir_touch_panel_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxTuio.h"
#include "ofxOsc.h"

class TouchPanel {
public:
    TouchPanel();
    
    void draw();
    vector<ofVec2f> getFingersPos();
    vector<ofVec2f> getCursorsPos();
    
    void tuioAdded(ofxTuioCursor & tuioCursor);
    void tuioRemoved(ofxTuioCursor & tuioCursor);
    void tuioUpdated(ofxTuioCursor & tuioCursor);
    bool isInJudgArea(ofVec2f p);
    
    struct cursor {
        ofVec2f pos;
        long sessionID;
    };
    vector<cursor> cursors;

    ofxTuioReceiver tuio;
    vector<cursor> fingerTip;
    vector<cursor> p_fingerTip;
    ofVec2f fingerPos;
    ofVec2f limitArea1, limitArea2;
};

#endif /* ir_touch_panel_hpp */
