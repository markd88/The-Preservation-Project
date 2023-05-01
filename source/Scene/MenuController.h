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
    
    int _highestUnlocked;

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
        dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
        
        _scene = cugl::Scene2::alloc(dimen);
        
        _assets = assets;
        // now menu assets loaded in loading scene
        // load menu assets
        
        _assets->loadDirectory("json/menu.json");
        auto menu_backdrop = _assets->get<scene2::SceneNode>("menu-backdrop");
        menu_backdrop->setPosition(_scene->getSize()/2);
        _scene->addChild(menu_backdrop);
        
        
        auto menu_layer = _assets->get<scene2::SceneNode>("menu");
        menu_layer->setPosition(_scene->getSize()/2);
       
        Vec2 menu_size = menu_layer->getSize();
        Vec2 space = Vec2 (menu_size.x/8, menu_size.y/5);
        Vec2 level_pos = Vec2 (space.x*2, space.y*3);
        auto level_font = _assets->get<cugl::Font>("courier");
        auto level_color = Color4(206, 144, 23);
        for (int i=0; i<15; i++){
//            _level_buttons[i] = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("menu_backdrop_grid_button"+to_string(i+1)));
            // 1. get level num
            auto level_num = to_string(i+1);
            auto level_label = std::make_shared<cugl::scene2::Label>();
            level_label->initWithText(level_num, level_font);
            level_label->setForeground(level_color);
            level_label->setScale(0.4);
            
            // 2. get button image
            std::shared_ptr<cugl::scene2::PolygonNode> button_image = std::make_shared<cugl::scene2::PolygonNode>();
            button_image->initWithTexture(_assets->get<Texture>("menu_selectbutton"));
            button_image->addChild(level_label);
            level_label->setAnchor(Vec2(0.5, 0.5));
            level_label->setPosition(button_image->getSize()/2);
            
            // get complete image
            std::shared_ptr<cugl::scene2::PolygonNode> complete_image = std::make_shared<cugl::scene2::PolygonNode>();
            complete_image->initWithTexture(_assets->get<Texture>("menu_complete"));
            button_image->addChild(complete_image);
            complete_image->setAnchor(Vec2(0.5, 0.5));
            complete_image->setPosition(Vec2(button_image->getSize().width*0.75, button_image->getSize().height*0.25));
            
            // 3. get button
            auto level_button = std::make_shared<cugl::scene2::Button>();
            level_button->init(button_image);
            
            menu_layer->addChild(level_button);
            level_button->setAnchor(Vec2(0.5, 0.5));
            level_button->setPosition(level_pos);
            
            if((i+1) % 5 == 0){
                level_pos = Vec2(space.x*2, level_pos.y - space.y+1);
            }
            else{
                level_pos += Vec2(space.x, 0);
            }
            
            
   
            _level_buttons[i] = level_button;
            
            
            _level_buttons[i]->setVisible(true);

        }
        _scene->addChild(menu_layer);
        
        updateMenu();
        return true;
    }
    
    void updateMenu(){
        for (int i=0; i<15; i++){
            auto button_img = std::dynamic_pointer_cast<scene2::PolygonNode>(_level_buttons[i]->getChildren()[0]);
            if (i > _highestUnlocked) {
                button_img->setTexture(_assets->get<Texture>("menu_lock"));
                button_img->getChildren()[0]->setVisible(false);
                button_img->getChildren()[1]->setVisible(false);
            }
            else {
                button_img->setTexture(_assets->get<Texture>("menu_selectbutton"));
                button_img->getChildren()[0]->setVisible(true);
                if(i < _highestUnlocked){
                    button_img->getChildren()[1]->setVisible(true);
                }else{
                    button_img->getChildren()[1]->setVisible(false);
                }
                _level_buttons[i]->activate();
                _level_buttons[i]->addListener([=](const std::string& name, bool down) {
                    if(!down){
                        // this->_scene->setActive(down);
                        nextScene = GAMEPLAY;
                        level = i % 4 + 1;
                    }
                });
            }
        }
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
    
    void setHighestUnlocked(int highestUnlocked) {
        _highestUnlocked = highestUnlocked;
    }
};
