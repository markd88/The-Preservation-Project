#include <chrono>
#include "GamePlayController.h"
#include <chrono>
#include <thread>
#include "Level/LevelConstants.h"
#include <common.h>

// This is NOT in the same directory
using namespace std;
using namespace cugl;
#define PHYSICS_SCALE 50
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024
#define ACTIONDURATION 0.08f
#define ANIMDURATION 1f
#define PREVIEW_RADIUS 150
#define SWITCH_DURATION 1

#define ACT_KEY  "current"

GamePlayController::GamePlayController(const Size displaySize, std::shared_ptr<cugl::AssetManager>& assets ):
_scene(cugl::Scene2::alloc(displaySize)), _other_scene(cugl::Scene2::alloc(displaySize)) {
    // Initialize the assetManage
    
    _ordered_root = cugl::scene2::OrderedNode::allocWithOrder(cugl::scene2::OrderedNode::Order::DESCEND);
    
    _other_ordered_root = cugl::scene2::OrderedNode::allocWithOrder(cugl::scene2::OrderedNode::Order::DESCEND);

    _assets = assets;
    
    // load sound
    _collectArtifactSound = assets->get<Sound>("arrowShoot");
    _collectResourceSound = assets->get<Sound>("NPC_flip");
    _switchSound = assets->get<Sound>("lovestruck");
    _loseSound = assets->get<Sound>("win");
    _winSound = assets->get<Sound>("lose");
    
    // load music
    _pastMusic = assets->get<Sound>("past");
    _presentMusic = assets->get<Sound>("present");
    
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    _input->init(dimen);
    
    _cam = _scene->getCamera();
    _other_cam = _other_scene->getCamera();
    
    // Allocate the manager and the actions
    _actions = cugl::scene2::ActionManager::alloc();
    _action_world_switch = cugl::scene2::ActionManager::alloc();
    
    // Allocate the camera manager
    _camManager = CameraManager::alloc();

    _scene->setSize(displaySize*1.5);
    _other_scene->setSize(displaySize*1.5);
    
    _previewNode = cugl::scene2::PolygonNode::alloc();
    _scene2texture = Scene2Texture::alloc(displaySize*5);
//    _scene->setSize(displaySize *3)
//    _other_scene->setSize(displaySize *3);
    
    _path = make_unique<PathController>();
    // initialize character, two maps, path
    
    // two-world switch animation initialization
    std::shared_ptr<Texture> world_switch  = assets->get<Texture>("two_world_switch");
    _world_switch_node = scene2::SpriteNode::allocWithSheet(world_switch, 5, 4, 20); // SpriteNode for two_world switch animation
    // _world_switch_node->setScale(0.8f); // Magic number to rescale asset
    _world_switch_node->setRelativeColor(false);
    _world_switch_node->setVisible(true);
    _world_switch_node->setFrame(19);

    std::vector<int> d0 = {0,1,2,3,4,5,6,7,8};
    _world_switch_0 = cugl::scene2::Animate::alloc(d0, SWITCH_DURATION);

    std::vector<int> d1 = {9,10,11,12,13,14,15,16,17,18};
    _world_switch_1 = cugl::scene2::Animate::alloc(d1, SWITCH_DURATION);

    // init the button
    _button_layer = _assets->get<scene2::SceneNode>("button");
    _button_layer->setContentSize(dimen);
    _button_layer->doLayout(); // This rearranges the children to fit the screen
    
    
    // add Win/lose panel
    _pause_layer = _assets->get<scene2::SceneNode>("pause");
    _pause_resume = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("pause_resume"));
    
    _pause_resume->addListener([this](const std::string& name, bool down) {
        if (!down) {
            // back to game
            auto s = _pause_layer->getScene();
            s->removeChild(_pause_layer);
        }
    });
    
    _pause_restart = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("pause_restart"));
    
    _pause_restart->addListener([this](const std::string& name, bool down) {
        if (!down) {
            // restart the game
            loadLevel();
            init();
        }
    });
    
    _pause_exit = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("pause_exit"));
    
    _pause_exit->addListener([this](const std::string& name, bool down) {
        if (!down) {
            // back to menu
            nextScene = MENU;
        }
    });
    
    // add pause button
    
    _pause_button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("button_pause-button"));
    _pause_button->addListener([this](const std::string& name, bool down) {
        if (!down) {
            // TODO:: activate the pause window
            pauseOn();
        }
    });
    
    _pause_button->activate();
    
    
    
    _inventory_layer = assets->get<scene2::SceneNode>("button_panel");

    
    // add Win/lose panel
    _complete_layer = _assets->get<scene2::SceneNode>("complete");
    _complete_next_button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("complete_next"));
    
    _complete_next_button->addListener([this](const std::string& name, bool down) {
        if (!down) {
            // go to next level
            nextLevel = true;
        }
    });
    
    _complete_back_button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("complete_back"));
    
    _complete_back_button->addListener([this](const std::string& name, bool down) {
        if (!down) {
            nextScene = MENU;
        }
    });

    // fail panel
    _fail_layer = _assets->get<scene2::SceneNode>("fail");
    _fail_again_button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("fail_again"));
    
    _fail_again_button->addListener([this](const std::string& name, bool down) {
        if (!down) {
            this->init();
        }
    });
    
    _fail_back_button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("fail_back"));
    
    _fail_back_button->addListener([this](const std::string& name, bool down) {
        if (!down) {
            nextScene = MENU;
        }
    });

    
    // add switch indicator
    _switchNode = _assets->get<scene2::SceneNode>("button_switch");
    
    
    _moveTo = cugl::scene2::MoveTo::alloc();
    _moveCam = CameraMoveTo::alloc();
    _moveCam->setDuration(ACTIONDURATION);
    _moveTo->setDuration(ACTIONDURATION);
    
    
    
    // initiazation of inventory bars, only run once
    Vec2 barPos(550, 300);
    for (int i=0; i<5; i++){
        std::shared_ptr<cugl::scene2::PolygonNode> tn = std::make_shared<cugl::scene2::PolygonNode>();
        tn->initWithTexture(_assets->get<Texture>("inventory_artifact_transparent_bar"));
        tn->setVisible(false);
        tn->setPosition(barPos);
        barPos = barPos.add(150, 0);
        _inventory_layer->addChild(tn);
        _art_bar_vec.push_back(tn);
    }

    barPos = barPos.add(400, 0);

    for (int i=0; i<5; i++){
        std::shared_ptr<cugl::scene2::PolygonNode> tn = std::make_shared<cugl::scene2::PolygonNode>();
        tn->initWithTexture(_assets->get<Texture>("inventory_resource_bar"));
        tn->setVisible(false);
        tn->setPosition(barPos);
        barPos = barPos.add(150, 0);
        _inventory_layer->addChild(tn);
        _res_bar_vec.push_back(tn);
    }
    
    
    
    loadLevel();
    init();
    
}

