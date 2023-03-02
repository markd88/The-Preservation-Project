#include <cugl/cugl.h>
using namespace cugl;

class TileView {
    
#pragma mark Internal References
private:
    /** Main tile view */
    std::shared_ptr<scene2::PolygonNode> _node;
    
#pragma mark Main Functions
public:
    /**
     * Creates the view.
     *
     * In addition to the passed arguments, the node's anchor is set to the
     * bottom left and it should not be tinted relative to it's parent's color.
     *
     * @param position  The bottom left corner of the tile
     * @param size The width and height of a tile
     * @param color The tile color
     */
    TileView(Vec2 position, Size size, Color4 color) {
        // TODO: Implement me
        this->_node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setRelativeColor(false);
        setColor(color);
        setPosition(position);
        setSize(size);
    }
    
    /**
     * Deletes this TileView
     *
     * Upon destruction, you should remove the node from its parent.
     */
    ~TileView() {
        // TODO: Implement me
        auto scene = _node->getParent();
        if(scene != nullptr){
            scene->removeChild(_node);
        }
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<scene2::SceneNode>& sceneNode) {
        // TODO: Implement me
        sceneNode->addChild(_node);
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<scene2::SceneNode>& sceneNode) {
        // TODO: Implement me
        sceneNode->removeChild(_node);
    }

#pragma mark Setters
public:
    /**
     * Sets the position of the bottom left corner of the tile.
     *
     * @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        // TODO: Implement me
        this->_node->setPosition(position);
    }
    
    /**
     * Sets the size of the tile.
     *
     * As the view is a PolygonNode, this means setting a new polygon
     * that is a rectangle whose size is the one given. The rectangle
     * however, is only the shape, so it can have a zero origin.
     *
     * @param size Width and height of a single tile
     */
    void setSize(Size size) {
        // TODO: Implement me
        this->_node->setContentSize(size);
    }
    
    /**
     *  Sets the color of the tile.
     *
     *  @param color The color of the tile
     */
    void setColor(Color4 color) {
        // TODO: Implement me
        _node->setColor(color);
    }

    /**
     *  Detect if this file contains a point
     *
     *  @param point, the position of the point
     */
    bool contains(Vec2 point){
        Vec2 global_pos = _node->getWorldPosition();
        Size s = _node->getSize();
        bool hor = (point.x >= global_pos.x && point.x <= global_pos.x + s.width);
        bool ver = (point.y >= global_pos.y && point.y <= global_pos.y + s.height);
        return hor && ver;
    }
};
