//
//  MVCTileModel.h
//  TileMap Lab
//
//  This module provides the MVC version of the TileModel class.
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __MVC_TILE_MODEL_H__
#define __MVC_TILE_MODEL_H__
 
#include <cugl/cugl.h>
using namespace cugl;

namespace MVC {

/**
 * A class representing a single tile
 */
class TileModel {
    
#pragma mark State
private:
    /** Bottom left corner of the tile */
    Vec2 _position;
    /** Width and height of a single tile */
    Size _size;
    /** Tile color */
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
    TileModel(Vec2 position, Size size, Color4 color): color(_color) {
        // TODO: Implement me

        setColor(color);
        setPosition(position);
        setSize(size);
         
    }

#pragma mark Setters
public:
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
    
};
}

#endif /* __MVC_TILE_MODEL_H__ */
