#pragma once

#include "ofMain.h"

#include "ofxDmx.h"
#include "ofxGui.h"

#include <vector>
#include <algorithm>

constexpr unsigned int WINDOW_WIDTH = 1024;
constexpr unsigned int WINDOW_HEIGHT = 768;

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );


		ofxDmx dmx;
		string port;
		int modules, channelsPerModule;
		std::vector<int> messageDMX;

        int mouse_x, mouse_y;
        std::vector<std::vector<ofPoint>> points_rgb;

        void affichage_couleur(std::vector<std::vector<ofPoint>> rgb, ofPoint mouse);
        void affichage_couleur2(std::vector<std::vector<ofPoint>> rgb, ofPoint mouse);


};
