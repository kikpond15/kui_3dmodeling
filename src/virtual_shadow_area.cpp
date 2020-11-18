//
//  virtual_shadow_area.cpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/03.
//

#include "virtual_shadow_area.hpp"

VirtualShadow::VirtualShadow(){
    maxDiameter = MAX_DIAMETER;
    minDiameter = MIN_DIAMETER;
    diameter = minDiameter;
    limitArea1 = ofVec2f(447.282, 209.883);
    limitArea2 = ofVec2f(1008.16, 716.69);
    center = ofVec2f(1008.16-447.282, 716.69-209.883);
    homePos = center;
}
//--------------------------------------------------------------

void VirtualShadow::setCenter(ofVec2f p){
    if(limitArea1.x <= p.x && p.x <= limitArea2.x){
        center.x = p.x;
    }
    if(limitArea1.y <= p.y && p.y <= limitArea2.y){
        center.y = p.y;
    }
}
//--------------------------------------------------------------

void VirtualShadow::setDiameter(float dia){
    if(MIN_DIAMETER < dia && dia < MAX_DIAMETER){
        diameter = dia;
    } else if(MIN_DIAMETER > dia){
        diameter = MIN_DIAMETER;
    } else if(MAX_DIAMETER < dia){
        diameter = MAX_DIAMETER;
    }
}
//--------------------------------------------------------------

void VirtualShadow::updateCenter(ofVec2f p){
    setCenter(center+p);
}
//--------------------------------------------------------------

void VirtualShadow::updateDiameter(float dia){
    setDiameter(diameter + dia);
}
//--------------------------------------------------------------

void VirtualShadow::update(ofVec2f p, float dia){
    setCenter(p);
    setDiameter(dia);
}
//--------------------------------------------------------------

void VirtualShadow::draw(){
    ofPushStyle();
        ofNoFill();
        ofSetColor(0,255,255);
        ofDrawEllipse(center, diameter, diameter);
    ofPopStyle();
}
//--------------------------------------------------------------

bool VirtualShadow::isAreaInside(ofVec2f p){
    float dist = abs(ofDist(p.x, p.y, center.x, center.y));
    if(dist < diameter/2){
        return true;
    } else {
        return false;
    }
}
//--------------------------------------------------------------

ofVec2f VirtualShadow::getCenterPos(){
    return center;
}
