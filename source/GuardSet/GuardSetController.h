//
//  GuardSetController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//
#ifndef __GUARDSET_CONTROLLER_H__
#define __GUARDSET_CONTROLLER_H__
#include "Guard/GuardModel.h"
#include "Guard/GuardView.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class GuardSetController {
    
#pragma mark External References
public:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<GuardController> Guard;
//    Guard _guard;
    std::vector<Guard> _guardSet;
    
    typedef std::shared_ptr<cugl::Scene2> Scene;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;
    
    /**vector of guard IDs**/
    vector<int> _usedIDs;


#pragma mark Main Methods
public:
    
    GuardSetController(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions)
    {
        _actions = actions;
        std::vector<Guard> _guardSet;
    };
    
#pragma mark Update Methods
public:
    // add one guard
    void add_this_moving(Vec2 gPos, Scene s, const std::shared_ptr<cugl::AssetManager>& assets, vector<Vec2> patrol_stops){
        Guard _guard = std::make_unique<GuardController>(gPos, assets, patrol_stops, _actions, generateUniqueID());
        _guard->addChildTo(s);
        _guardSet.push_back(std::move(_guard));
    }
    
    void add_this(Vec2 gPos, Scene s, const std::shared_ptr<cugl::AssetManager>& assets){
        Guard _guard = std::make_unique<GuardController>(gPos, assets, _actions, generateUniqueID());
        _guard->addChildTo(s);
        _guardSet.push_back(std::move(_guard));
    }
    
    void addChildTo (Scene s) {
        unsigned int vecSize = _guardSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _guardSet[i]->addChildTo(s);
        }
    }
    
    void removeChildFrom (Scene s) {
        unsigned int vecSize = _guardSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _guardSet[i]->removeChildFrom(s);
        }
    }
    
    void clearSet () {
        _guardSet.clear();
    }
    
//    void addChildTo(Scene2 s) {
//        unsigned int vecSize = _guardSet.size();
//        // run for loop from 0 to vecSize
//        for(unsigned int i = 0; i < vecSize; i++)
//        {
//            _guardSet[i];
//        }
//    }
        
    int generateUniqueID() {
        int id = 0;
        bool isUsed = true;
        while (isUsed){
            id = rand() % 900 + 100;
            if (std::find(_usedIDs.begin(), _usedIDs.end(), id) != _usedIDs.end()) {
                    id = rand() % 900 + 100;
                }
                else {
                    _usedIDs.push_back(id);
                    isUsed = false;
                }
        }

        return id;
    }
    
    void patrol(Vec2 _charPos){
        for (int i = 0; i < _guardSet.size(); i++){
            string chaseAction = "chasing" + std::to_string(_guardSet[i]->id);
            string patrolAction = "patrol" + std::to_string(_guardSet[i]->id);
            string returnAction = "return" + std::to_string(_guardSet[i]->id);

            Vec2 guardPos = _guardSet[i]->getNodePosition();
            float distance = guardPos.distance(_charPos);
            
            if (_actions->isActive(chaseAction) or _actions->isActive(returnAction)){
                //wait for guard to finish current action
            }
            //detection
            else if (_guardSet[i]->detection(_charPos)   and _actions->isActive(patrolAction)){
                Vec2 pos = _guardSet[i]->getNodePosition();
                _guardSet[i]->saveCurrentStop();
                _actions->remove(patrolAction);
                _guardSet[i]->updatePosition(pos);
            }
            else if (_guardSet[i]->detection(_charPos)){
                Vec2 target = guardPos + ((_charPos - guardPos)/distance)*8;
                //chase
                _guardSet[i]->updateChaseTarget(target);
                _guardSet[i]->chaseChar(chaseAction);
                //add to return vec
                _guardSet[i]->prependReturnVec(target);
            }
            else if (_guardSet[i]->returnVec.size() != 0){
                //return action
                _guardSet[i]->updateReturnTarget(_guardSet[i]->returnVec[0]);
                _guardSet[i]->returnGuard(returnAction);
                //erase from return vector
                _guardSet[i]->eraseReturnVec();
            }
            //no detection and no return vec
            else if (_guardSet[i]->doesPatrol){
                if (_actions->isActive(patrolAction)){
                    //wait for guard to finish patrol
                }else{
                    _guardSet[i]->nextStop(patrolAction);
                }
            }
        }
    }
    

};


#endif /* __GUARDSET_CONTROLLER_H__ */
