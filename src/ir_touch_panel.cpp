//
//  ir_touch_panel.cpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/02.
//

#include "ir_touch_panel.hpp"


TouchPanel::TouchPanel(){
    tuio.setup(new ofxTuioUdpReceiver(3333));
    ofAddListener(tuio.AddTuioCursor, this, &TouchPanel::tuioAdded);
    ofAddListener(tuio.UpdateTuioCursor, this, &TouchPanel::tuioUpdated);
    ofAddListener(tuio.RemoveTuioCursor, this, &TouchPanel::tuioRemoved);
    tuio.connect(false);
    cout << "connect : " << tuio.isConnected() << endl;
    
    limitArea1 = ofVec2f(0.22368, 0.0327163);
    limitArea2 = ofVec2f(0.62889, 0.91742);

}
//--------------------------------------------------------------

void TouchPanel::draw(){
    p_fingerTip = fingerTip;
    fingerTip = cursors;
    for (int i = 0; i < fingerTip.size(); i++) {
        ofVec2f pos = ofVec2f(fingerTip[i].pos.x * ofGetWidth(), fingerTip[i].pos.y * ofGetHeight());
        string id = ofToString(fingerTip[i].sessionID);
        fingerPos = pos;
        ofSetColor(0,100,255);
        ofDrawCircle(pos, 10);
        //        ofSetColor(0);
        //        ofDrawBitmapString(id, pos);
    }
}

//--------------------------------------------------------------
vector<ofVec2f> TouchPanel::getFingersPos(){
    vector<ofVec2f> fingersPos;
    for(int i = 0; i < cursors.size(); i++){
        //if(isInJudgArea(cursors[i].pos)){
            ofVec2f pos = ofVec2f(cursors[i].pos.x * ofGetWidth(), cursors[i].pos.y * ofGetHeight());
            fingersPos.push_back(pos);
        // }
        cout << "cursors:" << cursors[i].pos << endl;
    }
    return fingersPos;
}

//--------------------------------------------------------------
vector<ofVec2f> TouchPanel::getCursorsPos(){
    vector<ofVec2f> cursorsPos;
    for(int i = 0; i < cursors.size(); i++){
        cursorsPos.push_back(cursors[i].pos);
    }
    return cursorsPos;
}
//--------------------------------------------------------------
bool TouchPanel::isInJudgArea(ofVec2f p){
    
    if(limitArea1.x <= p.x && limitArea1.y <= p.y && p.x >= limitArea2.x && p.y >= limitArea2.y){
        return true;
    } else {
        return false;
    }
}



//--------------------------------------------------------------
void TouchPanel::tuioAdded(ofxTuioCursor &tuioCursor) {
    
    ofVec2f pos = ofVec2f(tuioCursor.getX(), tuioCursor.getY());
    long id = tuioCursor.getSessionID();
    //cout << "add " << id << " at " << pos << endl;
    cursor c;
    c.pos = pos;
    c.sessionID = id;
    cursors.push_back(c);
}

//--------------------------------------------------------------
void TouchPanel::tuioUpdated(ofxTuioCursor &tuioCursor) {
    
    ofVec2f pos = ofVec2f(tuioCursor.getX(), tuioCursor.getY());
    long id = tuioCursor.getSessionID();
    //cout << "update " << id << " at " << pos << endl;
    
    vector<cursor> c = cursors;
    for (int i = 0; i < c.size(); i++) {
        if (c[i].sessionID != id) continue;
        c[i].pos = ofVec2f(tuioCursor.getPosition().getX(), tuioCursor.getPosition().getY());
    }
    cursors = c;
}

//--------------------------------------------------------------
void TouchPanel::tuioRemoved(ofxTuioCursor &tuioCursor) {
    
    ofVec2f pos = ofVec2f(tuioCursor.getX(), tuioCursor.getY());
    long id = tuioCursor.getSessionID();
    //cout << "remove " << id << " at " << pos << endl;
    
    vector<cursor> c = cursors;
    for (int i = 0; i < c.size(); i++) {
        if (c[i].sessionID != id) continue;
        c.erase(c.begin() + i);
        break;
    }
    cursors = c;
}

//--------------------------------------------------------------
