//
//  CsvManager.hpp
//  testCsv
//
//  Created by 菊池康太 on 2020/06/22.
//

#ifndef csv_manager_hpp
#define csv_manager_hpp

#include <stdio.h>
#include "ofxCsv.h"
#include "ofMain.h"


class CsvManager {
public:
    CsvManager();
    void stratRec();
    void recoding(float x, float y, float z);
    void recoding(vector<ofVec2f> pos);
    vector<ofVec2f> playbackVec2();
    ofVec3f playbackVec3();
    void endRec();
    bool isEndPlayBack();
    void stratPlayBack();
    
    ofxCsv csv;
    ofxCsv csvLoader;
    string fileName, blenderPath;
    int maxRowSize;
    int csvRowCounter;
    bool isStartRec, isRecoding, isEndRec, isPlayBack;
    
};
#endif /* CsvManager_hpp */
