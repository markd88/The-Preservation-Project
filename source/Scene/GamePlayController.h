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
#include <GuardSet/Guard/GuardController.h>


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
    Vec2 _button_screen_pos;
    std::shared_ptr<cugl::scene2::SceneNode> _button_layer;
    
    /** The tilemap to procedurally generate */
    std::unique_ptr<CharacterController> _character;
    std::unique_ptr<TilemapController> _tilemap1;
    std::unique_ptr<TilemapController> _tilemap2;
    std::unique_ptr<GuardController> _guard;
    
    std::unique_ptr<PathController> _path;
    std::shared_ptr<InputController> _input = InputController::getInstance();
    vector<Vec2> path_trace;
    std::shared_ptr<Camera> _cam;
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;
    std::shared_ptr<cugl::scene2::MoveTo> _moveTo;
    
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
    
    /** Generates guards in the primary world. */
    void generateGuard(std::unique_ptr<GuardController> &_guard);
    

#pragma mark Helpers
private:

    /**
     * Creates a new tile map with the given template number
     *
     * @param choice    The template number
     */
    void generateTemplate(int choice);
    
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
    void printExecution(std::string name, std::function<void()> wrapper);
};

#endif /* __GAME_CONTROLLER_H__ */
