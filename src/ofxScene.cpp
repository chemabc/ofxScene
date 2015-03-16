#include "ofxScene.h"

///********************************************
///         STATIC VARIABLES
///********************************************
int ofxScene::i_numScenes = 0;

///********************************************
///         CONSTRUCTOR
///********************************************
#ifdef _APPGC_OFXSIMPLEGUITOO
//    ofxScene::ofxScene(ofxSimpleGuiToo *gui, string name, eTransitionType transitionType, float param ){
//        ofLogVerbose("[ofxScene::ofxScene]");
//        ptr_gui = gui;
//        s_sceneName = name;
//        i_ID = i_numScenes;
//        eT_transition = transitionType;
//        f_paramTransition = param;
//        i_numScenes++;
//    }
    ofxScene::ofxScene(ofxEtcGuiControl *gui, string name, eTransitionType transitionType, float param ){
        ofLogVerbose("[ofxScene::ofxScene]");
        ptr_gui = gui;
        s_sceneName = name;
        i_ID = i_numScenes;
        eT_transition = transitionType;
        f_paramTransition = param;
        i_numScenes++;
    }
#else
    ofxScene::ofxScene(string name, eTransitionType transitionType, float param){
        ofLogVerbose("[ofxScene::ofxScene]");
        s_sceneName = name;
        i_ID = i_numScenes;
        eT_transition = transitionType;
        f_paramTransition = param;
        i_numScenes++;
    }
#endif

ofxScene::~ofxScene(){
    disableListeners();
    f_alphaColor = 0.0;
    #ifdef _APPGC_OFXSIMPLEGUITOO
    ptr_gui = 0;
    #endif
    i_numScenes--;
}

///********************************************
///         SETUP, INIT, END
///********************************************
void ofxScene::setup(){
    ofLogVerbose("[ofxScene::setup]");
    eS_state = NO_SETUP_YET;
    b_listenersEnabled = false;
    f_alphaColor = 0.0;
    s_font = "";
    s_fontDebug = "";
    cf_colorBackground = ofColor::black;
    cf_colorText = ofColor::white;
    cf_colorTextDebug = ofColor::whiteSmoke;
    b_thisSceneWantsToBeFinished = false;
    b_firstLoopToInitDone = false;
    b_firstLoopToEndDone = false;


    nextState();
    #ifdef _APPGC_OFXSIMPLEGUITOO
    setupGui();
    #endif

    if(s_font != "")
        font_text.loadFont(s_font, i_fontSize, true, true);
    if(s_fontDebug != "")
        font_textDebug.loadFont(s_fontDebug, i_fontSizeDebug, true, true);
}
///********************************************
///         UPDATE
///********************************************
void ofxScene::update(){
    s_debug = "";
    s_debug = "-State: "+getSceneName()+" ; state: " + getStringState() + "\n";
    switch(eS_state){
        case NO_SETUP_YET:

            break;
        case SETUP_DONE:

            break;
        case INITING:
            updateInitScene();
            break;
        case NORMAL:
            updateScene();
            break;
        case ENDING:
            updateEndScene();
            break;
        case ENDING_DONE:

            break;
    }
}

void ofxScene::initializeInitScene() { //Only one time per Scene loop
    ofLogVerbose("***************** initializeInitScene (" + getSceneName() + ")" + ofToString(ofGetElapsedTimef(),1) + "***********" );
    b_firstLoopToEndDone = false;
    b_firstLoopToInitDone = true;
    eS_state = SETUP_DONE;
    f_alphaColor = 0.0;

}
void ofxScene::updateInitScene(){
    s_debug += "    [ofxScene::updateInitScene] \n";

    if(!b_firstLoopToInitDone){
        initializeInitScene();
    }

    switch(eT_transition){
        case FADE:
            f_alphaColor += f_paramTransition;
            if(f_alphaColor >= 255.0){
                nextState();
            }
            break;
        case TWEEN:
            nextState();
            break;
    }


}

