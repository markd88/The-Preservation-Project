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
    std::shared_ptr<scene2::SceneNode> _tileNode;
    
    
#pragma mark Main Functions
public:
    /** contructor */
    ArtifactView(Vec2 position, float rot, Size size, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
//        float scale = GAME_WIDTH/size.width;
//        size *= scale;
        _tileNode = scene2::PolygonNode::alloc();
//        std::shared_ptr<Texture> texture  = assets->get<Texture>(textureKey);
//        _tileNode = scene2::PolygonNode::allocWithTexture(texture);
        _tileNode->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
        _tileNode->setAngle(-rot * M_PI/180);
        setPosition(position);
//        setSize(size);
    }
    
    ~ArtifactView(){
        auto parent = _tileNode->getParent();
        if (parent != nullptr && _tileNode != nullptr) {
            parent->removeChild(_tileNode);
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
        scene->addChild(_tileNode);
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        scene->removeChild(_tileNode);
    }

#pragma mark Setters
public:
    void setPosition(Vec2 position){
        _tileNode->setPosition(position);
    }
    
    void setSize(Size size){
        //_node->setPolygon(Rect(Vec2(0,0), size));
        _tileNode->setContentSize(size);
    }
    
    void setColor(Color4 color){
        _tileNode->setColor(color);
    }
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        //        auto tileNode = scene2::SceneNode::alloc();
        std::shared_ptr<Texture> texture  = assets->get<Texture>(textureKey);
        _tileNode = scene2::PolygonNode::allocWithTexture(texture);
//        setPosition(position);
    }
//        _node->addChild(tileNode);
    
    Vec2 nodePos(){
        return _tileNode->getPosition();
    }
    
    void setVisibility(bool visible){
        _tileNode->setVisible(visible);
    }
    
};

#endif /* ArtifactView_h */