void GamePlayController::loadLevel(){
    string pastFile = "tileset/levels/level-" + std::to_string(level) + "/level-" + std::to_string(level) + "-past.json";
    string pastKey = "level-" + std::to_string(level) + "-past";
    string presentFile = "tileset/levels/level-" + std::to_string(level) + "/level-" + std::to_string(level) + "-present.json";
    string presentKey = "level-" + std::to_string(level) + "-present";
    
    _assets->unload<LevelModel>(pastKey);
    _assets->unload<LevelModel>(presentKey);
    _assets->load<LevelModel>(pastKey, pastFile);
    _assets->load<LevelModel>(presentKey, presentFile);
    
    
    // Draw past world
    _pastWorldLevel = _assets->get<LevelModel>(pastKey);
    if (_pastWorldLevel == nullptr) {
        CULog("Failed to import level!");
    }
    _pastWorldLevel->setAssets(_assets);
    _pastWorldLevel->setTilemapTexture();
    _pastWorld = _pastWorldLevel->getWorld();
    _obsSetPast = _pastWorldLevel->getObs();
    _wallSetPast = _pastWorldLevel->getWall();
    _artifactSet = _pastWorldLevel->getItem();
    _artifactSet->setAction(_actions);
    artNum = _artifactSet->getArtNum();

    // Draw present world
    _presentWorldLevel = _assets->get<LevelModel>(presentKey);
    if (_presentWorldLevel == nullptr) {
        CULog("Failed to import level!");
    }
    _presentWorldLevel->setAssets(_assets);
    _presentWorldLevel->setTilemapTexture();
    _presentWorld = _presentWorldLevel->getWorld();
    _obsSetPresent = _presentWorldLevel->getObs();
    _wallSetPresent = _presentWorldLevel->getWall();
    //_presentWorld->updateColor(Color4::CLEAR);
    //_pastWorld->updateColor(Color4::CLEAR);
    
    auto pastEdges = _pastWorld->getEdges(_scene, _obsSetPast);
    generatePastMat(_pastWorld->getVertices());
    for (int i = 0; i < pastEdges.size(); i++){
        addPastEdge(pastEdges[i].first, pastEdges[i].second);
    }
    
    auto presentEdges = _presentWorld->getEdges(_other_scene, _obsSetPresent);
    generatePresentMat(_presentWorld->getVertices());
    for (int i = 0; i < presentEdges.size(); i++){
        addPresentEdge(presentEdges[i].first, presentEdges[i].second);
    }
    
    _guardSetPast = std::make_unique<GuardSetController>(_assets, _actions, _pastWorld, _obsSetPast, pastMatrix, _pastWorld->getNodes());
    _guardSetPresent = std::make_unique<GuardSetController>(_assets, _actions, _presentWorld, _obsSetPresent, presentMatrix, _presentWorld->getNodes());
    
    // get guard positions
    _pastMovingGuardsPos = _pastWorldLevel->getMovingGuardsPos();
    _pastStaticGuardsPos = _pastWorldLevel->getStaticGuardsPos();
    _presentMovingGuardsPos = _presentWorldLevel->getMovingGuardsPos();
    _presentStaticGuardsPos = _presentWorldLevel->getStaticGuardsPos();

    // generate guards in past world
    generateMovingGuards(_pastMovingGuardsPos, true);
    generateStaticGuards(_pastStaticGuardsPos, true);
    
    // generate guards in present world
    generateMovingGuards(_presentMovingGuardsPos, false);
    generateStaticGuards(_presentStaticGuardsPos, false);

//    Vec2 start = Vec2(_scene->getSize().width *.85, _scene->getSize().height *.15);
    
//    Vec2 start = Vec2(0,0);
    Vec2 start = _pastWorldLevel->getCharacterPos();

    _character = make_unique<CharacterController>(start, _actions, _assets);

    // change label with level
    auto pause_label  = std::dynamic_pointer_cast<scene2::Label>(_assets->get<scene2::SceneNode>("pause_title"));
    pause_label->setText("Mission " + to_string(level));
    
}

