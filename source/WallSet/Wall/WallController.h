//
//  WallController.h
//  Tilemap
//
//  Created by Hao Chen on 4/15/23.
//

#ifndef WallController_h
#define WallController_h

#include "WallModel.h"
#include "WallView.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class WallController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<WallModel> _model;
    /** View reference */
    std::unique_ptr<WallView> _view;

    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    WallController(Vec2 position, float rot, Size size, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        _model = std::make_unique<WallModel>(position, size, isResource, textureKey);
        _view = std::make_unique<WallView>(position, rot, size, isResource, assets, textureKey);
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
        _view->setColor(color);
    }
    
    
    Vec2 getNodePosition(){
        return _view->nodePos();
    }
    
    bool isResource(){
        return _model->isResource();
    }
    
    std::string getTextureKey() {
        return _model->getTextureKey();
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
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        _view->setTexture(assets, textureKey);
    }
    
    void setVisibility(bool visible){
        _view->setVisibility(visible);
    }
    
#pragma mark Controller Methods
public:
};

#endif /* WallController_h */
