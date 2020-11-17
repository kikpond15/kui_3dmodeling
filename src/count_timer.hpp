//
//  CountTimer.hpp
//  testCsv
//
//  Created by 菊池康太 on 2020/06/23.
//

#ifndef count_timer_hpp
#define count_timer_hpp

#include <stdio.h>
#include "ofMain.h"

class CountTimer {
public:
    CountTimer();
    void start();
    void stop();
    bool isTimeOver(int t);
    void sleep(int t);
    
private:
    float startTime, carentTime, endTime;
    bool isStart, isStop, isCount;
};
#endif /* CountTimer_hpp */


