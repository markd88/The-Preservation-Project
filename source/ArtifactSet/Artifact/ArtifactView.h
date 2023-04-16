//
//  ArtifactView.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef ArtifactView_h
#define ArtifactView_h

#include <cugl/cugl.h>
#include <math.h>
//using namespace cugl;

class ArtifactView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
//    std::shared_ptr<scene2::PolygonNode> _node;
    std::shared_ptr<scene2::SceneNode> _node;
    
    
#pragma mark Main Functions
public:
    /** contructor */
    ArtifactView(Vec2 position, float angle, Size size, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
//        float scale = GAME_WIDTH/size.width;
//        size *= scale;
        _node = scene2::PolygonNode::alloc();
        setPosition(position);
        setAngle(-angle * M_PI/180);
//        setSize(size);
    }
    
    ~ArtifactView(){
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
    
    void setAngle(float angle){
        _node->setAngle(angle);
    }
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        //        auto node = scene2::SceneNode::alloc();
        Vec2 pos = nodePos();
        float angle = _node->getAngle();
//        Size size = _node->getSize();
        std::shared_ptr<Texture> texture  = assets->get<Texture>(textureKey);
        _node = scene2::PolygonNode::allocWithTexture(texture);
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        setPosition(pos);
        setAngle(angle);
//        setSize(size);
//        _node->addChild(node);
    }

    
    Vec2 nodePos(){
        return _node->getPosition();
    }
    
    void setVisibility(bool visible){
        _node->setVisible(visible);
    }
    
};

#endif /* ArtifactView_h */
