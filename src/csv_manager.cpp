//
//  CsvManager.cpp
//  testCsv
//
//  Created by 菊池康太 on 2020/06/22.
//

#include "csv_manager.hpp"

CsvManager::CsvManager(){
    isStartRec = false;
    isRecoding = false;
    isEndRec = false;
    isPlayBack = false;
    maxRowSize = 3;
    fileName = "test.csv";
    blenderPath = "/Users/kouta/Desktop/blenderPyCsvConvertStl/data/";
    csvRowCounter = 0;
}

void CsvManager::stratRec(){
    if(!isStartRec){
        isStartRec = true;
        isEndRec = false;
    }
}

void CsvManager::recoding(float x, float y, float z){
    if(isStartRec && !isEndRec){
        ofxCsvRow row;
        row.setFloat(0, x);
        row.setFloat(1, y);
        row.setFloat(2, z);
        csv.addRow(row);
    }
}

void CsvManager::recoding(vector<ofVec2f> pos){
    if(isStartRec && !isEndRec){
        ofxCsvRow row;
        for(int i=0; i<pos.size(); i++){
            row.setFloat(i*2, pos[i].x);
            row.setFloat(i*2+1, pos[i].y);
        }
        csv.addRow(row);
    }
}

void CsvManager::stratPlayBack(){
    isPlayBack = true;
}

vector<ofVec2f> CsvManager::playbackVec2(){
    
    if(!isEndRec) endRec();
    if(isPlayBack){
        vector<ofVec2f> pos;
        ofxCsvRow rowLoader;
        rowLoader = csv.getRow(csvRowCounter);
        for(int i=0; i<rowLoader.size(); i+=2){
            ofVec2f p = ofVec2f(rowLoader.getFloat(i), rowLoader.getFloat(i+i));
            pos.push_back(p);
        }
        return pos;
    }
}

ofVec3f CsvManager::playbackVec3(){
    if(isPlayBack){
        ofxCsvRow rowLoader;
        rowLoader = csv.getRow(csvRowCounter);
        float x = rowLoader.getFloat(0);
        float y = rowLoader.getFloat(1);
        float z = rowLoader.getFloat(2);
        ofVec3f pos = ofVec3f(x,y,z);
        csvRowCounter++;
        if(csvRowCounter > csv.size()) csvRowCounter = 0;
        return pos;
    }
}

bool CsvManager::isEndPlayBack(){
    if(isPlayBack){
        if(csvRowCounter >= csv.size()){
            //isPlayBack = false;
            csvRowCounter = 0;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void CsvManager::endRec(){
    isStartRec = false;
    isEndRec = true;
    isPlayBack = true;
    csv.save(fileName);
    csv.save(blenderPath+fileName);
}
