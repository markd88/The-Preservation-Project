//
//  CharacterView.h
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef CharacterView_h
#define CharacterView_h
#include <cugl/cugl.h>
using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

class CharacterView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<cugl::scene2::SceneNode>  _node;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;
    
    
#pragma mark Main Functions
public:
    /** contructor */
    CharacterView(Vec2 position, Size size, Color4 color, std::shared_ptr<cugl::scene2::ActionManager> actions, const std::shared_ptr<cugl::AssetManager>& assets){
        // initialize view

        // Get the image and add it to the node.
        float scale = GAME_WIDTH/size.width;
        size *= scale;
        
        _node = scene2::SceneNode::alloc();
        
        std::shared_ptr<Texture> texture  = assets->get<Texture>("character");
        _node = scene2::SpriteNode::allocWithSheet(texture, 1, 1, 1); // SpriteNode for animation
        _node->setScale(0.1f); // Magic number to rescale asset

        _node->setRelativeColor(false);
        _node->setVisible(true);
        _node->setAnchor(Vec2(0.5, 0.25));
        _actions = actions;

        // initialize state
        setSize(size);
        setPosition(position);
//        setColor(Color4::BLACK);
    }
    
    ~CharacterView(){
        auto parent = _node->getParent();
        if (parent != nullptr && _node != nullptr) {
            parent->removeChild(_node);
        }
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->addChild(_node);
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_node);
    }

#pragma mark Setters
public:
    void setPosition(Vec2 position){
        _node->setPosition(position);
    }
    
    void setSize(Size size){
        //_node->setPolygon(Rect(Vec2(0,0), size));
        _node->setContentSize(size);
    }
    
    void setColor(Color4 color){
        _node->setColor(color);
    }
    
    void moveTo(const std::shared_ptr<cugl::scene2::MoveTo>& action){
        //auto fcn = EasingFunction::alloc(EasingFunction::Type::ELASTIC_IN_OUT);
        _actions->activate("moving", action, _node);
    }
    
    Vec2 nodePos(){
        return _node->getPosition();
    }
};


#endif /* CharacterView_h */
