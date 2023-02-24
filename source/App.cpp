//
//  TMApp.cpp
//  TileMap Lab
//
//  This is the header for the custom application. It is necessary so that
//  main.cpp can access your custom class. That is handled by a separate
//  subcontroller.
//
//  Authors: Walker White and Gonzalo Gonzalez
//  Version: 2/8/23
//
// Include the class header, which includes all of the CUGL classes
#include "TMApp.h"

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

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
void TileApp::onStartup() {
    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    setClearColor(Color4(229,229,229,255));
    
    // Activate the mouse and keyboard for interacting with the tilemap
    Input::activate<Mouse>();
    Input::activate<Keyboard>();
    
    // Seed the RNG to avoid psuedo-randomness
    unsigned seed = 42;
    _randoms = std::make_shared<std::mt19937>(seed);
    CULog("Seed = %d", seed);
    
    // Initialize GameController, passing it the random number generator
    Size size = getDisplaySize();
    size *= GAME_WIDTH/size.width;
    _gameController = std::make_unique<GameController>(size,_randoms);
    Application::onStartup();

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
void TileApp::onShutdown() {
    // Delete all smart pointers
    _batch = nullptr;
    
    // Deactivate input
    Input::deactivate<Mouse>();
    Input::deactivate<Keyboard>();
    
    Application::onShutdown();
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
void TileApp::update(float timestep) {
    _gameController->update(timestep);
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
void TileApp::draw() {
    _gameController->render(_batch);
}