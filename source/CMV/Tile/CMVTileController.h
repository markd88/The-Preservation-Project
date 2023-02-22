//
//  CMVTileController.h
//  TileMap Lab
//
//  This module provides the CMV version of the TileController class.
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __CMV_TILE_CONTROLLER_H__
#define __CMV_TILE_CONTROLLER_H__
#include "CMVTileModel.h"

namespace CMV {
/**
 * A class that updates the model. It only controls a single tile.
 */
class TileController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<TileModel> _model;
    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    TileController(Vec2 position, Size size, Color4 color) {
        // TODO: Implement me
        // std::cout<<"init the model"<<std::endl;
        this->_model = std::make_unique<TileModel>(position, size, color);
    }

    
#pragma mark Update Methods
public:
    /**
     * Updates the position of this tile by updating the model.
     *
     * @param position  The bottom left corner of the tile
     */
    void updatePosition(Vec2 position) {
        // TODO: Implement me
        this->_model->setPosition(position);
    }
    
    /**
     * Updates the size of this tile by updating the model.
     *
     * @param size  The bottom left corner of the tile
     */
    void updateSize(Size size) {
        // TODO: Implement me
        this->_model->setSize(size);
    }
    
    /**
     * Updates the color of this tile by updating the model.
     *
     * @param color The tile color
     */
    void updateColor(Color4 color) {
        // TODO: Implement me
        _model->setColor(color);
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the TileView as a child to the given `node`.
     *
     * The TileView is the view reference held within the model.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<scene2::SceneNode> node) {
        // TODO: Implement me
        node->addChild(_model->getNode());
    }
    
    /**
     * Removes the TileView child from the given node.
     *
     * The TileView is the view reference held within the model.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<scene2::SceneNode> node) {
        // TODO: Implement me
        node->removeChild(_model->getNode());
    }
    
#pragma mark Controller Methods
public:
    /**
     * Inverts the color of this tile.
     *
     * Examples:
     *      Inverting white (255, 255, 255, 0) gives black (0, 0, 0, 0)
     *      Inverting red (255, 0, 0, 0) gives cyan (0, 255, 255, 0)
     *      Inverting light purple (150, 100, 200, 0) gives a dull green
     *          (105, 155, 55, 0)
     */
    void invertColor() {
        // TODO: Implement me
        Color4 old_color = _model->color;
        Color4 new_color = old_color.complement();
        this->updateColor(new_color);
    }
    
    
};

}

#endif /* __CMV_TILE_CONTROLLER_H__ */
