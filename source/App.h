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
#include <Scene/MenuController.h>
#include <common.h>
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
    
    /** The loading controller*/
    std::shared_ptr<LoadingController> _loadingController;
    
    
    std::shared_ptr<MenuController> _menuController;
    
    /** Whether or not we have finished loading all assets */
    bool _loaded;
        
    std::string _saveDir;
    
    int _highestUnlocked = -1;
    
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
    App() : Application() {}
    
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
    virtual void onSuspend() override;
    
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
    virtual void onResume()  override;
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
    
    
    // methods for save progress
    void createSave(){
        writeSave(1);
    }

    int readSave(){
        // return -1 if file not exists
        bool exists = cugl::filetool::file_exists(_saveDir);
        if(!exists) return -1;
        auto read = JsonReader::alloc(_saveDir);
        auto temp = read->readAll();
        
        std::string highestStr = temp.substr(0, temp.size()-1);
        int highest = stoi(highestStr);
        read->close();
        return highest;
    }

    void writeSave(int highest){
        // write into saveDir
        auto write = JsonWriter::alloc(_saveDir);
        std::shared_ptr<cugl::JsonValue> saveValue = cugl::JsonValue::alloc(cugl::JsonValue::Type::NumberType);
        saveValue->init(highest*1.0);
        write->writeJson(saveValue);
        write->close();
    }

};

#endif /* __PV_APP_H__ */
