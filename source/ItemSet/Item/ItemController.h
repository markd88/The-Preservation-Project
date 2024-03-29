//
//  ItemController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef __ITEM_CONTROLLER_H__
#define __ITEM_CONTROLLER_H__

#include "ItemModel.h"
#include "ItemView.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class ItemController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<ItemModel> _model;
    /** View reference */
    std::unique_ptr<ItemView> _view;

    bool can_be_collected;

private: int _id;

    
#pragma mark Main Methods
public:

    const int& id;


    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    ItemController(Vec2 position, Size size, bool isArtifact, bool isResource, bool isObs, bool isExit,
        const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey, int id) : id(_id) {
        _model = std::make_unique<ItemModel>(position, size, isArtifact, isResource, isObs, isExit, textureKey);
        _view = std::make_unique<ItemView>(position, size, isArtifact, isResource, isObs, isExit, assets, textureKey, id);
        _id = id;
        if (isResource || isArtifact) {
            can_be_collected = true;
        } else {
            can_be_collected = false;
        }
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

    bool Iscollectable() {
        return can_be_collected;
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
    
    Vec2 getNodePosition(){
        return _view->nodePos();
    }
    
    bool isArtifact(){
        return _model->isArtifact();
    }
    
    bool isResource(){
        return _model->isResource();
    }
    
    bool isObs(){
        return _model->isObs();
    }
    
    bool isExit() {
        return _model->isExit();
    }
    
    std::string getTextureKey() {
        return _model->getTextureKey();
    }
    
    /**
     *  Detect if this file contains a point
     *
     *  @param point, the position of the point
     */
    bool contains(Vec2 point){
        return _view->contains(point);
    }

    
#pragma mark Scene Methods
public:
    /**
     * Adds the view as a child to the given `node`.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<cugl::scene2::OrderedNode>& node) {
        _view->addChildTo(node);
    }
    
    /**
     * Removes the view child from the given `node`.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<cugl::scene2::OrderedNode>& node) {
        _view->removeChildFrom(node);
    }
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {
        _view->setTexture(assets, textureKey);
    }
    
    void setVisibility(bool visible){
        _view->setVisibility(visible);
    }
    
    bool containsLine(Vec2 a, Vec2 b){
        return _view->containsLine(a, b);
    }
    
    
    void updatePriority(){
        return _view->updatePriority();
    }

    void setAction(std::shared_ptr<cugl::scene2::ActionManager> actions) {
        _view->setAction(actions);
    }

    void updateAnim() {
        _view->updateAnim();
    }

    void removeAnim() {
        _view->removeAnim();
        can_be_collected = false;
    }
    
    
    void updateTransparency(){
        this->_view->updateTransparency();
    }


    
#pragma mark Controller Methods
public:
};

#endif /* __ITEM_CONTROLLER_H__ */
