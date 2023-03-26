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
    
    std::shared_ptr<cugl::scene2::MoveTo> _patrolMove;
    std::shared_ptr<cugl::scene2::MoveTo> _chaseMove;
    std::shared_ptr<cugl::scene2::MoveTo> _returnMove;

    /**vector for the guard to use to return to his post*/
    vector<Vec2> _returnVec;
        
    
#pragma mark Main Methods
public:
    /**view only version of ID**/
    const int& id;
    /**view only version of ID**/
    const bool& doesPatrol;
    /**view only version of return vec**/
    const vector<Vec2>& returnVec;
    
    
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    //static guard
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, int id)
    : id(_id), doesPatrol(_doesPatrol), returnVec(_returnVec)
    {
        
        _returnVec = {};
        _chaseMove = cugl::scene2::MoveTo::alloc();
        _chaseMove->setDuration(.15);
        
        _returnMove = cugl::scene2::MoveTo::alloc();
        _returnMove->setDuration(.15);
        
        _doesPatrol = false;
        _id = id;
        _model = std::make_unique<GuardModel>(position, Size(100, 100), Color4::RED);
        _view = std::make_unique<GuardView>(position,Size(100, 100), Color4::RED, assets, actions);
    }
    
    //moving guard
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets, std::vector<Vec2> vec, std::shared_ptr<cugl::scene2::ActionManager> actions, int id) : id(_id), doesPatrol(_doesPatrol), returnVec(_returnVec)
    {
        _returnVec = {};
        std::vector<Vec2> temp = vec;
        temp.erase(temp.begin());
        std::reverse(temp.begin(), temp.end());
        std::cout<<temp.size()<<"\n";
        temp.erase(temp.begin());
        std::cout<<temp.size()<<"\n";
        vec.insert(vec.end(), temp.begin(), temp.end());
        
        _patrol_stops = vec;
        std::cout<<vec.size()<<"\n";

        _chaseMove = cugl::scene2::MoveTo::alloc();
        _chaseMove->setDuration(.15);
        
        _returnMove = cugl::scene2::MoveTo::alloc();
        _returnMove->setDuration(.15);
        
        _patrolMove = cugl::scene2::MoveTo::alloc();
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

#pragma mark Update Chase Methods
    
    void updateChaseTarget(Vec2 pos){
        _chaseMove->setTarget(pos);
    }
    
    void updateChaseSpeed(float speed){
        _chaseMove->setDuration(speed);

    }
#pragma mark Update Patrol Methods
    
    void updatePatrolSpeed(float speed){
        _patrolMove->setDuration(speed);
    }
    
#pragma mark Update Return Methods
    
    void updateReturnSpeed(float speed){
        _returnMove->setDuration(speed);
    }
    
    void updateReturnTarget(Vec2 pos){
        _returnMove->setTarget(pos);
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
    //moves the guard to the next patrol stop
    void nextStop(string actionName){
        if (_current_stop + 1 < _patrol_stops.size()){
            _current_stop += 1;
        }
        else{
            _current_stop = 0;
        }
        float speed = 53.3;
        int duration = getNodePosition().distance(_patrol_stops[_current_stop]) / speed;
        _patrolMove->setDuration(duration);
        _patrolMove->setTarget(_patrol_stops[_current_stop]);
        _view->performAction(actionName, _patrolMove);
    }
    
    void chaseChar(string actionName){
        _view->performAction(actionName, _chaseMove);
    }
    
    void returnGuard(string actionName){
        _view->performAction(actionName, _returnMove);
    }
    
    void prependReturnVec(Vec2 pos){
        _returnVec.insert(_returnVec.begin(), pos);
    }
    
    void eraseReturnVec(){
        _returnVec.erase(_returnVec.begin());
    }
    
    void updateCurrentStop(int stop){
        if (_current_stop == 0){
            _current_stop = 1;
        }
        else{
            _current_stop = _current_stop + stop;
        }
    }
    
};

#endif /* __GUARD_CONTROLLER_H__ */
