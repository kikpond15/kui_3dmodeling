//
//  gesture_manager.cpp
//  test_kui_tuio
//
//  Created by 菊池康太 on 2020/03/03.
//

#include "gesture_manager.hpp"
GestureManager::GestureManager() {
	fingDist = 0;
	pFingDist = 0;
	shadowInsideExisted = false;
	//操作の有効範囲(マーカー上4端のタッチ座標)
	effectiveMaxArea = ofVec2f();
	effectiveMinArea = ofVec2f();
	ofPoint minP = ofPoint(virtualShadow.limitArea1.x, virtualShadow.limitArea1.y, virtualShadow.minDiameter);
	ofPoint maxP = ofPoint(virtualShadow.limitArea2.x, virtualShadow.limitArea2.y, virtualShadow.maxDiameter);
	plotterCont.setLimitPos(minP, maxP);
	virtualObject.set(34, 1);
	setVirObjPos(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
	isStart = false;
	isPlay = false;
	isPlayBack = false;
	isPlayStop = false;
	exporter.beginModel("model_name");
	sender.setup(HOST, PORT);
}
//--------------------------------------------------------------

void GestureManager::setVirtualShadowParam(ofVec2f p, float area) {
	virtualShadow.setCenter(p);
	virtualShadow.setDiameter(area);
}
//--------------------------------------------------------------

void GestureManager::update(vector<ofVec2f> _fingers) {

	csvManager.stratRec();
	csvManager.recoding(fingers);


	fingers = _fingers;
	if (fingers.size() == 0) {
		shadowInsideExisted = false;
		fingDist = 0;
		return;
	}
	else if (fingers.size() == 1) {
		pFingerCenter = fingerCenter;
		fingerCenter = fingers[0];
		if (virtualShadow.isAreaInside(fingerCenter)) {
			if (shadowInsideExisted && fingDist == 0) {
				virtualShadow.updateCenter(fingerCenter - pFingerCenter);
				plotterCont.update(virtualShadow.center, virtualShadow.diameter);
			}
			shadowInsideExisted = true;
		}
		fingDist = 0;
	}
	else if (fingers.size() == 2) {
		pFingerCenter = fingerCenter;
		fingerCenter = ofVec2f((fingers[0].x + fingers[1].x) / 2, (fingers[0].y + fingers[1].y) / 2);
		pFingDist = fingDist;
		fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);

		if (virtualShadow.isAreaInside(fingerCenter)) {
			if (shadowInsideExisted && abs(pFingDist) > 0) {
				virtualShadow.updateCenter(fingerCenter - pFingerCenter);
				virtualShadow.updateDiameter(fingDist - pFingDist);
				plotterCont.update(virtualShadow.center, virtualShadow.diameter);
			}
			shadowInsideExisted = true;
		}
	}
	cout << "fingerCenter:" << fingerCenter << endl;
}
//--------------------------------------------------------------