// init assets and all scenegraph when restart
void GamePlayController::init(){
    
    // dispose all active actions
    _actions->dispose();
    // _action_world_switch->dispose();
    _camManager->dispose();
    
    // remove everything first
    _scene->removeAllChildren();
    _ordered_root->removeAllChildren();
    _scene2texture->removeAllChildren();
    _other_scene->removeAllChildren();
    _other_ordered_root->removeAllChildren();
    
    _pastWorld->addChildTo(_scene);
    _scene->addChild(_ordered_root);
    
    _presentWorld->addChildTo(_other_scene);
    _other_scene->addChild(_other_ordered_root);

    // for two world switch animation
    //_scene->addChild(_world_switch_node);
    _scene->addChild(_world_switch_node);
    _isSwitching = false;

    auto edges = _pastWorld->getEdges(_scene, _obsSetPast);
    generatePastMat(_pastWorld->getVertices());
    for (int i = 0; i < edges.size(); i++){
        addPastEdge(edges[i].first, edges[i].second);
    }
    
    _artifactSet->clearSet();
    _artifactSet = _pastWorldLevel->getItem();
    _artifactSet->addChildTo(_ordered_root);
    
    _obsSetPast->addChildTo(_ordered_root);
    _wallSetPast->addChildTo(_ordered_root);
    
    _obsSetPresent->addChildTo(_other_ordered_root);
    _wallSetPresent->addChildTo(_other_ordered_root);
    _obsSetPresent->setVisibility(true);
    auto presentEdges = _presentWorld->getEdges(_other_scene, _obsSetPresent);
    generatePresentMat(_presentWorld->getVertices());
    for (int i = 0; i < presentEdges.size(); i++){
        addPresentEdge(presentEdges[i].first, presentEdges[i].second);
    }
    
    _activeMap = "pastWorld";
    _pastWorld->setActive(true);
    _presentWorld->setActive(false);
    _template = 0;
    
    Vec2 start = _pastWorldLevel->getCharacterPos();

    _character = make_unique<CharacterController>(start, _actions, _assets);
    //_character->addChildTo(_scene);
    _character->addChildTo(_ordered_root);
    
    _guardSetPast = std::make_unique<GuardSetController>(_assets, _actions, _pastWorld, _obsSetPast, pastMatrix, _pastWorld->getNodes());
    _guardSetPresent = std::make_unique<GuardSetController>(_assets, _actions, _presentWorld, _obsSetPresent, presentMatrix, _presentWorld->getNodes());
    
    _guardSetPast->clearSet();
    _guardSetPresent->clearSet();
    
    // all guards are init in _guardSet
    // generate guards in past world
    generateMovingGuards(_pastMovingGuardsPos, true);
    generateStaticGuards(_pastStaticGuardsPos, true);
    
    // generate guards in present world
    generateMovingGuards(_presentMovingGuardsPos, false);
    generateStaticGuards(_presentStaticGuardsPos, false);


    _path = make_unique<PathController>();
    path_trace = {};
    
    _pause_button->activate();
    
    
    _scene->addChild(_button_layer);
    //_ordered_root->addChild(_button_layer);
    
    Vec2 cPos = _character->getPosition();
    _cam->setPosition(Vec3(cPos.x,cPos.y,0));
    _other_cam->setPosition(Vec3(cPos.x,cPos.y,0));
    
    _cam->update();
    _other_cam->update();
    
    // to make the button pos fixed relative to screen
    _button_layer->setPosition(_cam->getPosition());
    
    // play past sound
    AudioEngine::get()->clearEffects();
    AudioEngine::get()->play("past", _pastMusic, false, _switchSound->getVolume(), true);
}

