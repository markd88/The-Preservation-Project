//
//  PCApp.h
//  TileMap Lab
//
//  This is the header for the custom application. It is necessary so that
//  main.cpp can access your custom class. That is handled by a separate
//  subcontroller.
//
//  Authors: Walker White and Gonzalo Gonzalez
//  Version: 2/8/23
//
using namespace std;

#ifndef __PC_APP_H__
#define __PC_APP_H__
#include <cugl/cugl.h>

// This is NOT in the same directory
#include <Scene/GamePlayController.h>
#include <Scene/LoadingController.h>

/**
 * Class for a viewing procedurally generated tile-based content
 *
 * This application is keyboard-controlled, and as a result, only
 * works on the desktop platforms.
 */
class App : public cugl::Application {
protected:
    /** The main controller for our game */
    // std::unique_ptr<GameController> _gameController;
    /** A 3152 style SpriteBatch to render the scene */
    std::shared_ptr<cugl::SpriteBatch>  _batch;
    /** The global asset manager */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** The variable to determine which scene is being active */
    string active_scene;
    
    std::shared_ptr<GamePlayController> _gameplayController;
    std::shared_ptr<LoadingController> _loadingController;
    /** Whether or not we have finished loading all assets */
    bool _loaded;
    
public:
    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    App() : Application(), _loaded(false) {}
    
    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~App() { }
    
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
    virtual void onStartup() override;

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
    virtual void onShutdown() override;
    
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
    virtual void update(float timestep) override;
    
    /**
     * The method called to draw the application to the screen.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should OpenGL and related drawing calls.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     */
    virtual void draw() override;
    
};

#endif /* __PV_APP_H__ */
