//
//  GuardController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef __GUARD_CONTROLLER_H__
#define __GUARD_CONTROLLER_H__

#include "GuardView.h"
#include "GuardModel.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class GuardController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<GuardModel> _model;
    /** View reference */
    std::unique_ptr<GuardView> _view;

    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    GuardController(Vec2 position, Size size, Color4 color, const std::shared_ptr<cugl::AssetManager>& assets) {
        _model = std::make_unique<GuardModel>(position, size, color);
        _view = std::make_unique<GuardView>(position, size, color, assets);
    }
    
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets) {
        _model = std::make_unique<GuardModel>(position, Size(100, 100), Color4::RED);
        _view = std::make_unique<GuardView>(position, Size(100, 100), Color4::RED, assets);
    }

#pragma mark Update Methods
public:
    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updatePosition(Vec2 position) {
        _model->setPosition(position);
        _view->setPosition(position);
    }

    /**
     *  Updates the model and view with the size of this tile.
     *
     *  @param size  The bottom left corner of the tile
     */
    void updateSize(Size size) {
        _model->setSize(size);
        _view->setSize(size);
    }
    
    /**
     *  Updates the model and view with the color of this tile.
     *
     *  @param color The tile color
     */
    void updateColor(Color4 color) {
        _model->setColor(color);
        _view->setColor(color);
    }
    
    
    Vec2 getNodePosition(){
        return _view->nodePos();
    }

//#pragma mark Helpers
//    /**
//     *  See if the touch point is within the character
//     *
//     *  @param point The position of the touchpoint
//     */
//    bool contains(Vec2 point){
//        return _model->contains(point);
//    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view as a child to the given `node`.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<cugl::Scene2> node) {
        _view->addChildTo(node);
    }
    
    /**
     * Removes the view child from the given `node`.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<cugl::Scene2> node) {
        _view->removeChildFrom(node);
    }
    
#pragma mark Controller Methods
public:
    
    

};

#endif /* __GUARD_CONTROLLER_H__ */
