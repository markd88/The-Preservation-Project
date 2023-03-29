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
#include "Guard/GuardController.h"
#include <Tilemap/TilemapController.h>

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
    
    std::shared_ptr<TilemapController> _world;
    

#pragma mark Main Methods
public:
    
    GuardSetController(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, std::shared_ptr<TilemapController> world)
    {
        _world = world;
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
    
    void setVisbility(bool visible){
        for (int i = 0; i < _guardSet.size(); i++){
            _guardSet[i]->setVisibility(visible);
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
    
    void patrol(Vec2 _charPos, Scene s, float char_angle){
        
        for (int i = 0; i < _guardSet.size(); i++){
            
            string chaseAction = "chasing" + std::to_string(_guardSet[i]->id);
            string patrolAction = "patrol" + std::to_string(_guardSet[i]->id);
            string returnAction = "return" + std::to_string(_guardSet[i]->id);

            Vec2 guardPos = _guardSet[i]->getNodePosition();
            float distance = guardPos.distance(_charPos);
            
            bool detection = false;
            if (distance < 200 and _world->isActive()){
                //drawLines(s, _charPos, guardPos);
                detection = !_world->lineInObstacle(guardPos,_charPos);
            }
            
            if (_actions->isActive(chaseAction) or _actions->isActive(returnAction)){
                //wait for guard to finish current action
                //interupt return action if detection
            }
            //detection
            else if (detection and _actions->isActive(patrolAction)){
                Vec2 pos = _guardSet[i]->getNodePosition();
                _guardSet[i]->saveCurrentStop();
                _actions->remove(patrolAction);
                _guardSet[i]->updatePosition(pos);
            }
            
            else if (detection){
                //std::cout<<"guard angle: "<< _guardSet[i]->getAngle()<<"\n";
                //std::cout<<"char angle: "<<char_angle<<"\n";
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
    
    void drawLines(Scene s, Vec2 a, Vec2 b){
        s->removeChildByName("line");
        
        SplinePather splinePather = SplinePather();
        SimpleExtruder extruder = SimpleExtruder();
        bool detection = _world->lineInObstacle(a, b);
        Spline2 spline = Spline2(a, b);
        splinePather.set(&spline);
        splinePather.calculate();

        extruder.set(splinePather.getPath());
        extruder.calculate(1);
        Poly2 line = extruder.getPolygon();
        std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
        polyNode->setPolygon(line);
        if (detection){
            polyNode->setColor(Color4::BLUE);
        }else{
            polyNode->setColor(Color4::GREEN);
        }
        polyNode->setPosition(a.getMidpoint(b));
        
        s->addChildWithName(polyNode, "line");
        
    }
    

};


#endif /* __GUARDSET_CONTROLLER_H__ */
