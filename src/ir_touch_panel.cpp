//
//  ir_touch_panel.cpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/02.
//

#include "ir_touch_panel.hpp"
#include <list>

// make a list to keep track of active touches
list <ofTouchEventArgs> touches;

TouchPanel::TouchPanel(){
	ofxMultitouch::EnableTouch();
	// Add touch listeners
	ofAddListener(ofxMultitouch::touchDown, this, &TouchPanel::touchDown);
	ofAddListener(ofxMultitouch::touchMoved, this, &TouchPanel::touchMove);
	ofAddListener(ofxMultitouch::touchUp, this, &TouchPanel::touchUp);

    limitArea1 = ofVec2f(0.22368, 0.0327163);
    limitArea2 = ofVec2f(0.62889, 0.91742);
}

//--------------------------------------------------------------
vector<ofVec2f> TouchPanel::getFingersPos(){
    vector<ofVec2f> fingersPos;
	list<ofTouchEventArgs> ::iterator it;

	for (it = touches.begin(); it != touches.end(); ++it) {
		ofVec2f pos = ofVec2f((*it).x, (*it).y);
		fingersPos.push_back(pos);

		cout << "cursors:" << pos << endl;
	}
    return fingersPos;
}

//--------------------------------------------------------------
void TouchPanel::touchDown(ofTouchEventArgs & touch) {
	// Add a new circle for this new touch
	touches.push_back(touch);
}

//--------------------------------------------------------------
void TouchPanel::touchMove(ofTouchEventArgs & touch) {
	// Move the corresponding circle
	std::list <ofTouchEventArgs> ::iterator it;
	for (it = touches.begin(); it != touches.end(); ++it) {
		if (it->id == touch.id) {
			*it = touch;
		}
	}
}

//--------------------------------------------------------------
void TouchPanel::touchUp(ofTouchEventArgs & touch) {
	// Remove the corresponding circle
	ofTouchEventArgs touchToRemove;
	std::list <ofTouchEventArgs> ::iterator it;
	for (it = touches.begin(); it != touches.end(); ++it) {
		if (it->id == touch.id) {
			touchToRemove = *it;
		}
	}
	touches.remove(touchToRemove);
}
