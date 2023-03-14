//
//  artifactController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef __ARTIFACT_CONTROLLER_H__
#define __ARTIFACT_CONTROLLER_H__

#include "ArtifactModel.h"
#include "ArtifactView.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class ArtifactController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<ArtifactModel> _model;
    /** View reference */
    std::unique_ptr<ArtifactView> _view;

    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    ArtifactController(Vec2 position, Size size, Color4 color, bool isResource = false) {
    ArtifactController(Vec2 position, Size size, Color4 color, bool isResource = false) {
        _model = std::make_unique<ArtifactModel>(position, size, color);
        _view = std::make_unique<ArtifactView>(position, size, color);
    }
    
    ArtifactController(Vec2 position, bool isResource = false) {
        _model = std::make_unique<ArtifactModel>(position, Size(50, 50), Color4::RED);
        _view = std::make_unique<ArtifactView>(position, Size(50, 50), Color4::RED);
    }
//    ArtifactController(Vec2 position,  bool isResource = true) {
//        _model = std::make_unique<ArtifactModel>(position, Size(15, 15), Color4::CYAN);
//        _view = std::make_unique<ArtifactView>(position, Size(15, 15), Color4::CYAN);
//    }

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

#endif /* __ARTIFACT_CONTROLLER_H__ */