void GamePlayController::update(float dt){

    if(_fail_layer->getScene() != nullptr || _complete_layer->getScene() != nullptr || _pause_layer->getScene() != nullptr){
        return;
    }


    //
    _world_switch_node->setPosition(_character->getNodePosition());
    if (_isSwitching && _action_world_switch->isActive("first_half")) {
        _action_world_switch->update(dt);
      //  CULog("update first half");
        return;
    }
    if (_isSwitching && !_action_world_switch->isActive("first_half") && !_action_world_switch->isActive("second_half")) {

        if (_activeMap == "pastWorld") {
            _activeMap = "presentWorld";
            _pastWorld->setActive(false);
            _presentWorld->setActive(true);
            
            _other_cam->setPosition(_cam->getPosition());
            _other_cam->update();
            _scene->removeChild(_button_layer);
            _other_scene->addChild(_button_layer);

            _character->removeChildFrom(_ordered_root);
            _character->addChildTo(_other_ordered_root);

            _scene->removeChild(_world_switch_node);
            _other_scene->addChild(_world_switch_node);
            
            AudioEngine::get()->clearEffects();
            AudioEngine::get()->play("present", _presentMusic, false, _switchSound->getVolume(), true);
            
        }
        else {
            _activeMap = "pastWorld";
            _pastWorld->setActive(true);
            _presentWorld->setActive(false);
            _cam->setPosition(_other_cam->getPosition());
            _cam->update();
            _other_scene->removeChild(_button_layer);
            _scene->addChild(_button_layer);

            _character->removeChildFrom(_other_ordered_root);
            _character->addChildTo(_ordered_root);

            _other_scene->removeChild(_world_switch_node);
            _scene->addChild((_world_switch_node));
            
            // when move to the second world, minus 1 in model
            _character->useRes();
            
            AudioEngine::get()->clearEffects();
            AudioEngine::get()->play("past", _pastMusic, false, _switchSound->getVolume(), true);
        }

        // stop previous movement after switch world
        _path->clearPath(_scene);
        _action_world_switch->activate("second_half", _world_switch_1, _world_switch_node);
        _isSwitching = false;
        CULog("switch second half");
        return;
    }
    if (_action_world_switch->isActive("second_half")) {
        _action_world_switch->update(dt);
        CULog("update second half");
        return;
    }



#pragma mark Switch World Methods
    static auto last_time = std::chrono::steady_clock::now();
    // Calculate the time elapsed since the last call to pinch
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_time);



    // codes to determine if buttons should be activated
    if(_fail_layer->getScene() == nullptr){
        _fail_back_button->deactivate();
        _fail_again_button->deactivate();
    }
    if(_complete_layer->getScene() == nullptr){
        _complete_back_button->deactivate();
        _complete_next_button->deactivate();
    }
    if(_pause_layer->getScene() == nullptr){
        _pause_exit->deactivate();
        _pause_resume->deactivate();
        _pause_restart->deactivate();
    }
    

    _input->update(dt);
    // if pinch, switch world
    bool cant_switch = ((_activeMap == "pastWorld" && _obsSetPresent->inObstacle(_character->getPosition())) || (_activeMap == "presentWorld" && _obsSetPast->inObstacle(_character->getPosition())));
    

    cant_switch = cant_switch || (_character->getNumRes() == 0);
    
    if(cant_switch){
        _switchNode->setColor(Color4::RED);
    }
    else{
        _switchNode->setColor(Color4::GREEN);
    }
    if(elapsed.count() >= 0.5 && _input->getPinchDelta() != 0 && !cant_switch){
        AudioEngine::get()->play("lovestruck", _switchSound, false, _switchSound->getVolume(), true);

        // if the character's position on the other world is obstacle, disable the switch
        last_time = now;
        // remove and add the child back so that the child is always on the top layer
        _isSwitching = true;
        _action_world_switch->activate("first_half", _world_switch_0, _world_switch_node);
        CULog("activate two world animation");


    }
    
