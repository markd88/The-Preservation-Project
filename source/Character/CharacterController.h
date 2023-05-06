//
//  CharacterController.h
//
#ifndef __CHARACTER_CONTROLLER_H__
#define __CHARACTER_CONTROLLER_H__

// These do not need to be in angle brackets
#include "CharacterModel.h"
#include "CharacterView.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class CharacterController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<CharacterModel> _model;
    /** View reference */
    std::unique_ptr<CharacterView> _view;
    
    
    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    CharacterController(Vec2 position, Size size, Color4 color, std::shared_ptr<cugl::scene2::ActionManager> actions, const std::shared_ptr<cugl::AssetManager>& assets) {
        _view = std::make_unique<CharacterView>(position, size, color, actions, assets);
        _model = std::make_unique<CharacterModel>(_view->nodePos(), size, color);
    }
    
    CharacterController(Vec2 position, std::shared_ptr<cugl::scene2::ActionManager> actions, const std::shared_ptr<cugl::AssetManager>& assets) {
        _view = std::make_unique<CharacterView>(position, Size(20, 20), Color4::BLUE, actions, assets);
        _model = std::make_unique<CharacterModel>(_view->nodePos(), Size(20, 20), Color4::BLUE);
    }

#pragma mark Update Methods
public:
    
    Vec2 getPosition() {
        return _model->getPosition();
    }
    
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
     *  Animates the character with a moveTo action
     *
     *  @param action The action to perfrom
     */
    void moveTo(const std::shared_ptr<cugl::scene2::MoveTo>& action) {
        _model->setPosition(action->getTarget());
        _view->moveTo(action);
    }

    void updateAnimation(Vec2 pos) {
        _view->updateAnimation(pos);
    }

    void stopAnimation() {
        _view->stopAnimation();
    }

    void updateLastDirection(Vec2 pos){
        _view->updateLastDirection(pos);
    }

    Vec2 getNodePosition(){
        return _view->nodePos();
    }

    int getNumRes(){
        return _model->getNumRes();
    }
    
    void addRes(){
        _model->setNumRes(getNumRes() + 1);
    }
    
    bool useRes(){
        if(getNumRes() == 0) return false;
        _model->setNumRes(getNumRes() - 1);
        return true;
    }
    
    int getNumArt(){
        return _model->getNumArt();
    }
    
    void addArt(){
        _model->setNumArt(getNumArt() + 1);
    }
    
#pragma mark Helpers
    /**
     *  See if the touch point is within the character
     *
     *  @param point The position of the touchpoint
     */
    bool containsFar(Vec2 point){
        return _model->containsFar(point);
    }
    
    // for other purposes
    bool containsNear(Vec2 point){
        return _model->containsNear(point);
    }
    
    bool containsExit(Vec2 point){
        return _model->containsExit(point);
    }
#pragma mark Scene Methods
public:
    /**
     * Adds the view as a child to the given `node`.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<cugl::scene2::OrderedNode> node) {
        _view->addChildTo(node);
    }
    
    /**
     * Removes the view child from the given `node`.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<cugl::scene2::OrderedNode> node) {
        _view->removeChildFrom(node);
    }
    
    float getAngle(){
        return _view->getAngle();
    }
    
    void updatePriority(){
        _view->updatePriority();
    }
    
#pragma mark Controller Methods
public:

};

#endif /* __CHARACTER_CONTROLLER_H__ */

