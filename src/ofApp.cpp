#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    modules = 4;
    port = "/dev/ttyUSB0";
    channelsPerModule = 4;

    points_rgb.resize(modules);
    for (auto& l : points_rgb){
        l.resize(3);
    }

    // initialisation des points dans la matrice (module * rgb)
    points_rgb[0] = {ofPoint(0,0), ofPoint(WINDOW_WIDTH, 0), ofPoint(WINDOW_WIDTH, WINDOW_HEIGHT) };
    points_rgb[1] = {ofPoint(WINDOW_WIDTH,0), ofPoint(WINDOW_WIDTH, WINDOW_HEIGHT), ofPoint(0,WINDOW_HEIGHT) };
    points_rgb[2] = {ofPoint(WINDOW_WIDTH, WINDOW_HEIGHT), ofPoint(0, WINDOW_HEIGHT), ofPoint(0,0) };
    points_rgb[3] = {ofPoint(0,WINDOW_HEIGHT), ofPoint(0, 0), ofPoint(WINDOW_WIDTH, WINDOW_HEIGHT) };

    messageDMX.resize(modules * channelsPerModule);
    for (int i = 0; i < messageDMX.size(); i++){
        messageDMX[i] = 0;
    }

    dmx.connect(port, modules * channelsPerModule);
    dmx.update(true);
}

void ofApp::exit() {
	dmx.clear();
	dmx.update(true); // black on shutdown
}

//--------------------------------------------------------------
void ofApp::update(){

/*
    for (int i = 0; i < messageDMX.size(); i++){
        messageDMX[i] = 0;
    }
    */

    //affichage_couleur(points_rgb, ofPoint(mouse_x, mouse_y));
    affichage_couleur2(points_rgb, ofPoint(mouse_x, mouse_y));

    for (int i = 0; i < messageDMX.size(); i++){
        dmx.setLevel(i+1, messageDMX[i]);
    }

	if(dmx.isConnected()) {
		dmx.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mouse_x = x;
    mouse_y = y;
}


void ofApp::affichage_couleur(std::vector<std::vector<ofPoint>> rgb, ofPoint mouse) {
    int channel = 1;
    for (int i = 0; i < 4; i++){

        ofPoint& r = rgb[i][0];
        ofPoint& g = rgb[i][1];
        ofPoint& b = rgb[i][2];

        float dist_r = ofDist(mouse.x, mouse.y, r.x, r.y);
        float dist_g = ofDist(mouse.x, mouse.y, g.x, g.y);
        float dist_b = ofDist(mouse.x, mouse.y, b.x, b.y);

        float point_proche = std::min( {dist_r, dist_g, dist_b} );
        float point_loin = std::max( {dist_r, dist_g, dist_b}   );
        float point_autre = dist_r + dist_g + dist_b - point_proche - point_loin;

        int couleur = static_cast<int>(point_proche / point_autre * 255);

        if (point_proche == dist_r && point_loin == dist_g) {
            messageDMX[++channel] = 255;
            messageDMX[++channel]= 0;
            messageDMX[++channel]= couleur;
        } else if (point_proche == dist_g && point_loin == dist_r){
            messageDMX[++channel] = 0;
            messageDMX[++channel]= 255;
            messageDMX[++channel]= couleur;
        } else if (point_proche == dist_g && point_loin == dist_b) {
            messageDMX[++channel] = couleur;
            messageDMX[++channel]= 255;
            messageDMX[++channel]= 0;
        } else if (point_proche == dist_b && point_loin == dist_g){
            messageDMX[++channel] = couleur;
            messageDMX[++channel]= 0;
            messageDMX[++channel]= 255;
        } else if (point_proche == dist_b && point_loin == dist_r){
            messageDMX[++channel] = 0;
            messageDMX[++channel]= couleur;
            messageDMX[++channel]= 255;
        } else {
            messageDMX[++channel] = 255;
            messageDMX[++channel]= couleur;
            messageDMX[++channel]= 0;
        }
        ++channel;
    }
}

void ofApp::affichage_couleur2(std::vector<std::vector<ofPoint>> rgb, ofPoint mouse) {
    int channel = 0;
    for (int i = 0; i < modules; i++){

        ofPoint& r = rgb[i][0];
        ofPoint& g = rgb[i][1];
        ofPoint& b = rgb[i][2];

        float dist_r = ofDist(mouse.x, mouse.y, r.x, r.y);
        float dist_g = ofDist(mouse.x, mouse.y, g.x, g.y);
        float dist_b = ofDist(mouse.x, mouse.y, b.x, b.y);

        float point_proche = std::min( {dist_r, dist_g, dist_b} );
        float point_loin = std::max( {dist_r, dist_g, dist_b}   );
        float point_autre = dist_r + dist_g + dist_b - point_proche - point_loin;

        int couleur_min = static_cast<int>(255 - (point_proche / point_loin * 255));
        int couleur_mid = ((point_loin - point_proche) * point_autre) > 0 ? static_cast<int>( (point_proche * (point_loin - point_autre)) / ((point_loin - point_proche) * point_autre) * couleur_min) : 0 ;

        if (point_proche == dist_r && point_loin == dist_g) {
            messageDMX[++channel] = couleur_min;
            messageDMX[++channel]= 0;
            messageDMX[++channel]= couleur_mid;
        } else if (point_proche == dist_g && point_loin == dist_r){
            messageDMX[++channel] = 0;
            messageDMX[++channel]= couleur_min;
            messageDMX[++channel]= couleur_mid;
        } else if (point_proche == dist_g && point_loin == dist_b) {
            messageDMX[++channel] = couleur_mid;
            messageDMX[++channel] = couleur_min;
            messageDMX[++channel]= 0;
        } else if (point_proche == dist_b && point_loin == dist_g){
            messageDMX[++channel] = couleur_mid;
            messageDMX[++channel]= 0;
            messageDMX[++channel]= couleur_min;
        } else if (point_proche == dist_b && point_loin == dist_r){
            messageDMX[++channel] = 0;
            messageDMX[++channel]= couleur_mid;
            messageDMX[++channel]= couleur_min;
        } else {
            messageDMX[++channel] = couleur_min;
            messageDMX[++channel]= couleur_mid;
            messageDMX[++channel]= 0;
        }
        ++channel;
    }
}

