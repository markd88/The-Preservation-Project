#include <cugl/cugl.h>
using namespace cugl;

class TilemapView {
    
#pragma mark Internal References
private:
    /** Main tilemap view */
    std::shared_ptr<scene2::PolygonNode> _node;
    
#pragma mark Main Functions
public:
    /**
     * Initializes the view.
     *
     * In addition to the passed arguments, the node's anchor is set to the bottom left
     * and it should not be tinted relative to it's parent's color.
     *
     * @param position      The bottom left corner of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    TilemapView(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize) {
        // TODO: Implement me
        _node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        setPosition(position);
        setColor(color);
        setSize(dimensions * tileSize);
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `scene`.
     *
     * @param scene The scene to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
        // TODO: Implement me
        scene->addChild(_node);
        
    }
    
    /**
     * Removes the view component children from the given `scene`.
     *
     * @param scene The scene to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
        // TODO: Implement me
        scene->removeChild(_node);
    }
    
    /** Returns the main tilemap view */
    const std::shared_ptr<scene2::PolygonNode>& getNode() const {
        // TODO: Implement me
        return _node;
    }

#pragma mark Setters
public:
    
    /**
     * Sets the bottom left corner of the tilemap
     *
     * @param position Bottom left corner of the tilemap
     */
    void setPosition(Vec2 position) {
        // TODO: Implement me
        _node->setPosition(position);
    }
    
    /**
     * Sets the size of the tilemap.
     *
     * The size of the tilemap should be `dimensions * tileSize`. As
     * the view is a PolygonNode, this means setting a new polygon
     * that is a rectangle whose size is that of the tilemap. The
     * rectangle however, is only the shape, so it can have a zero
     * origin.
     *
     * @param size    The size of the tilemap based on its dimensions and tileSize
     */
    void setSize(Size size) {
        // TODO: Implement me
        _node->setContentSize(size);
    }
    
    /**
     * Sets the color of the tilemap.
     *
     * @param color The color of the tilemap
     */
    void setColor(Color4 color) {
        // TODO: Implement me
        _node->setColor(color);
    }
    
    void setVisibility(bool active){
        _node->setVisible(active);
    }
    
    void setPriority(float p){
        _node->setPriority(p);
    }
};
