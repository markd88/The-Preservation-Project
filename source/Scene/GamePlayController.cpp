
#include "GamePlayController.h"
// This is NOT in the same directory
using namespace std;
using namespace cugl;
#define PHYSICS_SCALE 50


GamePlayController::GamePlayController(const Size displaySize):_scene(cugl::Scene2::alloc(displaySize))
{
    // initialize character, two maps, path
    _character = make_unique<CharacterController>(_scene->getSize()/2);

    _character->addChildTo(_scene);
    
}

void GamePlayController::update(float dt){
    _input->update(dt);
    if(_input->didPress()){
        // if press, determine if press on character
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);

        
        if(_character->contains(input_posi)){
            // create path
        }
        
    }
    
    else if (_input->isDown()){
        // if path not null, then update path
    }
    
    else if(_input->didRelease()){
        // if path not null, determine if path is valid
        // start moving character
    }
    
    // if character is moving, move it 
    
}


