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
//#include <GuardSet/Guard/GuardController.h>
#include <ArtifactSet/ArtifactSetController.h>
//#include <ArtifactSet/Artifact/ArtifactController.h>


class GamePlayController {
#pragma mark Internal References
private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;

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
    
    /** The tilemap to procedurally generate */
    std::unique_ptr<CharacterController> _character;
    std::unique_ptr<TilemapController> _tilemap1;
    std::unique_ptr<TilemapController> _tilemap2;
    
//    std::unique_ptr<GuardController> _guard;
    std::unique_ptr<GuardSetController> _guardSet1;
    std::unique_ptr<GuardSetController> _guardSet2;
    
//    std::unique_ptr<ArtifactController> _artifact;
    std::unique_ptr<ArtifactSetController> _artifactSet;
    std::unique_ptr<ArtifactSetController> _resourceSet;
    
    std::unique_ptr<PathController> _path;
    std::shared_ptr<InputController> _input = InputController::getInstance();
    vector<Vec2> path_trace;
    std::shared_ptr<Camera> _cam;
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;
    std::shared_ptr<cugl::scene2::MoveTo> _moveTo;
    std::shared_ptr<cugl::scene2::Animate> _characterRight;
    std::shared_ptr<cugl::scene2::Animate> _characterLeft;
    
    /**manager to process camera actions**/
    std::shared_ptr<CameraManager> _camManager;
    std::shared_ptr<CameraMoveTo> _moveCam;
    
    string _activeMap;

//    std::shared_ptr<cugl::scene2::Label> _label;
    
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

#pragma mark Generation Helpers
private:
    /** Generates primary world with guards. */
    void generatePrimaryWorld(std::unique_ptr<TilemapController>& tilemap);

    /** Generates secondary world without guards. */
    void generateSecondaryWorld(std::unique_ptr<TilemapController>& tilemap);
    
    /** Generates artifacts and guards in the primary world. */
    void addArtifact(int w, int h, bool isResource) {
        Vec2 aPos = Vec2(w,h);
        _artifactSet->add_this(aPos, _scene, _assets, isResource);
    }
    void addGuard1(int w, int h) {
        Vec2 gPos = Vec2(w,h);
        _guardSet1->add_this(gPos, _scene, _assets);
    }
    void addGuard2(int w, int h) {
        Vec2 gPos = Vec2(w,h);
        _guardSet2->add_this(gPos, _scene, _assets);
    }
    void generateArtifact();
    void generateResource();
    void generateGuard();
    void secondaryGuard();



};

#endif /* __GAME_CONTROLLER_H__ */
