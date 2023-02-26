
#include "GamePlayController.h"
// This is NOT in the same directory
using namespace std;
using namespace cugl;

GamePlayController::GamePlayController(const Size displaySize){
    // initialize character, two maps, path
}

void GamePlayController::update(float dt){
    _input->update(dt);
    if(_input->didPress()){
        // if press, determine if press on character
        cout<<_input->getPosition().toString()<<endl;
    }
    
    else if (_input->isDown()){
        
    }
    // start record the positions
    
    // if down and moving, draw the path along the way
    
    // if up, update path object,
    
        // determine if the path is valid,
        // if valid, start move the character along the path
    
}


