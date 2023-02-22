//
//  CMVTilemapModel.h
//  TileMap Lab
//
//  This module provides the CMV version of the TilemapModel class.
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __CMV_TILEMAP_MODEL_H__
#define __CMV_TILEMAP_MODEL_H__

#include <cugl/cugl.h>
using namespace cugl;

namespace CMV {
 
/**
 * A class representing the entire tilemap.
 */
class TilemapModel {
    
#pragma mark Internal References
private:
    /** TilemapView  */
    std::shared_ptr<scene2::PolygonNode> _node;
    
#pragma mark State
private:
    /** Bottom left corner of the tilemap */
    Vec2 _position;
    /** The number of tiles per column and row   */
    Vec2 _dimensions;
    /** Tilemap color */
    Color4 _color;
    /** Width and height of a single tile */
    Size _tileSize;
    
public:
    /// Instead of writing getters for private states that need setters,
    /// you can create constant references to force users of this class
    /// to modify state through setters
    const Vec2& position;
    const Vec2& dimensions;
    const Color4& color;
    const Size& tileSize;
    
#pragma mark Main Functions
public:
    /**
     *  Creates the default tilemap.
     *
     *  The default tilemap is a black square whose bottom left corner is
     *  aligned with the bottom left corner of the screen. It's dimensions
     *  are 10 by 10 with tiles of size 5 by 5.
     */
    TilemapModel():
    position(_position),
    dimensions(_dimensions),
    color(_color),
    tileSize(_tileSize) {
        _node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        setPosition(Vec2(0,0));
        setDimensions(Vec2(10, 10));
        setColor(Color4::BLACK);
        setTileSize(Size(5,5));
        
    }
    
    /**
     * Creates the model state.
     *
     * Remember to put the anchor of the view node at the bottom left.
     *
     * @param position      The bottom left corner of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    TilemapModel(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize):
    position(_position),
    dimensions(_dimensions),
    color(_color),
    tileSize(_tileSize) {
        // TODO: Implement me
        _node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        setPosition(position);
        setDimensions(dimensions);
        setColor(color);
        setTileSize(tileSize);
    }
    
#pragma mark Getters
public:
    /** Returns the TilemapView */
    const std::shared_ptr<scene2::PolygonNode> getNode() const {
        // TODO: Implement me
        return this->_node;
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
        this->_position = position;
        this->_node->setPosition(position);
        
    }
    
    /**
     * Sets the dimensions of the tilemap.
     *
     * This modifies the model state and view node. The size of the
     * tilemap should be `dimensions * tileSize`. As the view is a
     * PolygonNode, this means setting a new polygon that is a rectangle
     * whose size is that of the tilemap. The rectangle however, is only
     * the shape, so it can have a zero origin.
     *
     * @param dimensions    The number of columns and rows in the tilemap
     */
    void setDimensions(Vec2 dimensions) {
        // TODO: Implement me
        _dimensions = dimensions;
        // not sure
        _node->setContentSize(_dimensions * _tileSize);
    }
        
    /**
     * Sets the size of all tiles in the tilemap.
     *
     * This modifies the model state and view node. The size of the
     * tilemap should be `dimensions * tileSize`. As the view is a
     * PolygonNode, this means setting a new polygon that is a rectangle
     * whose size is that of the tilemap.
     *
     * @param tileSize  The width and height of a tile
     */
    void setTileSize(Size tileSize) {
        // TODO: Implement me
        _tileSize = tileSize;
        _node->setContentSize(_dimensions * _tileSize);
    }
    
    /**
     * Sets the color of the tilemap.
     *
     * This modifies the model state and node.
     *
     * @param color The color of the tilemap
     */
    void setColor(Color4 color) {
        // TODO: Implement me
        this->_color = color;
        _node->setColor(color);
    }

    
};
}

#endif /* __CMV_TILEMAP_MODEL_H__ */
