#include <chrono>
#include "GamePlayController.h"
#include <chrono>
#include <thread>
// This is NOT in the same directory
using namespace std;
using namespace cugl;
#define PHYSICS_SCALE 50
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

GamePlayController::GamePlayController(const Size displaySize, std::shared_ptr<cugl::AssetManager>& assets ):_scene(cugl::Scene2::alloc(displaySize)) {
    // Initialize the assetManager
    _assets = assets;
    
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    _input->init(dimen);
    _cam = _scene->getCamera();
    // Allocate the manager and the actions
    _actions = cugl::scene2::ActionManager::alloc();
    
    // Allocate the camera manager
    _camManager = CameraManager::alloc();
    
    _scene->setSize(displaySize/1);
    
    _path = make_unique<PathController>();
    // initialize character, two maps, path
    
    _tilemap1 = std::make_unique<TilemapController>();
    _tilemap2 = std::make_unique<TilemapController>();
    generatePrimaryWorld(_tilemap1);
    generateSecondaryWorld(_tilemap2);

    generateArtifact(30, 400);
//    generateGuard(40, 490);
    
//    Vec2 gPos = Vec2(_scene->getSize().width * 0.02, _scene->getSize().height * 0.2);
//    std::unique_ptr<GuardController> _guard;
//    _guard = make_unique<GuardController>(gPos);
//    _guard->addChildTo(_scene);
    
    Vec2 start = Vec2(1,1);
    _character = make_unique<CharacterController>(start, _actions);
    
    // init the button
    _button_layer = _assets->get<scene2::SceneNode>("button");
    _button_layer->setContentSize(dimen);
    _button_layer->doLayout(); // This rearranges the children to fit the screen
    
    
    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("button_action"));
    _button->addListener([this](const std::string& name, bool down) {
        if (down) {
            this->init();
        }
    });
    
    _button->activate();
    
    init();
}


// init some assets when restart
void GamePlayController::init(){
    // remove everything first
    _scene->removeAllChildren();
    
    
    
    _tilemap1->addChildTo(_scene);
    _activeMap = "tileMap1";
    _template = 0;
    
    Vec2 start = Vec2(1,1);
    _character = make_unique<CharacterController>(start, _actions);
    _character->addChildTo(_scene);
    
    generateArtifact(30, 400);
    generateGuard(40, 490);
    
    _path = make_unique<PathController>();
    path_trace = {};
    
    _scene->addChild(_button_layer);
    
    Vec2 cPos = _character->getPosition();
    _cam->setPosition(Vec3(cPos.x,cPos.y,0));

    _cam->update();
    
    // to make the button pos fixed relative to screen
    _button_layer->setPosition(_cam->getPosition() - Vec2(900, 70));

}


