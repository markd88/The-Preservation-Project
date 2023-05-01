#include <cugl/cugl.h>
using namespace cugl;

class TileModel {
    
#pragma mark State
private:
    /** Bottom left corner of the tile */
    Vec2 _position;
    /** Width and height of a single tile */
    Size _size;
    /** Tile color */
    Color4 _color;
    /** If the tile is a obstacle*/
    bool _is_obs;
    std::string _textureKey;
    
public:
    /** A public accessible, read-only version of the color */
    const Color4& color;
    
#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     * @param is_obs     if the tile is an obstacle
     */
    TileModel(Vec2 position, Size size, Color4 color, bool is_obs): color(_color) {
        // TODO: Implement me
        _is_obs = is_obs;
        setColor(color);
        setPosition(position);
        setSize(size);
         
    }
    
    TileModel(Vec2 position, Size size, Color4 color, std::string textureKey, bool is_obs): color(_color) {
        // TODO: Implement me
        _is_obs = is_obs;
        setColor(color);
        setPosition(position);
        setSize(size);
        setTextureKey(textureKey);
    }

#pragma mark Setters
public:
    
    void setTextureKey(std::string textureKey) {
        this->_textureKey = textureKey;
    }
    /**
     *  Sets the position of the bottom left corner of the tile.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        // TODO: Implement me
        this->_position = position;
        
    }
    
    /**
     *  Sets the size of the tile.
     *
     *  @param size Width and height of a single tile
     */
    void setSize(Size size) {
        // TODO: Implement me
        this->_size = size;
        
    }
    
    /**
     *  Sets the color of the tile.
     *
     *  @param color The color of the tile
     */
    void setColor(Color4 color) {
        // TODO: Implement me
        _color = color;
        
    }
#pragma mark Getters
public:
    /**
     *  Get if the tile is obstacle
     *
     *  @param is_obs
     */
    bool is_obs(){
        return _is_obs;
    }
    
    std::string getTextureKey() {
        return _textureKey;
    }
    
    Size getSize(){
        return _size;
    }
};
