//
//  MenuController.h
//  Tilemap
//
//  Created by Yichen Qiu on 3/27/23.
//

#include <cugl/cugl.h>
#include <common.h>

using namespace cugl;
using namespace std;

// the level user picked
// int level = 0;

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

class MenuController{
public:
    std::shared_ptr<cugl::Scene2> _scene;

    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    vector<shared_ptr<cugl::scene2::Button>> _level_buttons;

    MenuController() {
        _level_buttons = vector<shared_ptr<cugl::scene2::Button>>(15);
    }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~MenuController() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose(){
        
    }
    
    bool init(const std::shared_ptr<cugl::AssetManager>& assets){
        Size dimen = Application::get()->getDisplaySize();
        // dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
        
        _scene = cugl::Scene2::alloc(dimen);
        
        _assets = assets;
        // now menu assets loaded in loading scene
        // load menu assets
        
        _assets->loadDirectory("json/menu.json");
        
        auto layer = _assets->get<scene2::SceneNode>("menu");
        layer->setContentSize(dimen);
        // layer->doLayout(); // This rearranges the children to fit the screen
        
        for (int i=0; i<15; i++){
            _level_buttons[i] = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("menu_backdrop_grid_button"+to_string(i+1)));
            _level_buttons[i]->setVisible(true);
            _level_buttons[i]->activate();
            _level_buttons[i]->addListener([=](const std::string& name, bool down) {
                if(!down){
                    // this->_scene->setActive(down);
                    nextScene = GAMEPLAY;
                    level = i % 4 + 1;
                }
                
            });

        }
        
        
        _scene->addChild(layer);
        return true;
    }
    
    void update(float timestep){
        
    }

    void render(std::shared_ptr<SpriteBatch> &batch){
        _scene->render(batch);
    }
    
    void setActive(bool active){
        if(active){
            for(auto button : _level_buttons){
                button->activate();
            }
        }
        else{
            for(auto button : _level_buttons){
                button->deactivate();
            }
        }
    }
};
