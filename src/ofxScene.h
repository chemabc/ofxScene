#pragma once

#include "ofMain.h"
#include "defines_main.h"


#ifdef _APPGC_OFXSIMPLEGUITOO
 #include "ofxSimpleGuiToo.h"
#endif // _APPGC_OFXSIMPLEGUITOO


enum eState {
    NO_SETUP_YET, //inactive
    SETUP_DONE, //inactive
    INITING, //active //<- enable
    NORMAL, //active
    ENDING, //active  //<-disable
    ENDING_DONE //inactive
};

enum eTransitionType{
    FADE,
    TWEEN
};

class ofxScene {
	public:
	    //Constructor
	    #ifdef _APPGC_OFXSIMPLEGUITOO
            ofxScene(ofxSimpleGuiToo *gui, string name ="", eTransitionType transitionType = FADE, float param = 1.0);
	    #else
            ofxScene(string name = "", eTransitionType transitionType = FADE, float param = 1.0);
	    #endif
	    virtual ~ofxScene()=0;

		virtual void setup() = 0;

        virtual void initializeInitScene() = 0; //INITING
        virtual void initializeEndScene() = 0; //INITING

        virtual void update() = 0;
		virtual void updateScene() = 0; //NORMAL
		virtual void updateInitScene() = 0; //INITING
		virtual void updateEndScene() = 0; //ENDING

        virtual void draw() = 0;
		virtual void drawScene() = 0; //NORMAL
		virtual void drawInitScene() = 0; //INITING
		virtual void drawEndScene() = 0; //ENDING

		///SCENE MANAGEMENT
        virtual bool isActive();
        virtual void enableScene();
		virtual void disableScene();
		virtual void endSceneCompletely();

		///LISTENERS
		void enableListeners();
        void disableListeners();
        virtual void keyPressed  (ofKeyEventArgs& args);
        virtual void keyReleased (ofKeyEventArgs& args);
        virtual void mouseMoved(ofMouseEventArgs& args);
        virtual void mouseDragged(ofMouseEventArgs& args);
        virtual void mousePressed(ofMouseEventArgs& args);
        virtual void mouseReleased(ofMouseEventArgs& args);
        virtual void windowResized(int w, int h);
        virtual void dragEvent(ofDragInfo dragInfo);
        virtual void gotMessage(ofMessage msg);

		///DEBUG
		void enableDebug();
		void disableDebug();
		string getStringDebug();


		///ACCESSORS
		int getSceneID();
		int getNumScenes();
		eState getState();
        string getSceneName();
        string getStringState();
		bool isListenersEnabled();
		void setColorBackground(ofFloatColor c);
		void setColorText(ofFloatColor c);
		void setColorTextDebug(ofFloatColor c);
		bool isThisSceneWantsToBeFinished();
		void setThisSceneWantsTobeFinished(bool b=true);



	protected:
        ///********************************************
        ///         PROTECTED METHODS
        ///********************************************
        #ifdef _APPGC_OFXSIMPLEGUITOO
        virtual void setupGui() = 0;
        #endif
        void nextState();
        void prevState();
        void resetState();

        ///********************************************
        ///         PROTECTED VARIABLES
        ///********************************************
        #ifdef _APPGC_OFXSIMPLEGUITOO
            ofxSimpleGuiToo *ptr_gui;
		#endif // _APPGC_OFXSIMPLEGUITOO
		bool b_debug;
		string s_debug;
		//Control
		bool b_firstLoopToInitDone;
		bool b_firstLoopToEndDone;
		//Transition
		float f_alphaColor;
		//Texts
		string s_font, s_fontDebug;
        ofTrueTypeFont  font_text, font_textDebug;
        int i_fontSize, i_fontSizeDebug;
        //Colors
		ofFloatColor cf_colorText, cf_colorTextDebug, cf_colorBackground;

	private:
        ///********************************************
        ///         PRIVATE METHODS
        ///********************************************


        ///********************************************
        ///         PRIVATE VARIABLES
        ///********************************************
		//Identifier
		static int i_numScenes;
		int i_ID;
		string s_sceneName;
		//State
		eState eS_state;
		//Listeners
		bool b_listenersEnabled;
		//Transition
		eTransitionType eT_transition;
		float f_paramTransition;
		float f_initTransitionTime;
		float f_transitionDuration;
		bool b_thisSceneWantsToBeFinished;
		//Parameters to begin and end







};
