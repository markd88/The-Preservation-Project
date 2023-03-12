//
//  LoadingController.cpp
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#include "LoadingController.h"

using namespace cugl;
using namespace std;

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

/**
* Initializes the controller contents, making it ready for loading
*
* The constructor does not allocate any objects or memory.  This allows
* us to have a non-pointer reference to this controller, reducing our
* memory allocation.  Instead, allocation happens in this method.
*
* @param assets    The (loaded) assets for this game mode
*
* @return true if the controller is initialized properly, false otherwise.
*/
bool LoadingController::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    
    _scene = cugl::Scene2::alloc(dimen);
    
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/loading.json");
    auto layer = assets->get<scene2::SceneNode>("load");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    
    _bar = std::dynamic_pointer_cast<scene2::ProgressBar>(assets->get<scene2::SceneNode>("load_bar"));
    _brand = assets->get<scene2::SceneNode>("load_name");
    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("load_play"));
    _button->addListener([=](const std::string& name, bool down) {
        this->_scene->setActive(down);
    });

    Application::get()->setClearColor(Color4(192,192,192,255));
    _scene->addChild(layer);
    return true;
}

/**
* Disposes of all (non-static) resources allocated to this mode.
*/
void LoadingController::dispose() {
    // Deactivate the button (platform dependent)
    if (isPending()) {
        _button->deactivate();
    }
    _button = nullptr;
    _bar = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadingController::update(float timestep) {
    if (_progress < 1) {
        _progress = _assets->progress();
        if (_progress >= 1) {
            _progress = 1.0f;
            _bar->setVisible(false);
            _brand->setVisible(false);
            _button->setVisible(true);
            _button->activate();
        }
        _bar->setProgress(_progress);
    }
}

void LoadingController::render(std::shared_ptr<SpriteBatch>& batch){
    _scene->render(batch);
}


/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LoadingController::isPending( ) const {
    return _button != nullptr && _button->isVisible();
}



