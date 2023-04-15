//
//  WallView.h
//  Tilemap
//
//  Created by Hao Chen on 4/15/23.
//

#ifndef WallView_h
#define WallView_h

#include <cugl/cugl.h>
#include <math.h>

class WallView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::PolygonNode> _node;
    
#pragma mark Main Functions
public:
    /** contructor */
    WallView(Vec2 position, float rot, Size size, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        _node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_CENTER);
        _node->setAngle(-rot * M_PI/180);
        setPosition(position);
//        setSize(size);
    }
    
    ~WallView(){
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
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        auto tileNode = scene2::SceneNode::alloc();
        std::shared_ptr<Texture> texture  = assets->get<Texture>(textureKey);
        tileNode = scene2::PolygonNode::allocWithTexture(texture);
         
        _node->addChild(tileNode);
    }
    
    Vec2 nodePos(){
        return _node->getPosition();
    }
    
    void setVisibility(bool visible){
        _node->setVisible(visible);
    }
    
};

#endif /* WallView_h */
