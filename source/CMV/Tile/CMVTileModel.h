//
//  CMVTileModel.h
//  TileMap Lab
//
//  This module provides the CMV version of the TileModel class.
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __CMV_TILE_MODEL_H__
#define __CMV_TILE_MODEL_H__
#include <cugl/cugl.h>
using namespace cugl;

namespace CMV {

/**
 * A class representing a single tile
 */
class TileModel {
    
#pragma mark Internal References
private:
    /** TileView */
    std::shared_ptr<scene2::PolygonNode> _node;
    
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
     * Creates the model state and view.
     *
     * In addition to the passed arguments, the node's anchor is set
     * to the bottom left and it should not be tinted relative to the
     * parent's color.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    TileModel(Vec2 position, Size size, Color4 color): color(_color) {
        // TODO: Implement me
        this->_node = scene2::PolygonNode::alloc();
        _node->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _node->setRelativeColor(false);
        setColor(color);
        setPosition(position);
        setSize(size);
         
    }
    
    /**
     * Deletes this TileModel
     *
     * Upon destruction, you should remove the node from its parent.
     */
    ~TileModel() {
        // TODO: Implement me
        auto scene = this->getNode()->getParent();
        if(scene != nullptr){
            scene->removeChild(this->getNode());
        }
        
        
    }
    
#pragma mark Getters
public:
    /** Returns the TileView */
    const std::shared_ptr<scene2::PolygonNode> getNode() const {
        // TODO: Implement me
        return this->_node;
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
        this->_position = position;
        this->_node->setPosition(position);
    }
    
    /**
     * Sets the size of the tile.
     *
     * This modifies the model state and view node. As the view is a
     * PolygonNode, this means setting a new polygon that is a
     * rectangle whose size is the one given. The rectangle however,
     * is only the shape, so it can have a zero origin.
     *
     * @param size Width and height of a single tile
     */
    void setSize(Size size) {
        // TODO: Implement me
        this->_size = size;
        this->_node->setContentSize(size);
    }
    
    /**
     * Sets the color of the tile.
     *
     * This modifies the model state and node.
     *
     * @param color The color of the tile
     */
    void setColor(Color4 color) {
        // TODO: Implement me
        // std::cout<<_color.toString()<<std::endl;
        // std::cout<<color.toString()<<std::endl<<std::endl;
        _color = color;
        _node->setColor(color);
    }
     
};

}

#endif /* __CMV_TILE_MODEL_H__ */
