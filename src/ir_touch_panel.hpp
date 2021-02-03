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
//#include "ofxTuio.h"
#include "ofxMultitouch.h"
//#include "ofxOsc.h"

class TouchPanel {
public:
    TouchPanel();
    vector<ofVec2f> getFingersPos();
	//void draw();
	void touchDown(ofTouchEventArgs& touch);
	void touchMove(ofTouchEventArgs& touch);
	void touchUp(ofTouchEventArgs& touch);

    ofVec2f fingerPos;
    ofVec2f limitArea1, limitArea2;
};

#endif /* ir_touch_panel_hpp */
