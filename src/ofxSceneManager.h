#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "defines_main.h"


#ifdef _APPGC_OFXSIMPLEGUITOO
 //#include "ofxSimpleGuiToo.h"
 #include "ofxEtcGuiControl.h"
#endif // _APPGC_OFXSIMPLEGUITOO


class ofxSceneManager {

    public:
        #ifdef _APPGC_OFXSIMPLEGUITOO
            //ofxSceneManager(ofxSimpleGuiToo *gui);
            ofxSceneManager(ofxEtcGuiControl *gui);
        #else
            ofxSceneManager();
        #endif

        void setup();
        void update();
        void draw();

        ///SCENE MANAGEMENT
        void addScene(int i, ofxScene *scene);
        void removeScene(int i);
        void enableScene(int i);


        ///LISTENERS
		void enableListeners();
        void disableListeners();
        void keyPressed  (ofKeyEventArgs& args);
        void keyReleased (ofKeyEventArgs& args);
        void mouseMoved(ofMouseEventArgs& args);
        void mouseDragged(ofMouseEventArgs& args);
        void mousePressed(ofMouseEventArgs& args);
        void mouseReleased(ofMouseEventArgs& args);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);



        ///DEBUG
		void enableDebug();
		void disableDebug();
		bool isDebugEnabled();
		string getStringDebug();

		bool isSceneChanging();



    private:
        ///********************************************
        ///         PRIVATE METHODS
        ///********************************************
        #ifdef _APPGC_OFXSIMPLEGUITOO
            void setupGui();
        #endif
        ///********************************************
        ///         PRIVATE VARIABLES
        ///********************************************
        bool b_debug;
		string s_debug;

        #ifdef _APPGC_OFXSIMPLEGUITOO
            //ofxSimpleGuiToo *ptr_gui;
            ofxEtcGuiControl *ptr_gui;
		#endif // _APPGC_OFXSIMPLEGUITOO

		//Scene map
		vector<int> vec_keysScenes;
		map<int, ofxScene *> map_scenes;
		int i_currentScene, i_newScene;

};
