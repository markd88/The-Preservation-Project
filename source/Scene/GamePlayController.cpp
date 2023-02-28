#include <chrono>
#include "GamePlayController.h"
// This is NOT in the same directory
using namespace std;
using namespace cugl;
#define PHYSICS_SCALE 50
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

GamePlayController::GamePlayController(const Size displaySize):_scene(cugl::Scene2::alloc(displaySize))
{
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution

    _input->init(dimen);
    
    _path = make_unique<PathController>();
    
    // Allocate the manager and the actions
    _actions = cugl::scene2::ActionManager::alloc();
    
    // initialize character, two maps, path
    
    
    _tilemap1 = std::make_unique<TilemapController>();
    generatePrimaryWorld(_tilemap1);
    _tilemap1->addChildTo(_scene);
    _activeMap = "tileMap1";
    
    _tilemap2 = std::make_unique<TilemapController>();
    generateSecondaryWorld(_tilemap2);

    _template = 0;
    
    _character = make_unique<CharacterController>(_scene->getSize()/2, _actions);

    _character->addChildTo(_scene);
}

void GamePlayController::update(float dt){
    _input->update(dt);
    // if pinch, switch world
    if(_input->getPinchDelta() < -90 && _input->getPinchDelta() > -130){
        CULog("didPinch +++++++++++++");
        std::cout<<"pinch delta = "<< _input->getPinchDelta()<<std::endl;
        if (_activeMap == "tileMap1") {
            _tilemap1->removeChildFrom(_scene);
            _tilemap2->addChildTo(_scene);
            _activeMap = "tileMap2";
        }
        else {
            _tilemap2->removeChildFrom(_scene);
            _tilemap1->addChildTo(_scene);
            _activeMap = "tileMap1";
        }

    } else if(_input->didPress()){
        CULog("didPress");
        // if press, determine if press on character
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);
        
        
        if(_character->contains(input_posi)){
            // create path
            CULog("here");
            _path->setIsDrawing(true);
            _path->updateLastPos(input_posi);
            
        }
        
    }
    
    else if (_input->isDown() && _path->_isDrawing){
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);
        if (_path->farEnough(input_posi)){
            _path->addSegment(input_posi, _scene);
            _path->updateLastPos(input_posi);
        }
    }
    
    else if(_input->didRelease()){
        CULog("didRelease");
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);
        _path->setIsDrawing(false);
        path_trace = _path->_model->Path;

        _moveTo = cugl::scene2::MoveTo::alloc();
        _moveTo->setDuration(.25);
        _path->clearPath(_scene);
        
    }
    
    else if (path_trace.size() != 0 && !_actions->isActive("moving")){
        _moveTo->setTarget(path_trace[0]);
       
        _character->moveTo(_moveTo);
        path_trace.erase(path_trace.begin());
    }
    
    
    // Animate
    _actions->update(dt);

}
    
    
#pragma mark Main Methods

    
#pragma mark -
#pragma mark Generation Helpers
    
    /** Generates the first world. */
    void GamePlayController::generatePrimaryWorld(std::unique_ptr<TilemapController> &_tilemap) {
        _tilemap->updateDimensions(Vec2(65, 35));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(15, 15));
        _tilemap->updatePosition(_scene->getSize()/2);
        
        Color4 tileColor = Color4::BLACK;
        // lower-right line
        for(int i = 65; i >= 10; i--) {
            _tilemap->addTile(i, 12, tileColor);
        }
        // upper-left line
        for(int i = 0; i <= 50; i++) {
            _tilemap->addTile(i, 23, tileColor);
        }
        
        // lower guard
        tileColor = Color4::RED;
        for (int i = 2; i <= 7; i++) {
            for(int j = 7; j <= 17; j++) {
                _tilemap->addTile(i, j, tileColor);
            }
        }
        
        // upper guard
        for (int i = 50; i <= 60; i++) {
            for(int j = 15; j <= 20; j++) {
                _tilemap->addTile(i, j, tileColor);
            }
        }
        
    }
    
    /**
     * Generates the second world.
     *
     * @param p The probability that a tile is generated.
     */
    void GamePlayController::generateSecondaryWorld(std::unique_ptr<TilemapController> & _tilemap) {
        _tilemap->updateDimensions(Vec2(65, 35));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(15, 15));
        _tilemap->updatePosition(_scene->getSize()/2);
        
        Color4 tileColor = Color4::BLACK;
        
        // lower block
        for (int i = 65; i >= 10; i--) {
            for(int j = 0; j <= 12; j++) {
                _tilemap->addTile(i, j, tileColor);
            }
        }
        
        // middle block
        for (int i = 22; i <= 27; i++) {
            for(int j = 12; j <= 23; j++) {
                _tilemap->addTile(i, j, tileColor);
            }
        }
        
        // upper block
        for (int i = 0; i <= 50; i++) {
            for(int j = 35; j >= 23; j--) {
                _tilemap->addTile(i, j, tileColor);
            }
        }
        
    }
    
    
    
#pragma mark -
#pragma mark Helpers
    /**
     * Creates a new tile map with the given template number
     *
     * @param template    The template number
     */
//    void GamePlayController::generateTemplate(int choice) {
//        /// Pre-made templates
//        switch (choice) {
//            case 1:
//                // _tilemap->clearMap();
//                printExecution("generatePrimaryWorld", [this](){
//                    generatePrimaryWorld();
//                });
//                _template = 1;
//                break;
//            case 2:
//                // _tilemap->clearMap();
//                printExecution("generateSecondaryWorld", [this](){
//                    generateSecondaryWorld();
//                });
//                _template = 2;
//                break;
//            default:
//                break;
//        }
//    }
    
    /**
     * Executes a function with debugging information.
     *
     * This function runs function `name` wrapped in `wrapper` and will call
     * CULog twice. The information from CLog will indicate
     *
     * - when the function starts
     * - how long it took to execute
     *
     * @param name      The name of the wrapped function
     * @param wrapper   The function wrapper to execute
     */
    void GamePlayController::printExecution(std::string name, std::function<void()> wrapper) {
        auto before = std::chrono::high_resolution_clock::now();
        CULog("Running %s", name.data());
        wrapper();
        auto after = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
        CULog("Generated after %lld milliseconds", duration);
    }


void GamePlayController::render(std::shared_ptr<SpriteBatch>& batch){
    _scene->render(batch);
}


