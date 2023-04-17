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
// #define DURATION 1.0f

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
    int _goingTo = 0;
    
    bool _doesPatrol;
    
    std::shared_ptr<cugl::scene2::MoveTo> _patrolMove;
    std::shared_ptr<cugl::scene2::MoveTo> _chaseMove;
    std::shared_ptr<cugl::scene2::MoveTo> _returnMove;

    /**vector for the guard to use to return to his post*/
    vector<Vec2> _returnVec;

    vector<Vec2> _chaseVec;
    
    //whether guard is traversing back the path
    bool going_back = false;
    //whether or not guard returned from chasing
    bool returned;
    //saved stop to use when returning
    int saved_stop;
    //current state
    string _state;
    //prev state
    string _prev_state;
    // if the guard is in question state
    bool _is_question;
    // fixed direction for static guard
    int _fixed_direction;

    string _state_before_question;

    bool _if_question_inSP;

    Vec2 _static_pos;


    
#pragma mark Main Methods
public:
    /**view only version of ID**/
    const int& id;
    /**view only version of ID**/
    const bool& doesPatrol;
    /**view only version of return vec**/
    const vector<Vec2>& returnVec;

    const vector<Vec2>& chaseVec;

    /**view only version of state**/
    const string& state;

    /**view only version of prev state**/
    const string& prev_state;
    
    
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    //static guard
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, int id, bool isPast)
    : id(_id), doesPatrol(_doesPatrol), returnVec(_returnVec), chaseVec(_chaseVec),state(_state), prev_state(_prev_state)
    {
        _state = "static";
        _prev_state = "static";
        _returnVec = {};
        _chaseVec = {};
        _chaseMove = cugl::scene2::MoveTo::alloc();
        _chaseMove->setDuration(DURATION);
        
        _returnMove = cugl::scene2::MoveTo::alloc();
        _returnMove->setDuration(DURATION);
        _is_question = false;
        
        _doesPatrol = false;
        _id = id;

        _fixed_direction = 0;
        _if_question_inSP = false;
        _static_pos = position;

        _model = std::make_unique<GuardModel>(position, Size(100, 100), Color4::RED, _fixed_direction);
        _view = std::make_unique<GuardView>(position,Size(100, 100), Color4::RED, assets, actions, isPast);
    }
    
    //moving guard
    GuardController(Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets, std::vector<Vec2> vec, std::shared_ptr<cugl::scene2::ActionManager> actions, int id, bool isPast) : id(_id), doesPatrol(_doesPatrol), returnVec(_returnVec), chaseVec(_chaseVec), state(_state), prev_state(_prev_state)
    {
        _state = "patrol";
        _prev_state = "patrol";
        _goingTo = 0;
        _returnVec = {};
        _chaseVec = {};
        _patrol_stops = vec;

        _chaseMove = cugl::scene2::MoveTo::alloc();
        _chaseMove->setDuration(DURATION);
        
        _returnMove = cugl::scene2::MoveTo::alloc();
        _returnMove->setDuration(DURATION);
        
        _patrolMove = cugl::scene2::MoveTo::alloc();
        _doesPatrol = true;
        _is_question = false;
        _if_question_inSP = false;

        _static_pos = position;

        // just a placeholder
        _fixed_direction = 0;

        _id = id;
        // starting direction should from level editor
        _model = std::make_unique<GuardModel>(position, Size(100, 100), Color4::RED, 0);
        _view = std::make_unique<GuardView>(position, Size(100, 100), Color4::RED, assets, actions, isPast);
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

    Vec2 getStaticPosition() {
        return _static_pos;
    }

    void setStaticPosition(Vec2 value) {
        _static_pos = value;
    }

    void setStateBeforeQuestion(string s) {
        _state_before_question = s;
    }

    string getStateBeforeQuestion() {
        return _state_before_question;
    }

    bool getIfQuestionInSP(){
        return _if_question_inSP;
    }

    void setIfQuestionInSP(bool value) {
        _if_question_inSP = value;
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

    void updateChaseSPTarget(Vec2 pos){
        _chaseMove->setTarget(pos);
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
        if (returned){
            _goingTo = saved_stop;
            returned = false;
        }
        else{
            if (_goingTo + 1 == _patrol_stops.size()){
                going_back = true;
            }
            else if (going_back and (_goingTo == 0)){
                going_back = false;
            }
            
            if(going_back){
                _goingTo -= 1;
            }
            else{
                _goingTo += 1;
            }
        }
        
        float speed = 53;
        float distance = getNodePosition().distance(_patrol_stops[_goingTo]);
        float duration = distance / speed;
        
        //move guard
        _patrolMove->setDuration(duration);
        _patrolMove->setTarget(_patrol_stops[_goingTo]);
        _view->performAction(actionName, _patrolMove);


        //animate guard

    }

    void updateAnimation(Vec2 target, string state, int last_direction, string last_state, bool valid_target, string id) {

        int direction;
        if (!valid_target and state == "static") {
            direction = _fixed_direction;
        }
        else if (!valid_target and state == "question") {
            direction = last_direction;
        }
        else {
            Vec2 pos = _view->nodePos();
            direction = calculateMappedAngle(pos.x, pos.y, target.x, target.y);
        }
        _view->performAnimation(direction, state, last_direction, last_state, id);
        _model->setDirection(direction);


    }

    void stopQuestionAnim(string id){
        _view->stopQuestionAnim(id);
    }

    void lookAroundAnim(string id) {
        updateAnimation(Vec2(0,0), _state, _model->getDirection(), _prev_state, false, id);
    }
    void questionAnim(string id) {
        updateAnimation(Vec2(0,0), _state, _model->getDirection(), _prev_state, false,id);
        // question animation
        _view->startQuestionAnim(id);
    }

    void staticGuardAnim(string id) {
        updateAnimation(Vec2(0,0), _state, _model->getDirection(), _prev_state, false, id);
    }

    void chaseGuardAnim(string id) {
        updateAnimation(_chaseMove->getTarget(), _state, _model->getDirection(), _prev_state, true, id);
    }

    void patrolGuardAnim(string id) {
        updateAnimation(_patrolMove->getTarget(), _state, _model->getDirection(), _prev_state, true, id);
    }

    void returnGuardAnim(string id) {
        updateAnimation(_returnMove->getTarget(), _state, _model->getDirection(), _prev_state, true, id);
    }

    void chaseChar(string actionName){
        // CULog("chasing");
        _view->performAction(actionName, _chaseMove);
    }
    
    void returnGuard(string actionName){
        // CULog("returning");
        _view->performAction(actionName, _returnMove);
    }
    
    void prependReturnVec(Vec2 pos){
        _returnVec.insert(_returnVec.begin(), pos);
    }
    
    void setReturnVec(vector<Vec2> returnVec){
        _returnVec = returnVec;
    }

    void setChaseVec(vector<Vec2> chaseVec){
        _chaseVec = chaseVec;
    }

    void eraseChaseSPVec(){
        _chaseVec.erase(_chaseVec.begin());
    }
    void eraseReturnVec(){
        _returnVec.erase(_returnVec.begin());
    }
    
    void saveCurrentStop(){
        saved_stop = _goingTo;
        returned = true;
    }
    

    int getDirection() {
        return _model->getDirection();
    }

    
    void setVisibility(bool visible){
        _view->setVisibility(visible);
    }
    
    void updateState(string state){
        _state = state;
    }
    
    void updatePrevState(string prev_state){
        _prev_state = prev_state;
    }
    
    Vec2 getSavedStop(){
        return _patrol_stops[saved_stop];
    }


//    int getIsQuestion() {
//        return _is_question;
//    }
//
//    void setIsQuestion(bool value) {
//        _is_question = value;
//    }



    int calculateMappedAngle(float x1, float y1, float x2, float y2)
    {
        // calculate the angle in radians
        double angleRadians = atan2(y2 - y1, x2 - x1);

        // convert the angle to degrees
        float angleDegrees = angleRadians * 180.0 / M_PI;

        // make sure the angle is between 0 and 360 degrees
        if (angleDegrees < 0.0)
        {
            angleDegrees += 360.0;
        }

        // map the angle from 0 to 360 degrees to 0 to 7
        //CULog("%f", angleDegrees);
        if (angleDegrees > 337.5 || angleDegrees < 22.5) {
            return 2;
        } else if (angleDegrees >= 22.5 && angleDegrees < 67.5){
            return 1;
        }
        else if (angleDegrees >= 67.5 && angleDegrees < 112.5){
            return 0;
        }
        else if (angleDegrees >= 112.5 && angleDegrees < 157.5){
            return 7;
        }
        else if (angleDegrees >= 157.5 && angleDegrees < 202.5){
            return 6;
        }
        else if (angleDegrees >= 202.5 && angleDegrees < 247.5){
            return 5;
        }
        else if (angleDegrees >= 247.5 && angleDegrees < 292.5){
            return 4;
        }else if (angleDegrees >= 292.5 && angleDegrees < 337.5){
            return 3;
        }

        return 0;
    }

};

#endif /* __GUARD_CONTROLLER_H__ */
