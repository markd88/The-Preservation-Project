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

class CharacterView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::PolygonNode> _node;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;
    
    
#pragma mark Main Functions
public:
    /** contructor */
    CharacterView(Vec2 position, Size size, Color4 color, std::shared_ptr<cugl::scene2::ActionManager> actions){
        // TODO: Implement me
        // initialize view
        _node = scene2::PolygonNode::alloc();
        _node->setRelativeColor(false);
        _node->setAnchor(Vec2::ANCHOR_CENTER);
        _actions = actions;

        // initialize state
        setSize(size);
        setPosition(position);
        setColor(color);
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
        _actions->activate("moving", action, _node);
    }
};


#endif /* CharacterView_h */
