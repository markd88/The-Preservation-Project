//
//  GuardView.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef GuardView_h
#define GuardView_h

#include <cugl/cugl.h>
using namespace cugl;

#include <math.h>

#define DURATION 1.0f

class GuardView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::SpriteNode> _node;
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;

    std::shared_ptr<cugl::scene2::Animate> _guard_0;
    std::shared_ptr<cugl::scene2::Animate> _guard_1;
    std::shared_ptr<cugl::scene2::Animate> _guard_2;
    std::shared_ptr<cugl::scene2::Animate> _guard_3;
    std::shared_ptr<cugl::scene2::Animate> _guard_4;
    std::shared_ptr<cugl::scene2::Animate> _guard_5;
    std::shared_ptr<cugl::scene2::Animate> _guard_6;
    std::shared_ptr<cugl::scene2::Animate> _guard_7;


#pragma mark Main Functions
public:
    /** contructor */
    GuardView(Vec2 position, Size size, Color4 color, const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions) {
        // Get the image and add it to the node.
        _actions = actions;
        float scale = GAME_WIDTH/size.width;
        // size *= scale;

        std::shared_ptr<Texture> texture  = assets->get<Texture>("guard");
        _node = scene2::SpriteNode::allocWithSheet(texture, 16, 16, 256); // SpriteNode for animation
        _node->setScale(0.6f); // Magic number to rescale asset

        _node->setRelativeColor(false);
        _node->setVisible(true);
        _node->setAnchor(Vec2::ANCHOR_CENTER);
        _node->setPosition(position);


        std::vector<int> d0 = {1,2,3,4,5,6,7,0};
        _guard_0 = cugl::scene2::Animate::alloc(d0, DURATION);


        std::vector<int> d1 = {9,10,11,12,13,14,15,8};
        _guard_1 = cugl::scene2::Animate::alloc(d1, DURATION);


        std::vector<int> d2 = {17,18,19,20,21,22,23,16};
        _guard_2 = cugl::scene2::Animate::alloc(d2, DURATION);


        std::vector<int> d3 = {25,26,27,28,29,30,31,24};
        _guard_3 = cugl::scene2::Animate::alloc(d3, DURATION);


        std::vector<int> d4 = {33,34,35,36,37,38,39,32};
        _guard_4 = cugl::scene2::Animate::alloc(d4, DURATION);

        std::vector<int> d5 = {41,42,43,44,45,46,47,40};
        _guard_5 = cugl::scene2::Animate::alloc(d5, DURATION);

        std::vector<int> d6 = {49,50,51,52,53,54,55,48};
        _guard_6 = cugl::scene2::Animate::alloc(d6, DURATION);

        std::vector<int> d7 = {57,58,59,60,61,62,63,56};
        _guard_7 = cugl::scene2::Animate::alloc(d7, DURATION);


    }
    
    ~GuardView(){
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
        _node->setContentSize(size);
    }

    Vec2 nodePos(){
        return _node->getPosition();
    }
    
    void performAction(string actionName, const std::shared_ptr<cugl::scene2::MoveTo>& action){
        _actions->activate(actionName, action, _node);
    }


    void performAnimation(string actionName, int d) {
        CULog("%d", d);
        std::shared_ptr<cugl::scene2::Animate> animation = _guard_0;
        if (d == 0) {
            animation = _guard_0;
        } else if (d == 1){
            animation = _guard_1;
        }
        else if (d == 2){
            animation = _guard_2;
        }
        else if (d == 3){
            animation = _guard_3;
        }
        else if (d == 4){
            animation = _guard_4;
        }
        else if (d == 5){
            animation = _guard_5;
        }else if (d == 6){
            animation = _guard_6;
        }
        else if ( d ==7 ){
            animation = _guard_7;
        }

        _actions->activate(actionName, animation, _node);
    }
    
#pragma mark Helpers
public:
    
   
    void setVisibility(bool visible){
        _node->setVisible(visible);
    }
};

#endif /* GuardView_h */
