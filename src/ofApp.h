
#pragma once

#include "ofMain.h"
#include "ofxHapPlayer.h"

class ofxHapPlayerWithRect: public ofxHapPlayer{
public:
    
    ofRectangle rect;
    virtual void draw(float x, float y) override{
        draw(x, y, getWidth(), getHeight());
    }
    virtual void draw(float x, float y, float width, float height) override{
        rect.set(x,y, width, height);
        ofxHapPlayer::draw(x, y, width, height);
    }
    
    void scaleTo(const ofRectangle& targetRect){
        if(isLoaded()){
            rect.set(0,0, getWidth(), getHeight());
            rect.scaleTo(targetRect);
        }
    }
    void draw(){
        if(rect.isEmpty()){
            rect.set(0,0, getWidth(), getHeight());
        }
        if (isLoaded())
        {
            // Draw the video frame
            ofSetColor(255, 255, 255);
            ofxHapPlayer::draw(rect.x, rect.y, rect.width, rect.height);
        }
        else
        {
            if (getError().length())
            {
                ofDrawBitmapStringHighlight(getError(), rect.x, rect.y + 20);
            }
            else
            {
                ofDrawBitmapStringHighlight("Movie is loading...", rect.x, rect.y + 20);
            }
        }
        
    }
    
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

        void loadDirectory(std::string dirPath);
//        ofRectangle getBarRectangle() const;
        vector<unique_ptr<ofxHapPlayerWithRect>> players;
    
    void makePlayersRects();
    bool addPlayer(size_t index);
    
    ofDirectory dir;
    size_t lastLoadedIndex = 0;
        
    
    
    
    
//        uint64_t lastMovement;

//    bool wasPaused;
//        bool drawBar;
//        bool inScrub;
};
