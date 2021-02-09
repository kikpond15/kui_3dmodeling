//
//  gesture_manager.hpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/03.
//

#ifndef gesture_manager_hpp
#define gesture_manager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxSTLExporter.h"
#include "virtual_shadow_area.hpp"
#include "plotter_controller.hpp"
#include "csv_manager.hpp"
#include "count_timer.hpp"
#include "ofxOsc.h"

#define HOST "10.9.34.223"
#define PORT 12345


class GestureManager {
public:
    GestureManager();
    void setVirtualShadowParam(ofVec2f p, float area);
    void drawVirtualShadow();
    void drawFingers();
	void drawVirtualObject();
    void update(vector<ofVec2f> _fingers);
    void update2(vector<ofVec2f> _fingers);
    void update3(vector<ofVec2f> _fingers);
	void update4(vector<ofVec2f> _fingers);
    //void shadowPlotterUpdate(vector<ofVec2f> _fingers);
    void homing();
    void saveCsv();
    void setSphereObject(ofVec3f p);
    void drawSphereObjects();
    void saveSphereSTL();
	void oscSender(ofVec3f p);
	void setVirObjPos(ofVec3f p);

    VirtualShadow virtualShadow;
    PlotterController plotterCont;
    CsvManager csvManager;
    CountTimer timer;
    ofxSTLExporter exporter;
	ofxOscSender sender;

    vector<ofVec2f> fingers;
    ofVec2f fingerCenter, pFingerCenter;
    vector<ofIcoSpherePrimitive> spheres;
	ofIcoSpherePrimitive virtualObject;
    
    float fingDist, pFingDist;
    bool shadowInsideExisted;
    bool isStart, isPlay, isPlayBack, isPlayStop;

};
#endif /* gesture_manager_hpp */