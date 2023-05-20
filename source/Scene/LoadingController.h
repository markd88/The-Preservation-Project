
#include <cugl/cugl.h>
// #include <common.h>

using namespace cugl;
using namespace std;

/**
 * This class is a simple loading screen for asychronous asset loading.
 *
 * The screen will display a very minimal progress bar that displays the
 * status of the asset manager.  Make sure that all asychronous load requests
 * are issued BEFORE calling update for the first time, or else this screen
 * will think that asset loading is complete.
 *
 * Once asset loading is completed, it will display a play button.  Clicking
 * this button will inform the application root to switch to the gameplay mode.
 */
class LoadingController{

public:
    std::shared_ptr<cugl::Scene2> _scene;

    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;

    // VIEW
    /** The animated progress bar */
    std::shared_ptr<cugl::scene2::ProgressBar>  _bar;
    /** The engine name */
    std::shared_ptr<cugl::scene2::SceneNode>  _brand;
    /** The "play" button */
    std::shared_ptr<cugl::scene2::Button>    _button;

    std::shared_ptr<cugl::scene2::SceneNode>  _loading_bg;

    std::shared_ptr<cugl::scene2::SceneNode>  _main_bg;
    // MODEL
    /** The progress displayed on the screen */
    float _progress;

public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new loading screen with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    LoadingController() {
        _progress = 0.0f;
    }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LoadingController() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();

    /**
     * Initializes the scene contents, making it ready for loading
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);


#pragma mark -
#pragma mark Progress Monitoring
    /**
     * The method called to update the scene.
     *
     * This method will update the progress bar.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);

    void render(std::shared_ptr<SpriteBatch> &batch) ;
    
    /**
     * Returns true if loading is complete, but the player has not pressed play
     *
     * @return true if loading is complete, but the player has not pressed play
     */
    bool isPending( ) const;

};

