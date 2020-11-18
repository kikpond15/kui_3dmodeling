//
//  plotter_controller.hpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/04.
//

#ifndef plotter_controller_hpp
#define plotter_controller_hpp

#include <stdio.h>
#include "ofMain.h"

class PlotterController{
public:
    PlotterController();
    void setLimitPos(ofPoint _minPos, ofPoint _maxPos);
    void sendSerial(string str);
    void update(ofVec2f pos, float dia);
    string convertPos2XY(ofVec2f pos);
    string convertPinch2Z(float dia);
    
    
    ofSerial arduino;
    ofPoint maxPos, minPos;
};

#endif /* plotter_controller_hpp */
