
#include <cugl/cugl.h>
using namespace cugl;
using namespace std;
// Uncomment to activate (but comment out MVC)
#include <Path/PathController.h>
#include <Character/CharacterController.h>
#include <Tilemap/TilemapController.h>
#include <Input/InputController.h>

class GamePlayController {
#pragma mark Internal References
private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    
#pragma mark External References
public:
    /** The tilemap to procedurally generate */
    std::unique_ptr<CharacterController> _character;
    // std::unique_ptr<TileMapController> _tilemap;
    // std::unique_ptr<PathController> _path;
    std::shared_ptr<InputController> _input = InputController::getInstance();
    vector<float> path_trace;
    
    /**
     * Creates the game controller.
     *
     * This constructor will procedurally generate a tilemap immediately
     * on creation.
     *
     * @param displaySize   The display size of the game window
     */
    GamePlayController(const Size displaySize);
    
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
    void render(std::shared_ptr<SpriteBatch> batch) {
        _scene->render(batch);
        
    }
    
    
};