void ofxScene::updateScene(){
    s_debug += "    [ofxScene::updateScene] \n";

}
void ofxScene::initializeEndScene() { //Only one time per Scene loop
    ofLogVerbose("***************** initializeEndScene (" + getSceneName() + ")" + ofToString(ofGetElapsedTimef(),1) + "***********");
    b_firstLoopToInitDone = false;
    b_firstLoopToEndDone = true;

}
void ofxScene::updateEndScene(){
    s_debug += "    [ofxScene::updateEndScene] \n";
     if(!b_firstLoopToEndDone){
        initializeEndScene();
    }
    switch(eT_transition){
        case FADE:
            f_alphaColor -= f_paramTransition;
             if(f_alphaColor <= 0.0){
                    //Destroy objects
                    //Change state
                    nextState();
                    endSceneCompletely();
                }
            break;
        case TWEEN:
            nextState();
            break;
    }



}
///********************************************
///         DRAW
///********************************************
void ofxScene::draw(){
    s_debug += "    [ofxScene::draw] (alpha="+ofToString(f_alphaColor,2)+") \n";
    ofPushStyle();
        ofEnableAlphaBlending();

        ofSetColor(255,255,255,f_alphaColor);

        switch(eS_state){
            case NO_SETUP_YET:

                break;
            case SETUP_DONE:

                break;
            case INITING:
                drawInitScene();
                break;
            case NORMAL:
                drawScene();
                break;
            case ENDING:
                drawEndScene();
                break;
            case ENDING_DONE:

                break;
        }


        ofDisableAlphaBlending();
    ofPopStyle();
}

void ofxScene::drawInitScene(){
     s_debug += "   [ofxScene::drawInitScene] \n";

//    ofPushStyle();
//    ofSetColor(0,255,0, f_alphaColor);
//    ofFill();
//    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 30);
//    ofPopStyle();
}
void ofxScene::drawScene(){
    s_debug += "    [ofxScene::drawScene] \n";

//    ofPushStyle();
//    ofSetColor(255,0,0, f_alphaColor);
//    ofFill();
//    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 30);
//    ofPopStyle();

}
void ofxScene::drawEndScene(){
     s_debug += "   [ofxScene::drawEndScene] \n";

//    ofPushStyle();
//    ofSetColor(0,0,255, f_alphaColor);
//    ofFill();
//    ofCircle(ofGetWidth()/2, ofGetHeight()/2, 30);
//    ofPopStyle();
}
///********************************************
///         SCENE MANAGEMENT
///********************************************

bool ofxScene::isActive(){
    switch(eS_state){
        case NO_SETUP_YET:
        case SETUP_DONE:
        case ENDING_DONE:
            return false;
        break;

        case INITING:
        case NORMAL:
        case ENDING:
            return true;
        break;
    }
}

void ofxScene::enableScene(){
    if(b_debug)ofLogVerbose("[ofxScene::enableScene] Enabling scene " + getSceneName() + "");
    if(!isActive()){
        eS_state = INITING;
        f_initTransitionTime = ofGetElapsedTimef();
        f_alphaColor=0.0;
    }
}
void ofxScene::disableScene(){
    if(b_debug)ofLogVerbose("[ofxScene::disableScene] Disabling scene " + getSceneName() + "");
    if(isActive()){
        eS_state = ENDING;
    }
}

void ofxScene::endSceneCompletely(){
    if(b_debug)ofLogVerbose("[ofxScene::endSceneCompletely] END completely scene " + getSceneName() + "");
     eS_state = SETUP_DONE;
}
///********************************************
///         STATES
///********************************************
void ofxScene::nextState(){
    string sNotice = "[ofxScene::nextState] Scene " + getSceneName() + " from " + getStringState() + " to ";

     switch(eS_state){
        case NO_SETUP_YET:
            eS_state = SETUP_DONE;
            break;
        case SETUP_DONE:
            eS_state = INITING;
            break;
        case INITING:
            eS_state = NORMAL;
            break;
        case NORMAL:
            eS_state = ENDING;
            break;
        case ENDING:
            eS_state = ENDING_DONE;
            break;
        case ENDING_DONE:
            eS_state = ENDING_DONE;
            break;
    }
    sNotice += getStringState();
    ofLogVerbose(""+ sNotice);
}
void ofxScene::prevState(){
    string sNotice = "[ofxScene::prevState] Scene " +getSceneName() + " from " + getStringState() + " to ";
    switch(eS_state){
        case NO_SETUP_YET:
            eS_state = NO_SETUP_YET;
            break;
        case SETUP_DONE:
            eS_state = NO_SETUP_YET;
            break;
        case INITING:
            eS_state = SETUP_DONE;
            break;
        case NORMAL:
            eS_state = INITING;
            break;
        case ENDING:
            eS_state = NORMAL;
            break;
        case ENDING_DONE:
            eS_state = ENDING;
            break;
    }
    sNotice += getStringState();
    ofLogVerbose(""+ sNotice );
}
void ofxScene::resetState(){
    eS_state = NO_SETUP_YET;
}
///********************************************
///         LISTENERS
///********************************************
void ofxScene::enableListeners(){

    b_listenersEnabled = true;
}
void ofxScene::disableListeners(){

    b_listenersEnabled = false;
}

