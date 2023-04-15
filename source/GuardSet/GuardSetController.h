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

using namespace std;
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
    
    std::shared_ptr<TilemapController> _world;
    
    /**adjacency matrix*/
    bool** _adjMatrix;
    
    /** nodes to vec2 positions*/
    std::unordered_map<int, Vec2> _nodes;
    
    



#pragma mark Main Methods
public:
    
    GuardSetController(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, std::shared_ptr<TilemapController> world, bool** adjMatrix, std::unordered_map<int, Vec2> nodes)
    {
        _adjMatrix = adjMatrix;
        _nodes = nodes;
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

        static auto last_time_question = std::chrono::steady_clock::now();
        static auto last_time_lookaround = std::chrono::steady_clock::now();

        // Calculate the time elapsed since the last call to pinch
        auto now = std::chrono::steady_clock::now();
        auto elapsed_question = std::chrono::duration_cast<std::chrono::seconds>(now - last_time_question);
        auto elapsed_lookaround = std::chrono::duration_cast<std::chrono::seconds>(now - last_time_lookaround);

        for (int i = 0; i < _guardSet.size(); i++){

            string id = std::to_string(_guardSet[i]->id);

            string chaseAction = "chase" + id;
            string patrolAction = "patrol" + id;
            string returnAction = "return" + id;


            Vec2 guardPos = _guardSet[i]->getNodePosition();
            float distance = guardPos.distance(_charPos);
            int charDirection = calculateMappedAngle(guardPos.x, guardPos.y, _charPos.x, _charPos.y);
            int guardFacingDirection = _guardSet[i]->getDirection();
            bool insideVisionCone = false;
            if ((guardFacingDirection + 8 -1) %8 == charDirection || (guardFacingDirection + 8 + 1) % 8 == charDirection) {
                insideVisionCone = true;
            }
            bool visual_detection = false;
            if (distance < 100 and _world->isActive() and insideVisionCone){
                visual_detection = !_world->lineInObstacle(guardPos,_charPos);
            }

            bool acoustic_detection = false;
            if (distance < 400 and _world->isActive()) {
                acoustic_detection = true;
            }

#pragma mark Guard State Updates

            // static state
            if (_guardSet[i]->state == "static") {
                if (visual_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chaseD");
                }
                else if (acoustic_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("question");
                    last_time_question = now;
                    _guardSet[i]->setStateBeforeQuestion("static");
                }
                else {
                    // keep static
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
            }

            // question state
            else if (_guardSet[i]->state == "question") {
                if (visual_detection) {
                    // chase immediately
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chase");
                }
                else if (acoustic_detection && elapsed_question.count() <= 1 ) {
                    // keep question
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
                else if (acoustic_detection && elapsed_question.count() > 1) {
                    // chase in shortest path
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);

                    int start = findClosestNode(_guardSet[i]->getNodePosition());
                    int finish = findClosestNode(_charPos);
                    _guardSet[i]->setChaseVec(shortestPath(start, finish));

                    _guardSet[i]->updateState("chaseSP");
                }
                else {
                    // return to the previous state
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState(_guardSet[i]->getStateBeforeQuestion());
                    if (_guardSet[i]->getStateBeforeQuestion() == "question") {
                        last_time_question = now;
                    }
                }
            }

            else if (_guardSet[i]->state == "chaseD") {

                //go to lookaround if no detection
                if (!visual_detection){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("lookaround");
                    last_time_lookaround = now;
                }
                else{
                    //keep chasing otherwise
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
            }

            else if (_guardSet[i]->state == "chaseSP") {
                if (visual_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chaseD");
                }
                else if (acoustic_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("question");
                    last_time_question = now;
                    _guardSet[i]->setStateBeforeQuestion("chaseSP");
                }
                else if (_guardSet[i]->chaseVec.size() == 0 ){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("lookaround");
                }
                else {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }

            }

            else if (_guardSet[i]->state == "lookaround") {
                // maybe wider visual detection
                if (visual_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chaseD");
                }
                else if (acoustic_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("question");
                    last_time_question = now;
                    _guardSet[i]->setStateBeforeQuestion("lookaround");
                }
                else if (elapsed_lookaround.count() <= 1){
                    // keep lookaround
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
                else if (elapsed_lookaround.count() > 1) {
                    int start = findClosestNode(_guardSet[i]->getNodePosition());
                    int finish = findClosestNode(_guardSet[i]->getSavedStop());
                    _guardSet[i]->setReturnVec(shortestPath(start, finish));

                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("return");

                }
            }

            //patrol state
            else if (_guardSet[i]->state == "patrol"){
                
                //detection in patrol state = chase
                if (visual_detection){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chase");
                }
                else if (acoustic_detection){
                    // if did not see, but hear
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("question");
                    last_time_question = now;
                    _guardSet[i]->setStateBeforeQuestion("patrol");
                }
                //keep patroling otherwise
                else{
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
                
            }

            
            else if (_guardSet[i]->state == "return"){
                if (visual_detection){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chase");
                }
                else if (acoustic_detection){
                    // if did not see, but hear
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("question");
                    last_time_question = now;
                    _guardSet[i]->setStateBeforeQuestion("return");
                }
                //state change from return to patrol
                else if (_guardSet[i]->returnVec.size() == 0 and _guardSet[i]->doesPatrol){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("patrol");
                }
                else if (_guardSet[i]->returnVec.size() == 0 and !_guardSet[i]->doesPatrol){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("static");
                }
                //otherwise continue returning
                else{
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
            }


            
#pragma mark Guard action according to state



            if (_guardSet[i]->state == "static") {
                _guardSet[i]->staticGuardAnim();
            }
            else if (_guardSet[i]->state == "question") {
                _guardSet[i]->questionAnim();
            }
            else if (_guardSet[i]->state == "lookaround") {
                _guardSet[i]->lookAroundAnim();
            }
            // return state
            else if (_guardSet[i]->state == "return"){
                _guardSet[i]->updateReturnTarget(_guardSet[i]->returnVec[0]);
                _guardSet[i]->returnGuard(returnAction);
                // erase from return vector
                _guardSet[i]->eraseReturnVec();
                _guardSet[i]->returnGuardAnim();
            }
            //patrol state
            else if (_guardSet[i]->state == "patrol" and _guardSet[i]->doesPatrol){

                if(_actions->isActive(patrolAction)){
                    // guard is moving properly, wait till finished
                }
                else{
                    // guard is done moving, set next stop
                    _guardSet[i]->nextStop(patrolAction);
                }
                _guardSet[i]->patrolGuardAnim();
            }



            //detection for active guard
            else if (_guardSet[i]->state == "chaseD" and _actions->isActive(patrolAction)){
                Vec2 pos = _guardSet[i]->getNodePosition();
                _guardSet[i]->saveCurrentStop();
                _actions->remove(patrolAction);

                _guardSet[i]->updatePosition(pos);
            }
            //detection for static guard
            else if (_guardSet[i]->state == "chaseD"){
                Vec2 target = guardPos + ((_charPos - guardPos)/distance)*50;
                // chase
                _guardSet[i]->updateChaseTarget(target);
                _guardSet[i]->chaseChar(chaseAction);
                _guardSet[i]->chaseGuardAnim();
            }
            else if (_guardSet[i]->state == "chaseSP"){
                _guardSet[i]->updateChaseSPTarget(_guardSet[i]->chaseVec[0]);
                _guardSet[i]->chaseChar(chaseAction);
                // erase from return vector
                _guardSet[i]->eraseChaseSPVec();
                _guardSet[i]->chaseGuardAnim();
            }


            
        }
    }
    
    int findClosestNode(Vec2 pos){
        int closest = 0;
        int minDistance = 100000000;
        for (int i = 0; i < _nodes.size(); i++){
            int dis = pos.distance(_nodes[i]);
            if (dis < minDistance){
                minDistance = dis;
                closest = i;
            }
        }
        return closest;
    }
    
    
    void drawLines(Scene s, Vec2 a, Vec2 b){
        
        SplinePather splinePather = SplinePather();
        SimpleExtruder extruder = SimpleExtruder();
        Spline2 spline = Spline2(a, b);
        splinePather.set(&spline);
        splinePather.calculate();

        extruder.set(splinePather.getPath());
        extruder.calculate(1);
        Poly2 line = extruder.getPolygon();
        std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
        polyNode->setPolygon(line);
        
        polyNode->setColor(Color4::GREEN);
        
        polyNode->setPosition(a.getMidpoint(b));
        
        s->addChild(polyNode);
        
    }
    
    vector<Vec2> shortestPath(int start, int end){
        int n = _nodes.size();
        vector<int> dist(n, 1e9);
        dist[start] = 0;

        // initialize parent vector
        vector<int> parent(n, -1);

        // initialize queue
        queue<int> q;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < n; v++) {
                if (_adjMatrix[u][v] && dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        // backtrack from end to start to get path
        vector<Vec2> path;
        int curr = end;
        while (curr != -1) {
            path.push_back(_nodes[curr]);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        return path;
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


#endif /* __GUARDSET_CONTROLLER_H__ */
