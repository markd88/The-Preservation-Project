//
//  CharacterModel.h
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef CharacterModel_h
#define CharacterModel_h

#include <cugl/cugl.h>
using namespace cugl;

class CharacterModel{
private:
    /** Center of the character */
    Vec2 _position;
    Size _size;
    Color4 _color;
    
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
     */
    CharacterModel(Vec2 position, Size size, Color4 color): color(_color) {
        setPosition(position);
        setSize(size);
        setColor(color);
    }

#pragma mark Setters
public:
    /**
     *  Sets the position of the bottom left corner of the tile.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }
    
    /**
     *  Sets the size of the tile.
     *
     *  @param size Width and height of a single tile
     */
    void setSize(Size size) {
        _size = size;
    }
    
    /**
     *  Sets the color of the tile.
     *
     *  @param color The color of the tile
     */
    void setColor(Color4 color) {
        _color = color;
    }
    
};
#endif /* CharacterModel_h */
