/*
 ofApp.cpp
 ofxHapPlayerExample
 
 Copyright (c) 2013, Tom Butterworth. All rights reserved.
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ofApp.h"
//
//#define BarInset 20
//#define BarHeight 40
#include <sys/resource.h>
//--------------------------------------------------------------
void ofApp::setup(){
    // Limit drawing to a sane rate
    ofSetVerticalSync(true);
    
    ofBackground(0);
    
    
    // Load a movie file
    loadDirectory("movies");
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // Show or hide the cursor and position bar
//    if (ofGetSystemTimeMillis() - lastMovement < 3000)
//    {
//        drawBar = true;
//    }
//    else
//    {
//        drawBar = false;
//    }
//    ofRectangle window = ofGetWindowRect();
//    if (!drawBar && window.inside(ofGetMouseX(), ofGetMouseY()))
//    {
//        ofHideCursor();
//    }
//    else
//    {
//        ofShowCursor();
//    }
}
void setMaxOpenFiles(size_t maxOpenFiles){
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        std::cout << "File Descriptor limits: \n";
        std::cout << "    Soft limit: " << rlim.rlim_cur << std::endl;
        std::cout << "    Hard limit: " << rlim.rlim_max << std::endl;
    } else {
        ofLogError("setMaxOpenFiles") << "Unable to get file descriptor limits";
    }
    if(rlim.rlim_max <= maxOpenFiles){
        ofLogError("setMaxOpenFiles") << "Cant set the limit above the max allowed: " << rlim.rlim_max;
        return;
    }
    rlim.rlim_cur = maxOpenFiles + 256; // just add a margin so the app can still open stuff 
    if (setrlimit(RLIMIT_NOFILE, &rlim) == -1) {
        std::cout << "Unable to set file descriptor limits" << std::endl;
    }else{
        ofLogVerbose("setMaxOpenFiles") << "Successfuly set the limit to " << maxOpenFiles;
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    for(auto& p: players){
        if(p){
            if(p->isLoaded() && p->rect.isEmpty()){
                makePlayersRects();
            }
            p->draw();
        }
    }
}
void ofApp::loadDirectory(std::string dirPath)
{
    dir.listDir(ofToDataPath(dirPath));
//    if(!dir.isDirectory()){
//        ofLogError("ofApp::loadDirectory") << dirPath << "  not a directory";
//        return;
//    }
    setMaxOpenFiles(dir.size()+256);
    players.clear();
    for(size_t i = 0; i < dir.size() ; i++){
        addPlayer(i);
    }
    makePlayersRects();
}

//--------------------------------------------------------------
bool ofApp::addPlayer(size_t index){
    if(index >= dir.size()) {
        cout << "ofApp::addPlayer : index >= dir.size()\n";
        return false;
    }
    cout << "loading player " << index << endl;
    players.emplace_back(make_unique<ofxHapPlayerWithRect>());
    if(players.back()->load(dir.getPath(index))){
        players.back()->setLoopState(OF_LOOP_NORMAL);
        players.back()->play();
        lastLoadedIndex = index;
        return true;
    }else{
        ofLogError("ofApp::loadDirectory") << "Failed loading file " << dir.getPath(index);
        players.pop_back();
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::makePlayersRects(){
    float maxRatio = 0;

    float x = 0;
    float y = 0;
    for(auto& p: players){
        if(p &&  p->isLoaded()){
            float r =  p->getWidth() / p->getHeight();
            if(maxRatio < r) maxRatio = r;
        }
    }
    auto wr = ofGetWindowRect();
    //this is a bit dumb but works. could be deterministic
    size_t numX = 0;
    size_t numY =0;
    
    for(size_t i = 1; i < players.size()+1; i ++){
        float w = (wr.width/(float)i);
        float h = w  * (1/maxRatio);
        numY= (size_t)ceil(players.size()/(float)i);
    
        if(numY * h <= wr.height){
            numX = i;
            break;
        }
    }
    
    cout << "Num X: " << numX << " num Y: " << numY <<endl;
    if(numX == 0 || numY == 0){
        cout << "makeRects failed\n";
        return;
    }
    
    ofRectangle unitRect (0,0, wr.width/numX, wr.height/numY);
    
    
    ofRectangle targetRect;
    for(size_t y = 0; y < numY; y++){
        for(size_t x = 0; x < numX; x++){
            targetRect = unitRect;
            targetRect.x = x * unitRect.width;
            targetRect.y = y * unitRect.height;
            size_t i = y * numX + x;
            if(i < players.size()){
                players[i]->scaleTo(targetRect);
            }else{
                break;
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ')
    {
        if(addPlayer(lastLoadedIndex+1)){
            makePlayersRects();
        }
//        player.setPaused(!player.isPaused());
    }
//    else if (key == OF_KEY_UP)
//    {
//        player.setVolume(player.getVolume() + 0.1);
//    }
//    else if (key == OF_KEY_DOWN)
//    {
//        player.setVolume(player.getVolume() - 0.1);
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
//    if (ofGetWindowRect().inside(x, y))
//    {
//        lastMovement = ofGetSystemTimeMillis();
//    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
//    if (inScrub)
//    {
//        float position = static_cast<float>(x - BarInset) / getBarRectangle().width;
//        position = std::max(0.0f, std::min(position, 1.0f));
//        player.setPosition(position);
//        lastMovement = ofGetSystemTimeMillis();
//    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//    ofRectangle bar = getBarRectangle();
//    if (bar.inside(x, y))
//    {
//        inScrub = true;
//        wasPaused = player.isPaused() || player.getIsMovieDone();
//        player.setPaused(true);
//        mouseDragged(x, y, button);
//    }
//    lastMovement = ofGetSystemTimeMillis();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
//    if (inScrub)
//    {
//        inScrub = false;
//        player.setPaused(wasPaused);
//    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
//	vector< string > fileList = dragInfo.files;
//    load(fileList[0]);
}

//ofRectangle ofApp::getBarRectangle() const
//{
//    return ofRectangle(BarInset, ofGetWindowHeight() - BarInset - BarHeight, ofGetWindowWidth() - (2 * BarInset), BarHeight);
//}