void GamePlayController::update(float dt){
    static auto last_time = std::chrono::steady_clock::now();
    // Calculate the time elapsed since the last call to pinch
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_time);
    
    _input->update(dt);
    // if pinch, switch world
    bool can_switch = ((_activeMap == "tileMap1" && _tilemap2->inObstacle(_character->getPosition())) || (_activeMap == "tileMap2" && _tilemap1->inObstacle(_character->getPosition())));
    
    if(!can_switch){
        _character->updateColor(Color4::GREEN);
    }
    else{
        _character->updateColor(Color4::BLUE);
    }
    if(elapsed.count() >= 0.5 && _input->getPinchDelta() != 0 && !can_switch){
        // if the character's position on the other world is obstacle, disable the switch
        last_time = now;
        // remove and add the child back so that the child is always on the top layer
        _character->removeChildFrom(_scene);
        if (_activeMap == "tileMap1") {
            _tilemap1->removeChildFrom(_scene);
            _tilemap2->addChildTo(_scene);
            _activeMap = "tileMap2";
        }
        else {
            _tilemap2->removeChildFrom(_scene);
            _tilemap1->addChildTo(_scene);
            generateArtifact(30, 400);
            generateGuard(40, 490);
            _activeMap = "tileMap1";
        }
        _character->addChildTo(_scene);
        _scene->removeChild(_button_layer);
        _scene->addChild(_button_layer);
    }
    
    else if (!_input->getPanDelta().isZero()) {

        Vec2 delta = _input->getPanDelta();

        // init camera action
        _moveTo = cugl::scene2::MoveTo::alloc();
        _moveCam = CameraMoveTo::alloc();
        
        // pan move with the center of the camera view
        _moveTo->setTarget(_cam->getPosition() - delta);
        _moveCam->setTarget(_cam->getPosition() - delta);
        
        _camManager->activate("movingCam", _moveCam, _cam);
    }
    
    else if(_input->didPress()){        // if press, determine if press on character
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);
        
        if(_character->contains(input_posi)){
            // create path
            CULog("here");
            _path->setIsDrawing(true);
            _path->setIsInitiating(true);
            _path->updateLastPos(_character->getPosition()); //change to a fixed location on the character
        }
    }
    
    else if (_input->isDown() && _path->isDrawing){
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);
        // if input still within the character
        if(_path->isInitiating){
            // if input leaves out of the character's radius, draw the initial segments
            if (!_character->contains(input_posi)){
                _path->setIsInitiating(false);
            }
        }
        // an updated version to iteratively add segments until the input_posi is too close to last point in the model
        if(_path->isInitiating == false){
            while(_path->farEnough(input_posi)){
                Vec2 checkpoint = _path->getLastPos() + (input_posi - _path->getLastPos()) / _path->getLastPos().distance(input_posi) * _path->getSize();
                if((_activeMap == "tileMap1" && _tilemap1->inObstacle(checkpoint)) || (_activeMap == "tileMap2" && _tilemap2->inObstacle(checkpoint))){
                    _path->setIsDrawing(false);
                    path_trace.clear();
                    return;
                }
                else{
                    _path->addSegment(checkpoint, _scene);
                }
            }
        }
    }
    
    else if(_input->didRelease()){
        CULog("didRelease");
        Vec2 input_posi = _input->getPosition();
        input_posi = _scene->screenToWorldCoords(input_posi);
        _path->setIsDrawing(false);
        path_trace = _path->getPath();
        _moveTo = cugl::scene2::MoveTo::alloc();
        _moveCam = CameraMoveTo::alloc();
        _moveCam->setDuration(.1);
        _moveTo->setDuration(.1);
        _path->clearPath(_scene);
        
    }
    
    else if (path_trace.size() != 0 && _actions->isActive("moving") == false){
        _moveTo->setTarget(path_trace[0]);
        _moveCam->setTarget(path_trace[0]);
        Vec2 cPos = _character->getNodePosition();
        
        _character->moveTo(_moveTo);
        _camManager->activate("movingCam", _moveCam, _cam);
        path_trace.erase(path_trace.begin());

    }
    
    //Vec2 cPos = _character->getNodePosition();
    //_cam->setPosition(Vec3(cPos.x,cPos.y,0));
    //_cam->update();
    
    // Animate
    
    _actions->update(dt);
    _camManager->update(dt);
    
    // the camera is moving smoothly, but the UI only set its movement per frame
    _button_layer->setPosition(_cam->getPosition() - Vec2(900, 70));
}
    
    
#pragma mark Main Methods

    
#pragma mark -
#pragma mark Generation Helpers
    
    /** Generates the first world. */
    void GamePlayController::generatePrimaryWorld(std::unique_ptr<TilemapController> &_tilemap) {
        _tilemap->updateDimensions(Vec2(160, 88));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(8, 8));
        _tilemap->updatePosition(_scene->getSize()/2);
        // walls
        Color4 tileColor = Color4::BLACK;
        bool is_obs = true;
        for(int i = 0; i <= 35; i++) {
            _tilemap->addTile(i, 26, tileColor, is_obs);
        }
        for(int j = 0; j <= 13; j++) {
            _tilemap->addTile(22, j, tileColor, is_obs);
        }
        for(int j = 0; j <= 37; j++) {
            _tilemap->addTile(55, j, tileColor, is_obs);
        }
        for(int i = 0; i <= 35; i++) {
            _tilemap->addTile(i, 52, tileColor, is_obs);
        }
        for(int j = 53; j <= 73; j++) {
            _tilemap->addTile(35, j, tileColor, is_obs);
        }
        for(int i = 55; i <= 93; i++) {
            _tilemap->addTile(i, 37, tileColor, is_obs);
        }
        for(int j = 37; j <= 51; j++) {
            _tilemap->addTile(93, j, tileColor, is_obs);
        }
        for(int i = 93; i <= 113; i++) {
            _tilemap->addTile(i, 51, tileColor, is_obs);
        }
        for(int i = 93; i <= 126; i++) {
            _tilemap->addTile(i, 20, tileColor, is_obs);
        }
        for(int j = 0; j <= 20; j++) {
            _tilemap->addTile(126, j, tileColor, is_obs);
        }
        for(int j = 53; j <= 75; j++) {
            _tilemap->addTile(132, j, tileColor, is_obs);
        }
        for(int i = 132; i <= 142; i++) {
            _tilemap->addTile(i, 75, tileColor, is_obs);
        }
        for(int j = 75; j <= 88; j++) {
            _tilemap->addTile(142, j, tileColor, is_obs);
        }
    }
    
    /**
     * Generates the second world.
     *
     * @param p The probability that a tile is generated.
     */
    void GamePlayController::generateSecondaryWorld(std::unique_ptr<TilemapController> & _tilemap) {
        _tilemap->updateDimensions(Vec2(160, 88));
        _tilemap->updateColor(Color4::WHITE);
        _tilemap->updateTileSize(Size(8, 8));
        _tilemap->updatePosition(_scene->getSize()/2);
    }

    void GamePlayController::generateArtifact(float w, float h) {
        Vec2 aPos = Vec2(w,h);
        typedef std::unique_ptr<ArtifactController> Artifact;
        _artifact = make_unique<ArtifactController>(aPos);
        _artifact->updatePosition(aPos);
        _artifact->addChildTo(_scene);
//        _artifactSet->add_this(std::move(_artifact));
    }
    void GamePlayController::generateGuard(float w, float h) {
        Vec2 gPos = Vec2(w,h);
//        _guard = _guardSet->add_this(gPos);
//        typedef std::unique_ptr<GuardController> Guard;
        _guard = make_unique<GuardController>(Vec2(0,0));
        _guard->updatePosition(gPos);
        _guard->addChildTo(_scene);
    }

    
    // upper guard
    //        for (int i = 50; i <= 65; i++) {
    //            for(int j = 20; j <= 25; j++) {
    //                _tilemap->addTile(i, j, tileColor, is_obs);
    //            }
    //        }
    
    
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
    