void ofxScene::keyPressed  (ofKeyEventArgs& args){}
void ofxScene::keyReleased (ofKeyEventArgs& args){}
void ofxScene::mouseMoved(ofMouseEventArgs& args){}
void ofxScene::mouseDragged(ofMouseEventArgs& args){}
void ofxScene::mousePressed(ofMouseEventArgs& args){}
void ofxScene::mouseReleased(ofMouseEventArgs& args){}
void ofxScene::windowResized(int w, int h){}
void ofxScene::dragEvent(ofDragInfo dragInfo){}
void ofxScene::gotMessage(ofMessage msg){}

///********************************************
///         ACCESSORS
///********************************************
int ofxScene::getSceneID(){
    return i_ID;
}
string ofxScene::getSceneName(){
    return s_sceneName;
}
int ofxScene::getNumScenes(){
    return i_numScenes;
}
eState ofxScene::getState(){
    return eS_state;
}
string ofxScene::getStringState(){
    string s_state= "";

    switch(eS_state){
        case NO_SETUP_YET:
            s_state = "NO_SETUP_YET";
            break;
        case SETUP_DONE:
            s_state = "SETUP_DONE";
            break;
        case INITING:
            s_state = "INITING";
            break;
        case NORMAL:
            s_state = "NORMAL";
            break;
        case ENDING:
            s_state = "ENDING";
            break;
        case ENDING_DONE:
            s_state = "ENDING_DONE";
            break;
    }
    return s_state;
}

void ofxScene::setColorBackground(ofFloatColor c){
    cf_colorBackground = c;
}
void ofxScene::setColorText(ofFloatColor c){
    cf_colorText = c;
}
void ofxScene::setColorTextDebug(ofFloatColor c){
    cf_colorTextDebug = c;
}
bool ofxScene::isThisSceneWantsToBeFinished(){
    return b_thisSceneWantsToBeFinished;
}
void ofxScene::setThisSceneWantsTobeFinished(bool b){
    b_thisSceneWantsToBeFinished = b;
}

///********************************************
///         DEBUG
///********************************************
void ofxScene::enableDebug(){
    b_debug = true;
}
void ofxScene::disableDebug(){
    b_debug = false;
}
string ofxScene::getStringDebug(){
    return s_debug;
}
bool ofxScene::isChanging(){
    if(eS_state == NORMAL) return false;
    else return true;
}
///********************************************
///         GUI
///********************************************
#ifdef _APPGC_OFXSIMPLEGUITOO
    void ofxScene::setupGui(){
        if(ptr_gui!=0){
            ptr_gui->addPage(s_sceneName);
            ptr_gui->addToggle("Debug", b_debug);
            ptr_gui->addTitle("Automatic Details");
            ptr_gui->addSlider("AlphaDynamic", f_alphaColor, 0.0, 255.0);
            ptr_gui->addTitle("Background");
            ptr_gui->addColorPicker("BackgroundColor", cf_colorBackground);
            ptr_gui->addTitle("Texts");
            ptr_gui->addInputString("FontType", s_font);
            ptr_gui->addSlider("FontSize", i_fontSize, 0, 255);
            ptr_gui->addColorPicker("FontColor", cf_colorText);
            ptr_gui->addInputString("FontTypeDebug", s_fontDebug);
            ptr_gui->addSlider("FontSizeDebug", i_fontSizeDebug, 0, 255);
            ptr_gui->addColorPicker("FontColorDebug", cf_colorTextDebug);

            //ptr_gui->loadFromXML();
        }
    }
#endif // _APPGC_OFXSIMPLEGUITOO
