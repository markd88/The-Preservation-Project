//
//  MVCTileController.h
//  TileMap Lab
//
//  This module provides the MVC version of the TileController class.
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __MVC_TILE_CONTROLLER_H__
#define __MVC_TILE_CONTROLLER_H__

// These do not need to be in angle brackets
#include "MVCTileModel.h"
#include "MVCTileView.h"

namespace MVC {
/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class TileController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<TileModel> _model;
    /** View reference */
    std::unique_ptr<TileView> _view;
    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    TileController(Vec2 position, Size size, Color4 color) {
        // TODO: Implement me
        this->_model = std::make_unique<TileModel>(position, size, color);
        _view = std::make_unique<TileView>(position, size, color);
    }
    
#pragma mark Update Methods
public:
    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updatePosition(Vec2 position) {
        // TODO: Implement me
        this->_model->setPosition(position);
        _view->setPosition(position);
    }
    
    /**
     *  Updates the model and view with the size of this tile.
     *
     *  @param size  The bottom left corner of the tile
     */
    void updateSize(Size size) {
        // TODO: Implement me
        this->_model->setSize(size);
        this->_view->setSize(size);
    }
    
    /**
     *  Updates the model and view with the color of this tile.
     *
     *  @param color The tile color
     */
    void updateColor(Color4 color) {
        // TODO: Implement me
        _model->setColor(color);
        _view->setColor(color);
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view as a child to the given `node`.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<scene2::SceneNode> node) {
        // TODO: Implement me
        _view->addChildTo(node);
    }
    
    /**
     * Removes the view child from the given `node`.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<scene2::SceneNode> node) {
        // TODO: Implement me
        _view->removeChildFrom(node);
    }
    
#pragma mark Controller Methods
public:
    /**
     *  Inverts the color of this tile.
     *
     *  Examples:
     *      Inverting white (255, 255, 255, 0) gives black (0, 0, 0, 0)
     *      Inverting red (255, 0, 0, 0) gives cyan (0, 255, 255, 0)
     *      Inverting light purple (150, 100, 200, 0) gives a dull green
     *          (105, 155, 55, 0)
     */
    void invertColor() {
        // TODO: Implement me
        Color4 old_color = _model->color;
        Color4 new_color = old_color.complement();
        updateColor(new_color);
    }

};
}

#endif /* __MVC_TILE_CONTROLLER_H__ */