void GestureManager::update2(vector<ofVec2f> _fingers) {
	fingers = _fingers;

	if (isPlay) {
		csvManager.stratRec();
		csvManager.recoding(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter);
		setSphereObject(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
		oscSender(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
	}

	if (fingers.size() > 0) {
		if (!isStart) isStart = true;
		if (!isPlay) {
			isPlay = true;
			isPlayBack = false;
			csvManager.stratRec();
		}
		if (fingers.size() == 1) {
			pFingerCenter = fingerCenter;
			fingerCenter = fingers[0];
			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && fingDist == 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
				}
				shadowInsideExisted = true;
			}
			fingDist = 0;
		}
		else if (fingers.size() == 2) {
			pFingerCenter = fingerCenter;
			fingerCenter = ofVec2f((fingers[0].x + fingers[1].x) / 2, (fingers[0].y + fingers[1].y) / 2);
			pFingDist = fingDist;
			fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);

			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && abs(pFingDist) > 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					virtualShadow.updateDiameter(fingDist - pFingDist);
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
				}
				shadowInsideExisted = true;
			}
		}
		timer.stop();
		cout << "Play and Recoding gesture" << endl;
	}
	else {    //操作がない時または，操作終了時
		if (isStart) {
			if (isPlay) {
				timer.start();
				if (timer.isTimeOver(5)) {    //操作終了時カウント
					homing();
					csvManager.endRec();
					timer.stop();
					isPlay = false;
					timer.sleep(5);
				}
				cout << "timer count now..." << endl;
			}
			else {
				if (!isPlayBack) {    //再生処理
					isPlayBack = true;
				}
				else {
					ofVec3f p = csvManager.playbackVec3();
					plotterCont.update(p, p.z);
					cout << "Play Back now" << endl;
					if (csvManager.isEndPlayBack()) {
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
/*
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
*/
//--------------------------------------------------------------

void GestureManager::update3(vector<ofVec2f> _fingers) {
	fingers = _fingers;
	if (isPlay) {
		csvManager.stratRec();
		csvManager.recoding(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter);
		setSphereObject(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
	}
	if (fingers.size() > 0) {
		if (!isStart) isStart = true;
		if (!isPlay) {
			isPlay = true;
			isPlayBack = false;
			csvManager.stratRec();
			fingerCenter = fingers[0];
			return;
		}
		else if (isPlayStop) {
			fingerCenter = fingers[0];
			isPlayStop = false;
			return;
		}
		if (fingers.size() == 1) {
			pFingerCenter = fingerCenter;
			fingerCenter = fingers[0];
			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && fingDist == 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
				}
				shadowInsideExisted = true;
			}
			fingDist = 0;
		}
		else if (fingers.size() == 2) {
			pFingerCenter = fingerCenter;
			fingerCenter = ofVec2f((fingers[0].x + fingers[1].x) / 2, (fingers[0].y + fingers[1].y) / 2);
			pFingDist = fingDist;
			fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && abs(pFingDist) > 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					virtualShadow.updateDiameter(fingDist - pFingDist);
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
				}
				shadowInsideExisted = true;
			}
		}
		timer.stop();
		cout << "Play and Recoding gesture" << endl;
	}
	else {    //操作がない時または，操作終了時
		if (isStart) {
			if (isPlay) {
				isPlayStop = true;
				timer.start();
				if (timer.isTimeOver(5)) {    //操作終了時カウント
					homing();
					csvManager.endRec();
					timer.stop();
					isPlay = false;
					timer.sleep(5);
				}
				else {
					cout << "timer count now..." << endl;
				}
			}
			else {
				if (!isPlayBack) {    //再生処理
					isPlayBack = true;
				}
				else {
					ofVec3f p = csvManager.playbackVec3();
					plotterCont.update(p, p.z);
					cout << "Play Back now" << endl;
					if (csvManager.isEndPlayBack()) {
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

void GestureManager::update4(vector<ofVec2f> _fingers) {
	fingers = _fingers;
	if (isPlay) {
		//	csvManager.stratRec();
		//	csvManager.recoding(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter);
		//	setSphereObject(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
	}
	if (fingers.size() > 0) {
		if (!isStart) isStart = true;
		if (!isPlay) {
			isPlay = true;
			isPlayBack = false;
			csvManager.stratRec();
			fingerCenter = fingers[0];
			return;
		}
		else if (isPlayStop) {
			fingerCenter = fingers[0];
			isPlayStop = false;
			return;
		}
		if (fingers.size() == 1) {
			pFingerCenter = fingerCenter;
			fingerCenter = fingers[0];
			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && fingDist == 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
					csvManager.stratRec();
					csvManager.recoding(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter);
					setVirObjPos(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
					setSphereObject(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
					//AR view
					oscSender(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
				}
				shadowInsideExisted = true;
			}
			fingDist = 0;
		}
		else if (fingers.size() == 2) {
			pFingerCenter = fingerCenter;
			fingerCenter = ofVec2f((fingers[0].x + fingers[1].x) / 2, (fingers[0].y + fingers[1].y) / 2);
			pFingDist = fingDist;
			fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && abs(pFingDist) > 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					virtualShadow.updateDiameter(fingDist - pFingDist);
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
					csvManager.stratRec();
					csvManager.recoding(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter);
					setVirObjPos(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
					setSphereObject(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
					//AR view
					oscSender(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
				}
				shadowInsideExisted = true;
			}
		}
		else if (fingers.size() == 3) {
			pFingerCenter = fingerCenter;
			fingerCenter = ofVec2f((fingers[0].x + fingers[1].x + fingers[2].x) / 3, (fingers[0].y + fingers[1].y + +fingers[2].y) / 3);
			pFingDist = fingDist;
			fingDist = ofDist(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
			if (virtualShadow.isAreaInside(fingerCenter)) {
				if (shadowInsideExisted && abs(pFingDist) > 0) {
					virtualShadow.updateCenter(fingerCenter - pFingerCenter);
					virtualShadow.updateDiameter(fingDist - pFingDist);
					setVirObjPos(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
					plotterCont.update(virtualShadow.center, virtualShadow.diameter);
				}
				shadowInsideExisted = true;
			}
		}
		timer.stop();
		cout << "Play and Recoding gesture" << endl;
	}
	else {    //操作がない時または，操作終了時
		setVirObjPos(ofVec3f(virtualShadow.center.x, virtualShadow.center.y, virtualShadow.diameter));
		if (isStart) {
			if (isPlay) {
				isPlayStop = true;
				timer.start();
				if (timer.isTimeOver(5)) {    //操作終了時カウント
					homing();
					csvManager.endRec();
					timer.stop();
					isPlay = false;
					timer.sleep(5);
				}
				else {
					cout << "timer count now..." << endl;
				}
			}
			else {
				if (!isPlayBack) {    //再生処理
					isPlayBack = true;
				}
				else {
					ofVec3f p = csvManager.playbackVec3();
					plotterCont.update(p, p.z);
					cout << "Play Back now" << endl;
					if (csvManager.isEndPlayBack()) {
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

void GestureManager::setVirObjPos(ofVec3f p) {
	p.y = ofGetHeight() - p.y;
	virtualObject.setPosition(p.x, p.y, p.z);
}
//--------------------------------------------------------------

void GestureManager::setSphereObject(ofVec3f p) {
	ofIcoSpherePrimitive sphere;
	sphere.set(30, 1);
	p.y = ofGetHeight() - p.y;	//座標の関係上表示の際に反転させる
	sphere.setPosition(p.x, p.y, p.z);
	spheres.push_back(sphere);
}
//--------------------------------------------------------------

void GestureManager::drawVirtualObject() {
	ofPushStyle();
	ofSetColor(0, 100, 255, 50);
	virtualObject.draw();
	ofPopStyle();
}
//--------------------------------------------------------------

void GestureManager::drawVirtualShadow() {
	virtualShadow.draw();
}
//--------------------------------------------------------------

void GestureManager::drawFingers() {
	ofPushStyle();
	ofSetColor(0, 255, 100);
	if (fingers.size() == 1) {
		ofDrawEllipse(fingerCenter, 10, 10);
	}
	else if (fingers.size() == 2) {
		ofDrawEllipse(fingerCenter, 10, 10);
		ofDrawEllipse(fingers[0].x, fingers[0].y, 10, 10);
		ofDrawEllipse(fingers[1].x, fingers[1].y, 10, 10);
		ofDrawLine(fingers[0].x, fingers[0].y, fingers[1].x, fingers[1].y);
	}
	else if (fingers.size() == 3) {
		ofDrawEllipse(fingerCenter, 10, 10);
		ofDrawEllipse(fingers[0].x, fingers[0].y, 10, 10);
		ofDrawEllipse(fingers[1].x, fingers[1].y, 10, 10);
		ofDrawEllipse(fingers[2].x, fingers[2].y, 10, 10);
	}
	ofPopStyle();
}
//--------------------------------------------------------------

void GestureManager::drawSphereObjects() {
	if (spheres.size() > 0) {
		ofSetColor(255, 0, 0, 100);
		for (auto&& sp : spheres) {
			sp.draw();
		}
	}
}
//--------------------------------------------------------------

void GestureManager::saveSphereSTL() {
	if (spheres.size() > 0) {
		for (ofIcoSpherePrimitive sphere : spheres) {
			ofMesh meshs = sphere.getMesh();
			vector<ofMeshFace> faces = meshs.getUniqueFaces();
			for (ofMeshFace face : faces) {
				face.setVertex(0, face.getVertex(0) + sphere.getPosition());
				face.setVertex(1, face.getVertex(1) + sphere.getPosition());
				face.setVertex(2, face.getVertex(2) + sphere.getPosition());
				exporter.addTriangle(face.getVertex(0), face.getVertex(1), face.getVertex(2), face.getFaceNormal());
			}
		}
	}
	cout << "save STL" << endl;
	exporter.saveModel(ofFilePath::getCurrentExeDir() + "../../../model.stl");
}
//--------------------------------------------------------------

void GestureManager::saveCsv() {
	csvManager.endRec();
}
//--------------------------------------------------------------

void GestureManager::oscSender(ofVec3f p) {
	ofxOscMessage message;
	message.setAddress("/mouse/point");
	message.addFloatArg(p.x);
	message.addFloatArg(p.y);
	message.addFloatArg(p.z);
	sender.sendMessage(message);
}
//--------------------------------------------------------------

void GestureManager::homing() {
	//virtualShadow.updateCenter(virtualShadow.homePos);
	//virtualShadow.updateDiameter(virtualShadow.minDiameter);
	virtualShadow.setCenter(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2));
	//virtualShadow.setCenter(virtualShadow.homePos);
	virtualShadow.setDiameter(virtualShadow.minDiameter);
	plotterCont.update(virtualShadow.center, virtualShadow.diameter);
}
//--------------------------------------------------------------

vector<ofVec2f> GestureManager::mapingFingerPos(vector<ofVec2f> fingPos) {

	return fingPos;
}