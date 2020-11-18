//
//  gesture_manager.cpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/03.
//

#include "gesture_manager.hpp"
GestureManager::GestureManager(){
    fingDist = 0;
    pFingDist = 0;
    shadowInsideExisted = false;
    ofPoint minP = ofPoint(virtualShadow.limitArea1.x,virtualShadow.limitArea1.y,virtualShadow.minDiameter);
    ofPoint maxP = ofPoint(virtualShadow.limitArea2.x,virtualShadow.limitArea2.y,virtualShadow.maxDiameter);
    plotterCont.setLimitPos(minP, maxP);
    isStart = false;
    isPlay = false;
    isPlayBack = false;
    exporter.beginModel("model_name");
}
//--------------------------------------------------------------

void GestureManager::setVirtualShadowParam(ofVec2f p, float area){
    virtualShadow.setCenter(p);
    virtualShadow.setDiameter(area);
}
//--------------------------------------------------------------

void GestureManager::update(vector<ofVec2f> _fingers){
    
    csvManager.stratRec();
    csvManager.recoding(fingers);
    
    
    fingers = _fingers;
    if(fingers.size() == 0){
        shadowInsideExisted = false;
        fingDist = 0;
        return ;
    } else if(fingers.size() == 1){
        pFingerCenter = fingerCenter;
        fingerCenter = fingers[0];
        if(virtualShadow.isAreaInside(fingerCenter)){
            if(shadowInsideExisted && fingDist == 0){
                virtualShadow.updateCenter(fingerCenter-pFingerCenter);
                plotterCont.update(virtualShadow.center, virtualShadow.diameter);
            }
            shadowInsideExisted = true;
        }
        fingDist = 0;
    } else if(fingers.size() == 2){
        pFingerCenter = fingerCenter;
        fingerCenter = ofVec2f((fingers[0].x+fingers[1].x)/2, (fingers[0].y+fingers[1].y)/2);
        pFingDist = fingDist;
        fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
        
        if(virtualShadow.isAreaInside(fingerCenter)){
            if(shadowInsideExisted && abs(pFingDist) > 0){
                virtualShadow.updateCenter(fingerCenter-pFingerCenter);
                virtualShadow.updateDiameter(fingDist-pFingDist);
                plotterCont.update(virtualShadow.center, virtualShadow.diameter);
            }
            shadowInsideExisted = true;
        }
    }
    cout << "fingerCenter:" << fingerCenter << endl;
}
//--------------------------------------------------------------

