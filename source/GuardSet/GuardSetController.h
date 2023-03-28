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


#include <cmath>

#define DURATION 3.0f

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
    
    std::shared_ptr<TilemapController> _pastWorld;
    
    std::shared_ptr<TilemapController> _presentWorld;




#pragma mark Main Methods
public:
    
    GuardSetController(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, std::shared_ptr<TilemapController> pastWorld,
                std::shared_ptr<TilemapController> presentWorld)
    {
        _pastWorld = pastWorld;
        _presentWorld = presentWorld;
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
    
    void patrol(Vec2 _charPos, bool isPast, Scene s, float char_angle){
        
        for (int i = 0; i < _guardSet.size(); i++){
            
            string chaseAction = "chasing" + std::to_string(_guardSet[i]->id);
            string patrolAction = "patrol" + std::to_string(_guardSet[i]->id);
            string returnAction = "return" + std::to_string(_guardSet[i]->id);



            Vec2 guardPos = _guardSet[i]->getNodePosition();
            float distance = guardPos.distance(_charPos);
            
            bool detection = false;
            if (distance < 200){
                if (isPast){
                    drawLines(s, _charPos, guardPos);
                    detection = !_presentWorld->lineInObstacle(guardPos,_charPos);
                }else{
                    detection = !_presentWorld->lineInObstacle(guardPos,_charPos);
                }
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
                _actions->remove(patrolAction + "Animation");
                _guardSet[i]->updatePosition(pos);
            }
            else if (detection){
                // std::cout<<"guard angle: "<< _guardSet[i]->getDirection()<<"\n";
                std::cout<<"char angle: "<<char_angle<<"\n";
                Vec2 target = guardPos + ((_charPos - guardPos)/distance)*20;
                Vec2 pos = _guardSet[i]->getNodePosition();
                int direction = calculateMappedAngle(float (pos.x), float (pos.y), float (target.x), float (target.y));
                //chase

                _guardSet[i]->updateChaseTarget(target);

                _guardSet[i]->chaseChar(chaseAction, direction);
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
        bool detection = _pastWorld->lineInObstacle(a, b);
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
        CULog("%f", angleDegrees);
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


    }




};


#endif /* __GUARDSET_CONTROLLER_H__ */