#pragma mark Character Movement Methods
    else if(_input->didPress()){        // if press, determine if press on character
        
        Vec2 input_posi = _input->getPosition();

        if (_activeMap == "pastWorld"){
            input_posi = _scene->screenToWorldCoords(input_posi);
        }else{
            input_posi = _other_scene->screenToWorldCoords(input_posi);
        }
        auto r = _pastWorld->getNode()->getSize();

        if(_character->contains(input_posi)){
            // create path
            _path->setIsDrawing(true);
            _path->setIsInitiating(true);
//            _path->updateLastPos(_character->getPosition()); //change to a fixed location on the character
//            if (_activeMap == "pastWorld"){
//                _path->clearPath(_scene);
//            }else{
//                _path->clearPath(_other_scene);
//            }
            
        }

        else if (input_posi.x - PREVIEW_RADIUS > 0 and input_posi.x < r.width - PREVIEW_RADIUS and
                 input_posi.y > 0 and input_posi.y < r.height - PREVIEW_RADIUS*2 and !_isSwitching){
            //initialize preview
            _isPreviewing = true;
            if (_activeMap == "pastWorld"){
                auto _children = _other_scene->getChildren();
                for (int i = 0; i < _children.size(); i++){
                    auto tempChild = _children[i];
                    _other_scene->removeChild(_children[i]);
                    _scene2texture->addChild(tempChild);
                }
                _texture = _scene2texture->getTexture();
                _previewNode->setTexture(_texture);
                _previewNode->setVisible(false);
                _scene->addChildWithName(_previewNode, "preview");
            }
            else{
                auto _children = _scene->getChildren();
                for (int i = 0; i < _children.size(); i++){
                    auto tempChild = _children[i];
                    _scene->removeChild(_children[i]);
                    _scene2texture->addChild(tempChild);
                }
                _texture = _scene2texture->getTexture();
                _previewNode->setTexture(_texture);
                _previewNode->setVisible(false);
                _other_scene->addChildWithName(_previewNode, "preview");
            }
        }
    }
    
    else if (_input->isDown() && _path->isDrawing){
        
        Vec2 input_posi = _input->getPosition();
        if (_activeMap == "pastWorld"){
            input_posi = _scene->screenToWorldCoords(input_posi);
        }else{
            input_posi = _other_scene->screenToWorldCoords(input_posi);
        }
        // if input still within the character
        if(_path->isInitiating){
            // if input leaves out of the character's radius, draw the initial segments
            if (!_character->contains(input_posi)){
                
                _path->setIsInitiating(false);
                _path->updateLastPos(_character->getPosition()); //change to a fixed location on the character
                if (_activeMap == "pastWorld"){
                    _path->clearPath(_scene);
                }else{
                    _path->clearPath(_other_scene);
                }
            }
        }
        
        if(_path->isInitiating == false){
            while(_path->farEnough(input_posi)){
                Vec2 checkpoint = _path->getLastPos() + (input_posi - _path->getLastPos()) / _path->getLastPos().distance(input_posi) * _path->getSize();

                // TODO:: Need to add the logic so that the path won't go outside the map
                // get map's info
                Vec2 worldSize = _pastWorld->getSize();
                bool withinMap = (checkpoint.x >= 0) && (checkpoint.x <= worldSize.x) && (checkpoint.y >= 0) && (checkpoint.y <= worldSize.y);
                
                if((_activeMap == "pastWorld" && _obsSetPast->inObstacle(checkpoint)) || (_activeMap == "presentWorld" && _obsSetPresent->inObstacle(checkpoint))){
                    _path->setIsDrawing(false);
                    break;
                }
                else if(!withinMap){
                    _path->setIsDrawing(false);
                    break;
                }
                else{
                    if (_activeMap == "pastWorld"){
                        _path->addSegment(checkpoint, _scene);
                    }else{
                        _path->addSegment(checkpoint, _other_scene);
                    }
                }
            }
        }
    }


