//
//  ArtifactView.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef ArtifactView_h
#define ArtifactView_h

#include <cugl/cugl.h>
//using namespace cugl;

class ArtifactView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::PolygonNode> _node;
    
#pragma mark Main Functions
public:
    /** contructor */
    ArtifactView(Vec2 position, Size size, Color4 color) {
        
        this->_node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setRelativeColor(false);
        setColor(color);
        setPosition(position);
        setSize(size);
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
    
    void setColor(Color4 color){
        _node->setColor(color);
    }
    
    Vec2 nodePos(){
        return _node->getPosition();
    }
    
};

#endif /* ArtifactView_h */
