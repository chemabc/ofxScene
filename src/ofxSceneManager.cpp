#include "ofxSceneManager.h"

///********************************************
///         STATIC VARIABLES
///********************************************

///********************************************
///         CONSTRUCTOR
///********************************************
 #ifdef _APPGC_OFXSIMPLEGUITOO
//    ofxSceneManager::ofxSceneManager(ofxSimpleGuiToo *gui){
//        ptr_gui = gui;
//        i_currentScene = 0;
//        i_newScene = i_currentScene;
//    }
    ofxSceneManager::ofxSceneManager(ofxEtcGuiControl *gui){
        ptr_gui = gui;
        i_currentScene = 0;
        i_newScene = i_currentScene;
    }
#else
    ofxSceneManager::ofxSceneManager(){
        i_newScene = i_currentScene;
    }
#endif

///********************************************
///         SETUP
///********************************************
void ofxSceneManager::setup(){
    std::map<int, ofxScene*>::iterator it;
    for(it=map_scenes.begin(); it != map_scenes.end(); ++it){
        it->second->setup();
    }
}
///********************************************
///         UPDATE
///********************************************
void ofxSceneManager::update(){
    s_debug = "[ofxSceneManager]:";
    s_debug += " Key: \n";
    s_debug += " Number of added scenes: " + ofToString(map_scenes.size()) +"\n";
    s_debug += "    Current Scene= " + ofToString(i_currentScene) + ";\n";
    s_debug += "    Next= " + ofToString(i_newScene);

    //Check curretn scene is working
    bool bCurrentCorrect = false;
    std::map<int, ofxScene*>::iterator it_current;
    it_current=map_scenes.find(i_currentScene);
    if(it_current!=map_scenes.end())  bCurrentCorrect = true;

    if(bCurrentCorrect){
        if(i_currentScene == i_newScene){
            if(!it_current->second->isActive()){
                it_current->second->enableScene();
            }
            //Continue as it is
            it_current->second->update();
        }else{ ///Change to new
            ///1) Disable current
            if(it_current->second->isActive()){
                if(it_current->second->getState()!=ENDING){
                    it_current->second->disableScene();
                }
                it_current->second->update();
            }else{
                ///2) Update por ultima vez para que los datos de debug sean coherentes
                it_current->second->update();
                ///3) Enable new scene(when it_current stopped to be active)
                bool bNewCorrect = false;
                std::map<int, ofxScene*>::iterator it_new;
                it_new = map_scenes.find(i_newScene);
                if(it_new!=map_scenes.end())  bNewCorrect = true;
                if(bNewCorrect){
                    //Enable New
                    if(!it_new->second->isActive()){
                        it_new->second->enableScene();
                        i_currentScene = i_newScene;
                        it_new->second->update();
                    }
                }else{ //si no es correcto activa current y a seguir
                    it_current->second->enableScene();
                    it_current->second->update();
                }
            }
        }
        ///CHECK IF IT IS ONE SCENE THE ONE THAT WANTS TO BE FINISHED
        if(it_current->second->isThisSceneWantsToBeFinished()){
            //1) Look for the place in the vector;
            int iVectorIndex = -1; int iPos = 0;
            std::vector<int>::iterator it_vector;
            for(it_vector=vec_keysScenes.begin(); it_vector!=vec_keysScenes.end(); ++it_vector){
                if(*it_vector==i_currentScene){
                    iVectorIndex=iPos;
                    break;
                }
                iPos++;
            }
            //2)Coge la key del mapa
            int iKeyMap = -1;
            if( (iVectorIndex != -1) && vec_keysScenes.size()>0){
                    if(iVectorIndex<vec_keysScenes.size()-1){ //vete al siguiente
                        iKeyMap = vec_keysScenes.at(iVectorIndex+1);
                    }else if(iVectorIndex==vec_keysScenes.size()-1) {//Vete al inicio
                        iKeyMap = vec_keysScenes.at(0);
                    }else{
                        ofLogError("[ofxSceneManager::update] No hay sitio adonde ir y me quedo. VecSize:"+ofToString(vec_keysScenes.size()));
                    }
                    //3)Cuando la has cogido cambia de escena
                    if(iKeyMap != -1){
                        std::map<int, ofxScene*>::iterator it_next;
                        it_current->second->setThisSceneWantsTobeFinished(false);
                        ofLogVerbose( "[ofxSceneManager::update] Escena " + it_current->second->getSceneName() + " sin querer morir para la proxima interaccion. " + ofToString(ofGetElapsedTimef(),1));
                        it_next = map_scenes.find(iKeyMap);
                        if(it_next!=map_scenes.end()){
                            enableScene(iKeyMap);
                        }else{
                            ofLogError("[ofxSceneManager::update] Key " + ofToString(iKeyMap) + " no se encuentra en el mapa. MapSize:"+ofToString(map_scenes.size()));
                        }
                    }


            }else{
                ofLogError("[ofxSceneManager::update] Una escena internamente quiere acabar pero no se sabe a donde quiere ir. VecSize:"+ofToString(vec_keysScenes.size()));
            }
        }
    }else{
            ofLogWarning("[ofxSceneManager::update] Bad index of current scene: current="+ ofToString(i_currentScene) +"; new=" + ofToString(i_newScene));
    }





}
///********************************************
///         DRAW
///********************************************
void ofxSceneManager::draw(){
     //Check curretn scene is working
    bool bCurrentCorrect = false;
    std::map<int, ofxScene*>::iterator it_current;
    it_current=map_scenes.find(i_currentScene);
    if(it_current!=map_scenes.end())  bCurrentCorrect = true;

    if(bCurrentCorrect)it_current->second->draw();

}
///********************************************
///         SCENE MANAGEMENT
///********************************************
void ofxSceneManager::addScene(int i, ofxScene *scene){
    map_scenes.insert(pair<int, ofxScene*> (i, scene));
    vec_keysScenes.push_back(i);
}
void ofxSceneManager::removeScene(int i){
    std::map<int, ofxScene*>::iterator it;
    it = map_scenes.find(i);
    if( it!= map_scenes.end()){
        delete it->second; //End the scene
        map_scenes.erase(it); //Remove from map
    }
    std::vector<int>::iterator it_vector;
    for(it_vector=vec_keysScenes.begin(); it_vector!=vec_keysScenes.end(); ++it_vector){
        if(*it_vector==i){
            it_vector = vec_keysScenes.erase(it_vector);
        }
    }
}
void ofxSceneManager::enableScene(int i){
    bool bNewCorrect = false;
    std::map<int, ofxScene*>::iterator it_new;
    it_new = map_scenes.find(i);
    if(it_new!=map_scenes.end())  bNewCorrect = true;

    if(bNewCorrect){
            i_newScene = i;
            ofLogVerbose( "[ofxSceneManager::enableScene] Escena " + ofToString( i_newScene) + " being enabled. " + ofToString(ofGetElapsedTimef(),1));
    }
    else ofLogWarning("[ofxSceneManager::enableScene] Bad index of current scene; new=" + ofToString(i));
}
///********************************************
///         DEBUG
///********************************************
void ofxSceneManager::enableDebug(){
    b_debug = true;
    std::map<int, ofxScene*>::iterator it;
    for(it=map_scenes.begin(); it != map_scenes.end(); ++it){
        it->second->enableDebug();
    }
}
void ofxSceneManager::disableDebug(){
    b_debug = false;
    std::map<int, ofxScene*>::iterator it;
    for(it=map_scenes.begin(); it != map_scenes.end(); ++it){
        it->second->disableDebug();
    }
}
bool ofxSceneManager::isDebugEnabled(){
    return b_debug;
}
string ofxSceneManager::getStringDebug(){
    std::map<int, ofxScene*>::iterator it;
    for(it=map_scenes.begin(); it != map_scenes.end(); ++it){
        s_debug += "\n" + it->second->getStringDebug();
    }
    return s_debug;
}

bool ofxSceneManager::isSceneChanging(){
    bool bCurrentCorrect = false;
    std::map<int, ofxScene*>::iterator it_current;
    it_current=map_scenes.find(i_currentScene);
    if(it_current!=map_scenes.end())  bCurrentCorrect = true;

    if(bCurrentCorrect){
        return it_current->second->isChanging();
    }

}
///********************************************
///         LISTENERS
///********************************************
void ofxSceneManager::enableListeners(){}
void ofxSceneManager::disableListeners(){}

void ofxSceneManager::keyPressed  (ofKeyEventArgs& args){}
void ofxSceneManager::keyReleased (ofKeyEventArgs& args){}
void ofxSceneManager::mouseMoved(ofMouseEventArgs& args){}
void ofxSceneManager::mouseDragged(ofMouseEventArgs& args){}
void ofxSceneManager::mousePressed(ofMouseEventArgs& args){}
void ofxSceneManager::mouseReleased(ofMouseEventArgs& args){}
void ofxSceneManager::windowResized(int w, int h){}
void ofxSceneManager::dragEvent(ofDragInfo dragInfo){}
void ofxSceneManager::gotMessage(ofMessage msg){}

///********************************************
///         ACCESSORS
///********************************************
