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
#define DURATION 0.8f

class CharacterView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<cugl::scene2::SpriteNode>  _node;

    std::shared_ptr<cugl::scene2::PolygonNode>  _shadow;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;

    std::shared_ptr<cugl::scene2::Animate> _c_0;
    std::shared_ptr<cugl::scene2::Animate> _c_1;
    std::shared_ptr<cugl::scene2::Animate> _c_2;
    std::shared_ptr<cugl::scene2::Animate> _c_3;
    std::shared_ptr<cugl::scene2::Animate> _c_4;
    std::shared_ptr<cugl::scene2::Animate> _c_5;
    std::shared_ptr<cugl::scene2::Animate> _c_6;
    std::shared_ptr<cugl::scene2::Animate> _c_7;

    // last direction
    int _last_direction;
    
    
#pragma mark Main Functions
public:
    /** contructor */
    CharacterView(Vec2 position, Size size, Color4 color, std::shared_ptr<cugl::scene2::ActionManager> actions, const std::shared_ptr<cugl::AssetManager>& assets){

        _last_direction = 0;
        _actions = actions;
        // Get the image and add it to the node.
        float scale = GAME_WIDTH/size.width;
        size *= scale;
        

        std::shared_ptr<Texture> character  = assets->get<Texture>("character");
        _node = scene2::SpriteNode::allocWithSheet(character, 8, 8, 64); // SpriteNode for animation
        _node->setScale(0.8f); // Magic number to rescale asset

        _node->setRelativeColor(false);
        _node->setVisible(true);
        _node->setAnchor(Vec2(0.5, 0.25));
        _node->setPosition(position);

        _node->setFrame(16);

        std::shared_ptr<Texture> shadow = assets->get<Texture>("shadow");
        _shadow = scene2::PolygonNode::allocWithTexture(shadow);
        _shadow->setScale(0.2f);
        _shadow->setAnchor(0,0);
        _shadow->setPosition(_node->getPosition());
        _shadow->setRelativeColor(false);
        _shadow->setVisible(true);



        _node->addChildWithName(_shadow, "shadow");





        std::vector<int> d0 = {1,2,3,4,5,6,7,0};
        _c_0 = cugl::scene2::Animate::alloc(d0, DURATION);

        std::vector<int> d1 = {9,10,11,12,13,14,15,8};
        _c_1 = cugl::scene2::Animate::alloc(d1, DURATION);

        std::vector<int> d2 = {17,18,19,20,21,22,23,16};
        _c_2 = cugl::scene2::Animate::alloc(d2, DURATION);

        std::vector<int> d3 = {25,26,27,28,29,30,31,24};
        _c_3 = cugl::scene2::Animate::alloc(d3, DURATION);

        std::vector<int> d4 = {33,34,35,36,37,38,39,32};
        _c_4 = cugl::scene2::Animate::alloc(d4, DURATION);

        std::vector<int> d5 = {41,42,43,44,45,46,47,40};
        _c_5 = cugl::scene2::Animate::alloc(d5, DURATION);

        std::vector<int> d6 = {49,50,51,52,53,54,55,48};
        _c_6 = cugl::scene2::Animate::alloc(d6, DURATION);

        std::vector<int> d7 = {57,58,59,60,61,62,63,56};
        _c_7 = cugl::scene2::Animate::alloc(d7, DURATION);




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
    
    void moveTo(const std::shared_ptr<cugl::scene2::MoveTo>& action){
        //auto fcn = EasingFunction::alloc(EasingFunction::Type::ELASTIC_IN_OUT);
        _actions->activate("moving", action, _node);
    }

    void updateAnimation(Vec2 target) {

        std::shared_ptr<cugl::scene2::Animate> animation = _c_0;
        Vec2 pos = _node->getPosition();
        int d = calculateMappedAngle(pos.x, pos.y, target.x, target.y);

        if (d == 0) {
            animation = _c_0;
        } else if (d == 1){
            animation = _c_1;
        }
        else if (d == 2){
            animation = _c_2;
        }
        else if (d == 3){
            animation = _c_3;
        }
        else if (d == 4){
            animation = _c_4;
        }
        else if (d == 5){
            animation = _c_5;
        }else if (d == 6){
            animation = _c_6;
        }
        else if ( d ==7 ){
            animation = _c_7;
        }

        _actions->activate("character_animation", animation, _node);
    }

    void stopAnimation() {
        _actions->remove("character_animation");
    }

    void updateLastDirection(Vec2 target) {
        Vec2 pos = _node->getPosition();
        int d = calculateMappedAngle(pos.x, pos.y, target.x, target.y);

        if (!_actions->isActive("character_animation")) {
            updateAnimation(target);
        } else if (_actions->isActive("character_animation") && d == _last_direction) {
            // continue the animation
        } else if (_actions->isActive("character_animation") && d != _last_direction) {
            _actions->remove("character_animation");
            updateAnimation(target);
        }
        _last_direction = d;
    }

    Vec2 nodePos(){
        return _node->getPosition();
    }
    
    float getAngle(){
        return _node->getAngle();
    }

    int calculateMappedAngle(float x1, float y1, float x2, float y2)
    {
        // calculate the angle in radians
        double angleRadians = atan2(y2 - y1, x2 - x1);

        // convert the angle to degrees
        float angleDegrees = angleRadians * 180.0 / M_PI;

        // make sure the angle is between 0 and 360 degrees
        if (angleDegrees < 0.0)
        {
            angleDegrees += 360.0;
        }

        // map the angle from 0 to 360 degrees to 0 to 7
     //    CULog("%f", angleDegrees);
        if (angleDegrees > 337.5 || angleDegrees < 22.5) {
            return 2;
        } else if (angleDegrees >= 22.5 && angleDegrees < 67.5){
            return 1;
        }
        else if (angleDegrees >= 67.5 && angleDegrees < 112.5){
            return 0;
        }
        else if (angleDegrees >= 112.5 && angleDegrees < 157.5){
            return 7;
        }
        else if (angleDegrees >= 157.5 && angleDegrees < 202.5){
            return 6;
        }
        else if (angleDegrees >= 202.5 && angleDegrees < 247.5){
            return 5;
        }
        else if (angleDegrees >= 247.5 && angleDegrees < 292.5){
            return 4;
        }else if (angleDegrees >= 292.5 && angleDegrees < 337.5){
            return 3;
        }
        return 0;


    }




};


#endif /* CharacterView_h */
