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
    /**guards ID**/
    int _id;
    /** patrol locations for the guard to follow*/
    vector<Vec2> _patrol_stops;
    //The current stop of the guard (index in _patrol_stops)
    int _current_stop;
    
    bool _doesPatrol;
    
    std::shared_ptr<cugl::scene2::MoveTo> _moveTo;
    
    
#pragma mark Main Methods
public:
    /**view only version of ID**/
    const int& id;
    /**view only version of ID**/
    const bool& doesPatrol;
    
    
    
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    //static guard
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, int id)
    : id(_id), doesPatrol(_doesPatrol)
    {
        _doesPatrol = false;
        _id = id;
        _model = std::make_unique<GuardModel>(position, Size(100, 100), Color4::RED);
        _view = std::make_unique<GuardView>(position,Size(100, 100), Color4::RED, assets, actions);
    }
    
    //moving guard
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets, std::vector<Vec2> patrol_stops, std::shared_ptr<cugl::scene2::ActionManager> actions, int id) : id(_id), doesPatrol(_doesPatrol)
    {
        _patrol_stops = patrol_stops;
        _moveTo = cugl::scene2::MoveTo::alloc();
        _doesPatrol = true;
        _current_stop = 0;
        _id = id;
        _model = std::make_unique<GuardModel>(position, Size(100, 100), Color4::RED);
        _view = std::make_unique<GuardView>(position, Size(100, 100), Color4::RED, assets, actions);
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
    //moves the guard to the next patrol stop
    void nextStop(string actionName){
        _moveTo->setDuration(2);
        if (_current_stop + 1 < _patrol_stops.size()){
            _current_stop += 1;
            _moveTo->setTarget(_patrol_stops[_current_stop]);
            _view->patrol(actionName, _moveTo);
        }
        else{
            _current_stop = 0;
            _moveTo->setTarget(_patrol_stops[_current_stop]);
            _view->patrol(actionName, _moveTo);
        }
    }
    

};

#endif /* __GUARD_CONTROLLER_H__ */
