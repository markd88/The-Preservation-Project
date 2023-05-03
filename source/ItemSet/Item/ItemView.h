//
//  ItemView.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef ItemView_h
#define ItemView_h

#include <cugl/cugl.h>
#include <math.h>
//using namespace cugl;

class ItemView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/

    std::shared_ptr<scene2::PolygonNode> _static_node;
    std::shared_ptr<scene2::SpriteNode> _anim_node;
    bool _isArtifact;
    bool _isResource;
    bool _isObs;

    std::shared_ptr<cugl::scene2::Animate> _c_0;

    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;

    int _id;
    
#pragma mark Main Functions
public:
    /** contructor */
    ItemView(Vec2 position, float angle, Size size, bool isArtifact, bool isResource, bool isObs, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey, int id) {
        _static_node = scene2::PolygonNode::alloc();
        setPosition(position);

        _isArtifact = isArtifact;
        _isResource = isResource;
        _isObs = isObs;
        _id = id;


        std::vector<int> d0 = {1,2,3,4,5,6,7,0};
        _c_0 = cugl::scene2::Animate::alloc(d0, 1.0f);


    }
    
    ~ItemView(){
        auto parent = _static_node->getParent();
        if (parent != nullptr && _static_node != nullptr) {
            parent->removeChild(_static_node);
        }
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::scene2::OrderedNode>& scene) {
        scene->addChild(_static_node);
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::scene2::OrderedNode>& scene) {
        scene->removeChild(_static_node);
    }

    void removeAnim() {
        _anim_node->setVisible(false);
    }

#pragma mark Setters
public:
    void setPosition(Vec2 position){
        _static_node->setPosition(position);
    }

    void setAction(std::shared_ptr<cugl::scene2::ActionManager> actions) {
        _actions = actions;
    }

    void setSize(Size size){
        _static_node->setContentSize(size);
    }
    
    void setAngle(float angle){
        _static_node->setAngle(angle);
    }
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        //        auto node = scene2::SceneNode::alloc();
        Vec2 pos = nodePos();

        if (_isResource) {
            std::shared_ptr<Texture> texture  = assets->get<Texture>("clock");
            _static_node = scene2::PolygonNode::allocWithTexture(texture);
            _static_node->setAnchor(Vec2::ANCHOR_CENTER);
            setPosition(pos + Vec2(64,64));
            std::shared_ptr<Texture> textureAnim  = assets->get<Texture>("clock_Anim");
            _anim_node = scene2::SpriteNode::allocWithSheet(textureAnim, 2, 4, 8);

            _static_node->addChild(_anim_node);
            _anim_node->setVisible(true);

        }
        else if (_isArtifact ) {
            std::shared_ptr<Texture> texture  = assets->get<Texture>("vase");
            _static_node = scene2::PolygonNode::allocWithTexture(texture);
            _static_node->setAnchor(Vec2::ANCHOR_CENTER);
            setPosition(pos + Vec2(64,64));
            std::shared_ptr<Texture> textureAnim  = assets->get<Texture>("vase_Anim");
            _anim_node = scene2::SpriteNode::allocWithSheet(textureAnim, 2, 4, 8);

            _static_node->addChild(_anim_node);
            _anim_node->setVisible(true);

        }

        else {
            std::shared_ptr<Texture> texture  = assets->get<Texture>(textureKey);
            _static_node = scene2::PolygonNode::allocWithTexture(texture);
            _static_node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
            setPosition(pos);
        }
        if (_isObs) {

            setVisibility(false);
        }
    }


    void updateAnim() {
        if (_actions->isActive("item"+ std::to_string(_id))){
            // let it finish
        }
        else {
            _actions->activate("item"+ std::to_string(_id), _c_0, _anim_node);
        }

    }
    
    Vec2 nodePos(){
        return _static_node->getPosition();
    }
    
    void setVisibility(bool visible){
        _static_node->setVisible(visible);
    }
    
    /**
     *  Detect if this file contains a point
     *
     *  @param point, the position of the point
     */
    bool contains(Vec2 point){
        Vec2 global_pos = _static_node->getWorldPosition();
        Size s = _static_node->getSize();
        // Add a offset so that character & guard don't go too close to wall
        int offset = 5;
        bool hor = (point.x >= global_pos.x - offset && point.x <= global_pos.x + s.width + offset);
        bool ver = (point.y >= global_pos.y - offset && point.y <= global_pos.y + s.height + offset);
        return hor && ver;
    }
    
    
    bool containsLine(Vec2 a, Vec2 b){
        
        if (contains(a) or contains(b)){
            return true;
        }
    
        Vec2 pos = _static_node->getWorldPosition();
        Size s = _static_node->getSize();
        float rx = pos.x;
        float ry = pos.y;
        float rw = s.width;
        float rh = s.height;
        bool left = lineLine(a.x,a.y, b.x, b.y, rx, ry, rx, ry + rh);
        bool right = lineLine(a.x,a.y, b.x, b.y, rx + rw, ry, rx + rw, ry + rh);
        bool top = lineLine(a.x,a.y, b.x, b.y, rx, ry + rh, rx + rw, ry + rh);
        bool bottom = lineLine(a.x,a.y,b.x,b.y, rx, ry, rx+rw, ry);
        
        return (left or right or top or bottom);
         
    }
    
    bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        // calculate the distance to intersection point
        float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        
        // if uA and uB are between 0-1, lines are colliding
        return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
    }
    
    // update priority based on its y coor
    void updatePriority(){
        if(_isArtifact){
            _static_node->setPriority(0);
            if(_anim_node != nullptr )_anim_node->setPriority(-1);
        }
        else{
            _static_node->setPriority(_static_node->getPosition().y);
            if(_anim_node != nullptr )_anim_node->setPriority(_static_node->getPosition().y-1);
        }
        
    }
};

#endif /* ItemView_h */
