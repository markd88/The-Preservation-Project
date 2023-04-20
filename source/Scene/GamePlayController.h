#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__
#include <random>
#include <climits>

#include <cugl/cugl.h>
using namespace cugl;
using namespace std;
#include <Path/PathController.h>
#include <Character/CharacterController.h>
#include <Tilemap/TilemapController.h>
#include <Input/InputController.h>
#include <Camera/CameraManager.h>
#include <Camera/CameraMove.h>
#include <GuardSet/GuardSetController.h>
#include <ItemSet/ItemSetController.h>
#include "LevelModel.h"

class GamePlayController {
#pragma mark Internal References
private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    
    std::shared_ptr<cugl::Scene2> _other_scene;
    
    std::shared_ptr<cugl::scene2::OrderedNode> _ordered_root;
    
    std::shared_ptr<cugl::scene2::OrderedNode> _other_ordered_root;
    

    /** The current tile map template (for regeneration) */
    int _template;
    
#pragma mark External References
public:
    
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<cugl::scene2::Button> _button;
    std::shared_ptr<cugl::scene2::Label> _res_label;
    std::shared_ptr<cugl::scene2::Label> _art_label;
    
    Vec2 _button_screen_pos;
    std::shared_ptr<cugl::scene2::SceneNode> _button_layer;
    std::shared_ptr<cugl::scene2::SceneNode> _complete_layer;
    std::shared_ptr<cugl::scene2::SceneNode> _fail_layer;
    std::shared_ptr<cugl::scene2::SceneNode> _switchNode;
    
    // all buttons
    std::shared_ptr<cugl::scene2::Button> _reset_button;
    std::shared_ptr<cugl::scene2::Button> _complete_back_button;
    std::shared_ptr<cugl::scene2::Button> _complete_again_button;
    std::shared_ptr<cugl::scene2::Button> _fail_back_button;
    std::shared_ptr<cugl::scene2::Button> _fail_again_button;
    
    
    
    /** The tilemap to procedurally generate */
    std::unique_ptr<CharacterController> _character;
    std::shared_ptr<TilemapController> _pastWorld;
    std::shared_ptr<TilemapController> _presentWorld;
    
    std::unique_ptr<GuardSetController> _guardSetPast;
    std::unique_ptr<GuardSetController> _guardSetPresent;
    
    std::shared_ptr<ItemSetController> _artifactSet;
    std::shared_ptr<ItemSetController> _resourceSet;
    std::shared_ptr<ItemSetController> _obsSetPast;
    std::shared_ptr<ItemSetController> _obsSetPresent;
    std::shared_ptr<ItemSetController> _wallSetPast;
    std::shared_ptr<ItemSetController> _wallSetPresent;
    
    std::unique_ptr<PathController> _path;
    std::shared_ptr<InputController> _input = InputController::getInstance();
    vector<Vec2> path_trace;
    std::shared_ptr<Camera> _cam;
    std::shared_ptr<Camera> _other_cam;
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;
    std::shared_ptr<cugl::scene2::MoveTo> _moveTo;

    /**adjacency matrix*/
    bool** pastMatrix;
    bool** presentMatrix;
    
    /**manager to process camera actions**/
    std::shared_ptr<CameraManager> _camManager;
    std::shared_ptr<CameraMoveTo> _moveCam;
    
    string _activeMap;

//    std::shared_ptr<cugl::scene2::Label> _label;
    std::shared_ptr<LevelModel> _pastWorldLevel;
    std::shared_ptr<LevelModel> _presentWorldLevel;
    
    //whether or not user is previewing
    bool _isPreviewing;

    // two_world switch

    // if two-world switch is in progress
    bool _isSwitching;
    // first half: collapse
    std::shared_ptr<cugl::scene2::Animate> _world_switch_0;
    // second half: expand
    std::shared_ptr<cugl::scene2::Animate> _world_switch_1;

    std::shared_ptr<cugl::scene2::SpriteNode>  _world_switch_node;

    std::shared_ptr<cugl::scene2::ActionManager> _action_world_switch;


    // guards
    std::vector<std::vector<cugl::Vec2>> _pastMovingGuardsPos;
    std::vector<std::vector<int>> _pastStaticGuardsPos;

    std::vector<std::vector<cugl::Vec2>> _presentMovingGuardsPos;
    std::vector<std::vector<int>> _presentStaticGuardsPos;

