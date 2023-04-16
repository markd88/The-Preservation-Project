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
#include <ArtifactSet/ArtifactSetController.h>
#include "LevelModel.h"

class GamePlayController {
#pragma mark Internal References
private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    
    std::shared_ptr<cugl::Scene2> _other_scene;

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
    
    std::shared_ptr<ArtifactSetController> _artifactSet;
    std::shared_ptr<ArtifactSetController> _resourceSet;
    
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
    /** Generates artifacts and guards in the primary world. */
//    void addArtifact(int w, int h, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
//        Vec2 aPos = Vec2(w,h);
//        _artifactSet->add_this(aPos, isResource, assets, textureKey);
//    }
    void addMovingGuard1(int w, int h, vector<Vec2> patrol_stops) {
        Vec2 gPos = Vec2(w,h);
        _guardSetPast->add_this_moving(gPos, _scene, _assets, patrol_stops );
    }
    
    void addGuard1(int w, int h) {
        Vec2 gPos = Vec2(w,h);
        _guardSetPast->add_this(gPos, _scene, _assets);
    }
    
    void addMovingGuard2(int w, int h, vector<Vec2> patrol_stops) {
        Vec2 gPos = Vec2(w,h);
        _guardSetPresent->add_this_moving(gPos, _other_scene, _assets, patrol_stops);
    }

    void addGuard2(int w, int h) {
        Vec2 gPos = Vec2(w,h);
        _guardSetPresent->add_this(gPos, _other_scene, _assets);
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
        _scene->addChild(_fail_layer);
        _fail_layer->setPosition(_cam->getPosition());
        _fail_back_button->activate();
        _fail_again_button->activate();
    }
    
    void completeTerminate(){
        _scene->addChild(_complete_layer);
        _complete_layer->setPosition(_cam->getPosition());
        _complete_back_button->activate();
        _complete_again_button->activate();
    }
    
    // called when scene becomes active or inactive
    void generatePastGuards();
    void generatePresentGuards();


};

#endif /* __GAME_CONTROLLER_H__ */
