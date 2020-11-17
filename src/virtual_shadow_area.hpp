//
//  virtual_shadow_area.hpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/03.
//

#ifndef virtual_shadow_area_hpp
#define virtual_shadow_area_hpp


#include <stdio.h>
#include "ofMain.h"

#define MAX_DIAMETER 600
#define MIN_DIAMETER 100


class VirtualShadow {
public:
    VirtualShadow();
    
    void setCenter(ofVec2f p);
    void setDiameter(float dia);
    void updateCenter(ofVec2f p);
    void updateDiameter(float dia);
    void update(ofVec2f p, float dia);
    void draw();
    bool isAreaInside(ofVec2f p);
    ofVec2f getCenterPos();

    float diameter;
    float maxDiameter, minDiameter;
    ofVec2f center, limitArea1, limitArea2, homePos;
};

#endif /* virtual_shadow_area_hpp */
