#include <cugl/cugl.h>
#include <cugl/base/CUBase.h>

using namespace cugl;

class TileView {
    
#pragma mark Internal References
private:
    /** Main tile view */
    std::shared_ptr<scene2::PolygonNode> _node;
    std::string _textureKey;
    
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
    
    TileView(Vec2 position, Size size, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        // TODO: Implement me
        this->_node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setRelativeColor(false);
        setPosition(position);
        setSize(size);
//        setTextureKey(textureKey);
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
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        auto tileNode = scene2::SceneNode::alloc();
        std::shared_ptr<Texture> texture  = assets->get<Texture>(textureKey);
        tileNode = scene2::PolygonNode::allocWithTexture(texture);
         
        _node->addChild(tileNode);
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
    
    bool containsLocally(Vec2 point){
        Vec2 pos = _node->getPosition();
        Size s = _node->getSize();
        bool hor = (point.x >= pos.x && point.x <= pos.x + s.width);
        bool ver = (point.y >= pos.y && point.y <= pos.y + s.height);
        return hor && ver;
    }
    
    bool containsLine(Vec2 a, Vec2 b){
        
        if (containsLocally(a) or containsLocally(b)){
            return true;
        }
            
        Vec2 pos = _node->getPosition();
        Size s = _node->getSize();
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
    
    //determine if two lines intersect
    bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        // calculate the distance to intersection point
        float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        
        // if uA and uB are between 0-1, lines are colliding
        return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
    }
     
};