#pragma mark Preview Methods
    if(_input->didRelease() or _isSwitching){

        _isPreviewing = false;
       
        _path->setIsDrawing(false);
        // path_trace = _path->getPath();

        if(_isSwitching){
            if (_activeMap == "pastWorld"){
                _path->removeFrom(_scene);
            }else{
                _path->removeFrom(_other_scene);
            }
        }
        
        //finish previewing
        if (_activeMap == "pastWorld"){
            auto _children = _scene2texture->getChildren();
            for (int i = 0; i < _children.size(); i++){
                auto tempChild = _children[i];
                _scene2texture->removeChild(_children[i]);
                _other_scene->addChild(tempChild);
            }
            _scene->removeChildByName("preview");
        }
        else{
            auto _children = _scene2texture->getChildren();
            for (int i = 0; i < _children.size(); i++){
                auto tempChild = _children[i];
                _scene2texture->removeChild(_children[i]);
                _scene->addChild(tempChild);
            }
            _other_scene->removeChildByName("preview");
        }
    }
    
    else if (_isPreviewing){
        
        Vec2 input_posi = _input->getPosition();
        _previewNode->setVisible(true);
        if (_activeMap == "pastWorld"){
            input_posi = _scene->screenToWorldCoords(input_posi);
        }
        else {
            input_posi = _other_scene->screenToWorldCoords(input_posi);
        }
        
        auto r = _pastWorld->getNode()->getSize();
        
        if (input_posi.x - PREVIEW_RADIUS < 0 or input_posi.x > r.width - PREVIEW_RADIUS or
            input_posi.y < 0 or input_posi.y > r.height - PREVIEW_RADIUS*2){
            //input position is not in valid position
        }
        else {
            _previewNode->setAnchor(Vec2::ANCHOR_CENTER);
            PolyFactory polyFact = PolyFactory();
            Poly2 circle = polyFact.makeCircle(input_posi + Vec2(0, PREVIEW_RADIUS), PREVIEW_RADIUS);
            _previewNode->setPolygon(circle);
            _previewNode->setPosition(input_posi + Vec2(0,PREVIEW_RADIUS));
        }

    }
    
    
    
#pragma mark Path Methods
    
    if (_path->getPath().size() != 0 && !_actions->isActive("moving") ){
        _moveTo->setTarget(_path->getPath()[0]);
        _moveCam->setTarget(_path->getPath()[0]);
        _character->moveTo(_moveTo);
        _character->updateLastDirection(_path->getPath()[0]);

        if (_activeMap == "pastWorld"){
            _camManager->activate("movingCam", _moveCam, _cam);
            _path->removeFirst(_scene);
        }else{
            _camManager->activate("movingOtherCam", _moveCam, _other_cam);
            _path->removeFirst(_other_scene);
        }
        
    }

    if (!_actions->isActive("moving") && _actions->isActive("character_animation")) {
        _character->stopAnimation();
    }


