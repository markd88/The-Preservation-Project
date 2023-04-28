//
//  TPPApp.cpp
//  TPP
//
//  This is the header for the custom application. It is necessary so that
//  main.cpp can access your custom class. That is handled by a separate
//  subcontroller.
//
//  Authors: Walker White and Gonzalo Gonzalez
//  Version: 2/8/23
//
// Include the class header, which includes all of the CUGL classes
#include "App.h"
#include <Level/LevelConstants.h>
#include <Level/LevelModel.h>
#include <common.h>

// This keeps us from having to write cugl:: all the time
using namespace cugl;
using namespace std;
// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

ActiveScene curScene;
ActiveScene nextScene;
int level;
bool nextLevel;

/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
void App::onStartup() {
    // Init the asset manager
    _assets = AssetManager::alloc();
    
    // Start-up input
    Input::activate<Touchscreen>();
    Input::activate<CoreGesture>();
    
    // init the assetManager
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<WidgetValue>(WidgetLoader::alloc()->getHook());
    _assets->attach<scene2::SceneNode>(Scene2Loader::alloc()->getHook());
    
    _assets->attach<LevelModel>(GenericLoader<LevelModel>::alloc()->getHook());

    // load gameplay assets
    _assets->loadDirectoryAsync("json/assets.json", nullptr);

    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    auto cam = OrthographicCamera::alloc(getDisplaySize());
    setClearColor(Color4(229,229,229,255));
    
    // Activate the touchscreen for interacting with the tilemap

    Input::activate<Touchscreen>();
    
    // active_scene = "GamePlay";
    
    // Initialize GameController, passing it the random number generator
    Size size = getDisplaySize();
    size *= GAME_WIDTH/size.width;
    
    
    _loadingController = make_shared<LoadingController>();
    
    // _loaded = false;

    
    curScene = LOADING;
    nextScene = LOADING;
    
    _loadingController->init(_assets);
    
    AudioEngine::start();

    // parent call
    Application::onStartup();
    
    nextLevel = false;
}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void App::onShutdown() {
    // Delete all smart pointers
    _batch = nullptr;
    
    // Deactivate input
    Input::deactivate<Touchscreen>();
    
    AudioEngine::stop();
    Application::onShutdown();
}

/**
 * The method called when the application is suspended and put in the background.
 *
 * When this method is called, you should store any state that you do not
 * want to be lost.  There is no guarantee that an application will return
 * from the background; it may be terminated instead.
 *
 * If you are using audio, it is critical that you pause it on suspension.
 * Otherwise, the audio thread may persist while the application is in
 * the background.
 */

void App::onSuspend() {
    AudioEngine::get()->pause();
}

/**
 * The method called when the application resumes and put in the foreground.
 *
 * If you saved any state before going into the background, now is the time
 * to restore it. This guarantees that the application looks the same as
 * when it was suspended.
 *
 * If you are using audio, you should use this method to resume any audio
 * paused before app suspension.
 */
void App::onResume() {
    AudioEngine::get()->resume();
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void App::update(float timestep) {
//    if (!_loaded && _loadingController->_scene->isActive()) {
//        _loadingController->update(timestep);
//    } else if (!_loaded) {
//        _loadingController->dispose(); // Disables the input listeners in this mode
//        _loaded = true;
//        Size size = getDisplaySize();
//        size *= GAME_WIDTH/size.width;
//        // _gameplayController = make_shared<GamePlayController>(size, _assets);
//        _menuController = make_shared<MenuController>();
//        _menuController->init(_assets);
//
//    } else {
//
//        //_gameplayController->update(timestep);
//        _menuController->update(timestep);
//    }
    if(curScene == nextScene){
        switch(curScene)
        {
            case LOADING:
                _loadingController->update(timestep);
                break;
            case MENU:
                
                _menuController->update(timestep);
                break;
            case GAMEPLAY:
                // if next level, deactivate first, reload level i+1, and init
                if(nextLevel){
                    level += 1;
                    _gameplayController->setActive(false);
                    _gameplayController->loadLevel();
                    _gameplayController->init();
                    nextLevel = false;
                }
                else{
                    _gameplayController->update(timestep);
                }
        }
    }
    else{

        switch(curScene)
        {
            case LOADING:
                _loadingController->dispose();
                break;
            case MENU:
                _menuController->setActive(false);
                break;
            case GAMEPLAY:
                _gameplayController->setActive(false);
                break;
        }

        switch(nextScene)
        {
            case LOADING:
                break;
            case MENU:
                // if init before, just use previous menu
                if(_menuController == nullptr) {
                    _menuController = make_shared<MenuController>();
                    _menuController->init(_assets);
                }
                else{
                    _menuController->setActive(true);
                }
                curScene = MENU;
                break;
            case GAMEPLAY:
                if(_gameplayController == nullptr){
                    Size size = getDisplaySize();
                    size *= GAME_WIDTH/size.width;
                    _gameplayController = make_shared<GamePlayController>(size, _assets);
                }
                else{
                    _gameplayController->loadLevel();
                    _gameplayController->init();
                }

                curScene = GAMEPLAY;
                break;
        }

    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void App::draw() {
//    if (!_loaded) {
//        _loadingController->render(_batch);
//    } else {
//        _menuController->render(_batch);
//        //_gameplayController->render(_batch);
//    }
    switch(curScene)
    {
        case LOADING:
            _loadingController->render(_batch);
            break;
        case MENU:
            _menuController->render(_batch);
            break;
        case GAMEPLAY:
            _gameplayController->render(_batch);
    }
}