    // sounds
    std::shared_ptr<cugl::Sound> _collectArtifactSound;
    std::shared_ptr<cugl::Sound> _collectResourceSound;
    std::shared_ptr<cugl::Sound> _switchSound;
    std::shared_ptr<cugl::Sound> _loseSound;
    std::shared_ptr<cugl::Sound> _winSound;


#pragma mark Main Methods
public:
    /**
     * Creates the game controller.
     *
     * This constructor will init all fields in this class and activate necessary components. the attachment of scenenodes to the scene happen in init method.
     *
     * @param displaySize   The display size of the game window
     * @param randoms        Reference to the random number generator
     */
    // GamePlayController(const Size displaySize, const std::shared_ptr<std::mt19937>& randoms);
    GamePlayController(const Size displaySize, /** The asset manager for this game mode. */
                       std::shared_ptr<cugl::AssetManager>& assets);
    
    /**
     * Init the GameplayScene when start, mostly do scenegraph arrangement
     */
    void init();
    
    /**
     * Responds to the keyboard commands.
     *
     * This method allows us to regenerate the procedurally generated tilemap
     * upon command.
     *
     * @param dt  The amount of time (in seconds) since the last frame
     */
    void update(float dt);
    
    /**
     * Renders the game elements using the`batch.
     *
     * @param batch The SpriteBatch used to render this scene
     */

    void render(std::shared_ptr<SpriteBatch> &batch) ;
    
    void dispose();
    
    void setActive(bool active){
        // only handle when active is false
        // otherwise go init
        _reset_button->deactivate();
        _fail_back_button->deactivate();
        _fail_again_button->deactivate();
        _complete_back_button->deactivate();
        _complete_again_button->deactivate();
    }

#pragma mark Generation Helpers
//private:
    /** Generates items and guards in the primary world. */
//    void addItem(int w, int h, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
//        Vec2 aPos = Vec2(w,h);
//        _ItemSet->add_this(aPos, isResource, assets, textureKey);
//    }
    void addMovingGuard(int w, int h, vector<Vec2> patrol_stops, bool isPast) {
        Vec2 gPos = Vec2(w,h);
        if (isPast) {
            _guardSetPast->add_this_moving(gPos, _ordered_root, _assets, patrol_stops, true);
        } else {
            _guardSetPresent->add_this_moving(gPos,_other_ordered_root, _assets, patrol_stops, false);
        }
    }
    
    void addStaticGuard(int w, int h, int dir, bool isPast) {
        Vec2 gPos = Vec2(w,h);
        if (isPast) {
            _guardSetPast->add_this(gPos, _ordered_root, _assets, true, dir);
        } else {
            _guardSetPresent->add_this(gPos, _other_ordered_root, _assets, false, dir);
        }
    }
    
    void generateArtifact();
    
    void generateResource();
    
    void generatePastMat(int vertices){
        pastMatrix = new bool*[vertices];
        
        for (int i = 0; i < vertices; i++) {
          pastMatrix[i] = new bool[vertices];
          for (int j = 0; j < vertices; j++)
            pastMatrix[i][j] = false;
        }
    }
    
    void generatePresentMat(int vertices){
        presentMatrix = new bool*[vertices];
        
        for (int i = 0; i < vertices; i++) {
          presentMatrix[i] = new bool[vertices];
          for (int j = 0; j < vertices; j++)
            presentMatrix[i][j] = false;
        }
    }
    
    void addPastEdge(int i, int j) {
        pastMatrix[i][j] = true;
        pastMatrix[j][i] = true;
    }
    
    void addPresentEdge(int i, int j) {
        presentMatrix[i][j] = true;
        presentMatrix[j][i] = true;
    }

    void failTerminate(){
        AudioEngine::get()->play("win", _winSound, false, _winSound->getVolume(), true);
        _scene->addChild(_fail_layer);
        _fail_layer->setPosition(_cam->getPosition());
        _fail_back_button->activate();
        _fail_again_button->activate();
    }
    
    void completeTerminate(){
        AudioEngine::get()->play("lost", _loseSound, false, _loseSound->getVolume(), true);
        _scene->addChild(_complete_layer);
        _complete_layer->setPosition(_cam->getPosition());
        _complete_back_button->activate();
        _complete_again_button->activate();
    }
    
    // called when scene becomes active or inactive
    void generateMovingGuards(std::vector<std::vector<cugl::Vec2>> movingGuardsPos, bool isPast);
    void generateStaticGuards(std::vector<std::vector<int>> staticGuardsPos, bool isPast);

    
    void updateRenderPriority(){
        // both orderedRoot
        //_pastWorld->setPriority(1000);
        _artifactSet->updatePriority();
        _obsSetPast->updatePriority();
        _wallSetPast->updatePriority();
        _character->updatePriority();
        _guardSetPast->updatePriority();
        
        
        //_presentWorld->setPriority(1000);
        _obsSetPresent->updatePriority();
        _wallSetPresent->updatePriority();
        _guardSetPresent->updatePriority();
    }

};

#endif /* __GAME_CONTROLLER_H__ */
