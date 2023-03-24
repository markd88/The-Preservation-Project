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

class GuardView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::SceneNode> _node;
    std::shared_ptr<Poly2> tri;
    std::shared_ptr<scene2::PolygonNode> _cone;
    
    
#pragma mark Main Functions
public:
    /** contructor */
    GuardView(Vec2 position, Size size, Color4 color, const std::shared_ptr<cugl::AssetManager>& assets) {
        // Get the image and add it to the node.
//        float scale = GAME_WIDTH/size.width;
//        size *= scale;
        _node = scene2::SceneNode::alloc();
        std::shared_ptr<Texture> texture  = assets->get<Texture>("guard");
        _node = scene2::SpriteNode::allocWithSheet(texture, 1, 1, 1);
        _node->setScale(0.1f);
        _node->setRelativeColor(false);
        _node->setVisible(true);
        _node->setAnchor(Vec2::ANCHOR_CENTER);
        _node->setPosition(position);
        
        PolyFactory _polyFactory;
        Poly2 tri = _polyFactory.makeTriangle(Vec2(0,0), Vec2(0,200), Vec2(200,100));
        _cone = scene2::PolygonNode::allocWithPoly(tri);
        _cone->setScale(5);
        _node->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        _cone->setColor(color);
        _cone->setVisible(true);
        
        _node->addChild(_cone);
//        setPosition(position);
//        setSize(size);
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
//        _cone->setPosition(position);
    }
    
    void setSize(Size size){
//        _cone->setPolygon(Rect(Vec2(0,0), size));
        _node->setContentSize(size);
    }

    Vec2 nodePos(){
        return _node->getPosition();
    }
};

#endif /* GuardView_h */