void GestureManager::update2(vector<ofVec2f> _fingers){
    fingers = _fingers;
    
    
    if(isPlay){
        csvManager.stratRec();
        csvManager.recoding(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter);
        setSphereObject(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
    }
    
    
    if(fingers.size() > 0){
        if(!isStart) isStart = true;
        if(!isPlay){
            isPlay = true;
            isPlayBack = false;
            csvManager.stratRec();
        }
        if(fingers.size() == 1){
            pFingerCenter = fingerCenter;
            fingerCenter = fingers[0];
            if(virtualShadow.isAreaInside(fingerCenter)){
                if(shadowInsideExisted && fingDist == 0){
                    virtualShadow.updateCenter(fingerCenter-pFingerCenter);
                    plotterCont.update(virtualShadow.center, virtualShadow.diameter);
                }
                shadowInsideExisted = true;
            }
            fingDist = 0;
        } else if(fingers.size() == 2){
            pFingerCenter = fingerCenter;
            fingerCenter = ofVec2f((fingers[0].x+fingers[1].x)/2, (fingers[0].y+fingers[1].y)/2);
            pFingDist = fingDist;
            fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
            
            if(virtualShadow.isAreaInside(fingerCenter)){
                if(shadowInsideExisted && abs(pFingDist) > 0){
                    virtualShadow.updateCenter(fingerCenter-pFingerCenter);
                    virtualShadow.updateDiameter(fingDist-pFingDist);
                    plotterCont.update(virtualShadow.center, virtualShadow.diameter);
                }
                shadowInsideExisted = true;
            }
        }
        timer.stop();
        cout << "Play and Recoding gesture" << endl;
    } else {    //操作がない時または，操作終了時
        if(isStart){
            if(isPlay){
                timer.start();
                if(timer.isTimeOver(5)){    //操作終了時カウント
                    homing();
                    csvManager.endRec();
                    timer.stop();
                    isPlay = false;
                    timer.sleep(5);
                }
                cout << "timer count now..." << endl;
            } else {
                if(!isPlayBack){    //再生処理
                    isPlayBack = true;
                } else{
                    ofVec3f p = csvManager.playbackVec3();
                    plotterCont.update(p, p.z);
                    cout << "Play Back now" << endl;
                    if(csvManager.isEndPlayBack()){
                        cout << "End Play Back Count" << endl;
                        timer.sleep(3);
                        homing();
                        timer.sleep(3);
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------

void GestureManager::shadowPlotterUpdate(vector<ofVec2f> _fingers){
    if(_fingers.size() == 1){
        pFingerCenter = fingerCenter;
        fingerCenter = _fingers[0];
        if(virtualShadow.isAreaInside(fingerCenter)){
            if(shadowInsideExisted && fingDist == 0){
                virtualShadow.updateCenter(fingerCenter-pFingerCenter);
                plotterCont.update(virtualShadow.center, virtualShadow.diameter);
            }
            shadowInsideExisted = true;
        }
        fingDist = 0;
    } else if(fingers.size() == 2){
        pFingerCenter = fingerCenter;
        fingerCenter = ofVec2f((_fingers[0].x+_fingers[1].x)/2, (_fingers[0].y+_fingers[1].y)/2);
        pFingDist = fingDist;
        fingDist = ofDist(_fingers[0].x, _fingers[0].y, _fingers[1].x, _fingers[1].y);
        
        if(virtualShadow.isAreaInside(fingerCenter)){
            if(shadowInsideExisted && abs(pFingDist) > 0){
                virtualShadow.updateCenter(fingerCenter-pFingerCenter);
                virtualShadow.updateDiameter(fingDist-pFingDist);
                plotterCont.update(virtualShadow.center, virtualShadow.diameter);
            }
            shadowInsideExisted = true;
        }
    }
}
//--------------------------------------------------------------

void GestureManager::update3(vector<ofVec2f> _fingers){
    if(isPlay){
        csvManager.stratRec();
        csvManager.recoding(fingers);
    }
    fingers = _fingers;
    if(fingers.size() > 0){
        if(!isStart) isStart = true;
        if(!isPlay){
            isPlay = true;
            csvManager.stratRec();
        }
        if(isPlay){
            shadowPlotterUpdate(fingers);
            cout << "Play and Recoding gesture" << endl;
        }
    } else {    //操作がない時または，操作終了時
        if(isStart){
            if(isPlay){
                timer.start();
                if(timer.isTimeOver(5)){    //操作終了時カウント
                    homing();
                    csvManager.endRec();
                    timer.stop();
                    isPlay = false;
                    timer.sleep(5);
                }
                cout << "timer count now..." << endl;
            } else {
                if(!isPlayBack){    //再生処理
                    isPlayBack = true;
                } else{
                    fingers = csvManager.playbackVec2();
                    shadowPlotterUpdate(fingers);
                    cout << "Play Back now" << endl;
                    if(csvManager.isEndPlayBack()){
                        cout << "End Play Back Count" << endl;
                        timer.sleep(3);
                        homing();
                        timer.sleep(3);
                        //isPlayBack = false;
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------

void GestureManager::drawFingers(){
    ofPushStyle();
    ofSetColor(0, 255, 100);
    if(fingers.size() == 1){
        ofDrawEllipse(fingerCenter, 10,10);
    } else if(fingers.size() == 2){
        ofDrawEllipse(fingerCenter, 10,10);
        ofDrawLine(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
    }
    ofPopStyle();
}
//--------------------------------------------------------------

void GestureManager::drawVirtualShadow(){
    virtualShadow.draw();
}
//--------------------------------------------------------------


void GestureManager::homing(){
    //virtualShadow.updateCenter(virtualShadow.homePos);
    //virtualShadow.updateDiameter(virtualShadow.minDiameter);
    virtualShadow.setCenter(ofVec2f(ofGetWidth()/2, ofGetHeight()/2));
    //virtualShadow.setCenter(virtualShadow.homePos);
    virtualShadow.setDiameter(virtualShadow.minDiameter);
    plotterCont.update(virtualShadow.center, virtualShadow.diameter);
}


void GestureManager::saveCsv(){
    csvManager.endRec();
}

void GestureManager::setSphereObject(ofVec3f p){
    ofIcoSpherePrimitive sphere;
    sphere.set(30, 1);
    p.y = ofGetHeight()-p.y;
    sphere.setPosition(p.x, p.y, p.z);
    spheres.push_back(sphere);
}

void GestureManager::drawSphereObjects(){
    if(spheres.size() > 0){
        ofSetColor(255, 0, 0);
        for(auto&& sp : spheres){
            sp.draw();
        }
    }
}

void GestureManager::saveSphereSTL(){
    if(spheres.size() > 0){
        for(ofIcoSpherePrimitive sphere : spheres){
            ofMesh meshs = sphere.getMesh();
            vector<ofMeshFace> faces = meshs.getUniqueFaces();
            for(ofMeshFace face: faces){
                face.setVertex(0, face.getVertex(0)+sphere.getPosition());
                face.setVertex(1, face.getVertex(1)+sphere.getPosition());
                face.setVertex(2, face.getVertex(2)+sphere.getPosition());
                exporter.addTriangle(face.getVertex(0), face.getVertex(1), face.getVertex(2), face.getFaceNormal());
            }
        }
    }
    cout << "save STL" << endl;
    exporter.saveModel(ofFilePath::getCurrentExeDir() + "../../../model.stl");
}