#pragma mark Resource Collection Methods
    // if collect a resource
    if(_activeMap == "pastWorld"){
        for(int i=0; i<_artifactSet->_itemSet.size(); i++){
            // detect collision
            if(_character->contains(_artifactSet->_itemSet[i]->getNodePosition())){
                // if close, should collect it
                // if resource
                if(_artifactSet->_itemSet[i]->isResource()){
                    AudioEngine::get()->play("NPC_flip", _collectResourceSound, false, _collectResourceSound->getVolume(), true);
                    _character->addRes();
                   
                }
                // if artifact
                else if (_artifactSet->_itemSet[i]->isArtifact()){
                    AudioEngine::get()->play("arrowHit", _collectArtifactSound, false, _collectArtifactSound->getVolume(), true);
                    _character->addArt();

                }
                // make the artifact disappear and remove from set
                _artifactSet->remove_this(i, _ordered_root);
                if(_character->getNumArt() == artNum){
                    completeTerminate();
                }
                break;
            }
            
        }
        
    }

#pragma mark Guard Methods
    _guardSetPast->patrol(_character->getNodePosition(), _character->getAngle());
    _guardSetPresent->patrol(_character->getNodePosition(), _character->getAngle());
    // if collide with guard
    if(_activeMap == "pastWorld"){
        for(int i=0; i<_guardSetPast->_guardSet.size(); i++){
            if(_character->contains(_guardSetPast->_guardSet[i]->getNodePosition())){
                failTerminate();
                break;
            }
//            if(_obsSetPast->inObstacle(_guardSetPast->_guardSet[i]->getNodePosition())){
//                // guard stop
//            }
        }
    }
    
    else{
        for(int i=0; i<_guardSetPresent->_guardSet.size(); i++){
            if(_character->contains(_guardSetPresent->_guardSet[i]->getNodePosition())){
                failTerminate();
                break;
            }
        }
    }
    
    // Animate
    _actions->update(dt);
    _camManager->update(dt);
    
    // the camera is moving smoothly, but the UI only set its movement per frame
    if (_activeMap == "pastWorld"){
        _button_layer->setPosition(_cam->getPosition() - Vec2(900, 70));
    }else{
        _button_layer->setPosition(_other_cam->getPosition() - Vec2(900, 70));
    }
    
    
    // update render priority
    updateRenderPriority();
    
    // update inventory panel
    updateInventoryPanel();
}
    
    
#pragma mark Main Methods

    
#pragma mark -
#pragma mark Generation Helpers

    void GamePlayController::generateMovingGuards(std::vector<std::vector<cugl::Vec2>> movingGuardsPos, bool isPast) {
        
        for (int i = 0; i < movingGuardsPos.size(); i++) {
            int startX = movingGuardsPos[i][0].x;
            int startY = movingGuardsPos[i][0].y;
            std::vector<cugl::Vec2> patrolPoints = movingGuardsPos[i];
            addMovingGuard(startX, startY, patrolPoints, isPast);
        }
    }

    void GamePlayController::generateStaticGuards(std::vector<std::vector<int>> staticGuardsPos, bool isPast) {
        for (int i = 0; i < staticGuardsPos.size(); i++) {
            int x = staticGuardsPos[i][0];
            int y = staticGuardsPos[i][1];
            int dir = staticGuardsPos[i][2];
            addStaticGuard(x, y, dir, isPast);
        }
    }

    
#pragma mark -
#pragma mark Helpers

    
    void GamePlayController::render(std::shared_ptr<SpriteBatch>& batch){

        if (_activeMap == "pastWorld"){
            _scene->render(batch);
        }
        
        else{
            _other_scene->render(batch);
        }
        
        if (_isPreviewing){
            //_scene2texture->getCamera()->setPosition(_cam->getPosition());
            _scene2texture->render(batch);
        }
        

    }
    
