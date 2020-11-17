//
//  CountTimer.cpp
//  testCsv
//
//  Created by 菊池康太 on 2020/06/23.
//

#include "count_timer.hpp"

CountTimer::CountTimer(){
    startTime = 0;
    carentTime = 0;
    endTime = 0;
    isStart = false;
    isStop = false;
    isCount = false;
}

void CountTimer::start(){
    if(!isStart){
        isStart = true;
        isStop = false;
        startTime = ofGetElapsedTimeMillis();
    }
}

void CountTimer::stop(){
    isStart = false;
}

bool CountTimer::isTimeOver(int t){
    carentTime = (ofGetElapsedTimeMillis() - startTime) / 1000;
    cout << "Start Time:" << startTime << ",   Carent time:" << carentTime << endl;
    if(t <= carentTime){
        return true;
    } else {
        return false;
    }
}

void CountTimer::sleep(int t){
    startTime = ofGetElapsedTimeMillis();
    carentTime = (ofGetElapsedTimeMillis() - startTime);
    while(t*1000 >= carentTime){
        carentTime = (ofGetElapsedTimeMillis() - startTime);
    }
}
