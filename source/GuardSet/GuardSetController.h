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
#include <ItemSet/ItemSetController.h>

using namespace std;
#include <cmath>

// #define DURATION 3.0f

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
    
    std::shared_ptr<ItemSetController> _items;
    
    /**adjacency matrix*/
    bool** _adjMatrix;
    
    /** nodes to vec2 positions*/
    std::unordered_map<int, Vec2> _nodes;
    
    



#pragma mark Main Methods
public:
    
    GuardSetController(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, std::shared_ptr<TilemapController> world, std::shared_ptr<ItemSetController> items,
        bool** adjMatrix, std::unordered_map<int, Vec2> nodes)
    {
        _adjMatrix = adjMatrix;
        _nodes = nodes;
        _world = world;
        _items = items;
        _actions = actions;
        std::vector<Guard> _guardSet;
        

    };
    
#pragma mark Update Methods
public:


    // add one guard
    void add_this_moving(Vec2 gPos, std::shared_ptr<cugl::scene2::OrderedNode> s, const std::shared_ptr<cugl::AssetManager>& assets, vector<Vec2> patrol_stops, bool isPast){
        Guard _guard = std::make_unique<GuardController>(gPos, assets, patrol_stops, _actions, generateUniqueID(), isPast);
        _guard->addChildTo(s);
        _guardSet.push_back(std::move(_guard));
    }
    
    void add_this(Vec2 gPos, std::shared_ptr<cugl::scene2::OrderedNode> s, const std::shared_ptr<cugl::AssetManager>& assets, bool isPast, int dir){
        Guard _guard = std::make_unique<GuardController>(gPos, assets, _actions, generateUniqueID(), isPast, dir);
        _guard->addChildTo(s);
        _guardSet.push_back(std::move(_guard));
    }
    
    void addChildTo (std::shared_ptr<cugl::scene2::OrderedNode> s) {
        unsigned int vecSize = _guardSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _guardSet[i]->addChildTo(s);
        }
    }
    
    void removeChildFrom (std::shared_ptr<cugl::scene2::OrderedNode> s) {
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
    
    void patrol(Vec2 _charPos, float char_angle){

        static auto last_time_question = std::chrono::steady_clock::now();
        static auto last_time_lookaround = std::chrono::steady_clock::now();
        static auto start_question_inSP = std::chrono::steady_clock::now();
        // Calculate the time elapsed since the last call to pinch
        auto now = std::chrono::steady_clock::now();
        auto elapsed_question = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time_question);
        auto elapsed_lookaround = std::chrono::duration_cast<std::chrono::seconds>(now - last_time_lookaround);
        auto elapsed_question_inSP = std::chrono::duration_cast<std::chrono::seconds>(now - start_question_inSP);
        for (int i = 0; i < _guardSet.size(); i++){

            string id = std::to_string(_guardSet[i]->id);

            string chaseDAction = "chaseD" + id;
            string chaseSPAction = "chaseSP" + id;
            string patrolAction = "patrol" + id;
            string returnAction = "return" + id;


            Vec2 guardPos = _guardSet[i]->getNodePosition();
            float distance = guardPos.distance(_charPos);
            int charDirection = calculateMappedAngle(guardPos.x, guardPos.y, _charPos.x, _charPos.y);
            int guardFacingDirection = _guardSet[i]->getDirection();
            bool insideVisionCone = false;
            if ((guardFacingDirection + 8 -1) %8 == charDirection || (guardFacingDirection + 8 + 1) % 8 == charDirection || guardFacingDirection == charDirection) {
                insideVisionCone = true;
            }
            bool visual_detection = false;
            if (distance < 300 and _world->isActive() and insideVisionCone){
                // visual_detection = !_world->lineInObstacle(guardPos,_charPos);
                visual_detection = !_items->lineInObstacle(guardPos, _charPos);
            }

            bool acoustic_detection = false;
            if (distance < 150 and _world->isActive()) {
                acoustic_detection = true;
            }


#pragma mark Guard State Updates

            // static state
            if (_guardSet[i]->state == "static") {
           //     CULog("static");
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
          //      CULog("question");
                if (visual_detection) {
                    // chase immediately
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->stopQuestionAnim(id);
                    _guardSet[i]->updateState("chaseD");
                }
                else if (acoustic_detection && elapsed_question.count() <= 1000 ) {
                    // keep question
                    // CULog("question");
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
                else if (acoustic_detection && elapsed_question.count() > 1000) {
                    // chase in shortest path
              //      CULog("switch to chaseSP from question");
                    _guardSet[i]->stopQuestionAnim(id);
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);

                    int start = findClosestNode(_guardSet[i]->getNodePosition());
                    int finish = findClosestNode(_charPos);

//                    CULog("start: %d", start);
//                    CULog("finish : %d", finish);
                    vector<Vec2> sp =  shortestPath(start, finish);
                    _guardSet[i]->setChaseVec(sp);
                    _guardSet[i]->eraseChaseSPVec();
//                    CULog("chase SP shortest path cycle");
//                    for(int i=0; i < sp.size(); i++) {
//                        CULog( "x: %f, y: %f", sp.at(i).x, sp.at(i).y );
//                    }

                    _guardSet[i]->updateState("chaseSP");
                }
                else {
                    // return to the previous state
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
        //            CULog( " state before question: %s", _guardSet[i]->getStateBeforeQuestion().c_str());
                    _guardSet[i]->updateState(_guardSet[i]->getStateBeforeQuestion());
                    if (_guardSet[i]->getStateBeforeQuestion() == "question") {
                        last_time_question = now;
                    }

                }
            }

            else if (_guardSet[i]->state == "chaseD") {
      //          CULog("chaseD");
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
       //         CULog("chaseSP");
                if (visual_detection) {
        //            CULog("switch from chaseSP to chaseD");
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chaseD");
                }

                else if (_guardSet[i]->chaseVec.size() == 0 ){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("lookaround");
                    last_time_lookaround = now;
                    _guardSet[i]->setIfQuestionInSP(false);
                }

                else if (acoustic_detection) {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    if (_guardSet[i]->getIfQuestionInSP() == false) {
                        CULog("start question while chaseSP");
                        _guardSet[i]->setIfQuestionInSP(true);
                        start_question_inSP = now;
                    }
                    else{
                        if (elapsed_question_inSP.count() <= 2) {
                            // continue
                        }
                        else {
            //                CULog("recalculate chaseSP");
                            Vec2 pos = _guardSet[i]->getNodePosition();
                            _actions->remove(chaseSPAction);
                            _actions->remove("guard_animation");
                            _guardSet[i]->updatePosition(pos);

                            int start = findClosestNode(_guardSet[i]->getNodePosition());
                            int finish = findClosestNode(_charPos);

//                            CULog("start: %d", start);
//                            CULog("finish : %d", finish);
                            vector<Vec2> sp =  shortestPath(start, finish);
                            _guardSet[i]->setChaseVec(sp);
                            _guardSet[i]->eraseChaseSPVec();

//                            CULog("chase SP shortest path cycle");
//                            for(int i=0; i < sp.size(); i++) {
//                                CULog( "x: %f, y: %f", sp.at(i).x, sp.at(i).y );
//                            }

                            _guardSet[i]->setIfQuestionInSP(false);
                        }
                    }
                }

                else {
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->setIfQuestionInSP(false);
                }

            }

            else if (_guardSet[i]->state == "lookaround") {
           //     CULog("lookaround");
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
                else if (elapsed_lookaround.count() <= 2){
                    // keep lookaround
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
                else if (elapsed_lookaround.count() > 2) {
                    int start = findClosestNode(_guardSet[i]->getNodePosition());
                    int finish;
                    Vec2 true_point;
                    if (!_guardSet[i]->doesPatrol){
                        true_point = _guardSet[i]->getStaticPosition();
                        finish = findClosestNode(_guardSet[i]->getStaticPosition());
                    }else {
                        true_point = _guardSet[i]->getSavedStop();
                        finish = findClosestNode(_guardSet[i]->getSavedStop());
                    }

                    vector<Vec2> sp = shortestPath(start, finish);

                     sp.erase(sp.begin());
                     sp.pop_back();
                     sp.push_back(true_point);

           //         CULog("return path");
//                    for(int i=0; i < sp.size(); i++) {
//                        CULog( "x: %f, y: %f", sp.at(i).x, sp.at(i).y );
//                    }

                    _guardSet[i]->setReturnVec(sp);

                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("return");

                }
            }

            //patrol state
            else if (_guardSet[i]->state == "patrol"){
            //    CULog("patrol");
                //detection in patrol state = chase
                if (visual_detection){
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("chaseD");
                    _guardSet[i]->saveCurrentStop();
                }
                else if (acoustic_detection){
                    // if did not see, but hear
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                    _guardSet[i]->updateState("question");
                    Vec2 pos =  _guardSet[i]->getNodePosition();
                    _actions->remove(patrolAction);
                    _guardSet[i]->updatePosition(pos);
                    last_time_question = now;
                    _guardSet[i]->setStateBeforeQuestion("patrol");

                    _guardSet[i]->saveCurrentStop();

                }
                //keep patroling otherwise
                else{
                    _guardSet[i]->updatePrevState(_guardSet[i]->state);
                }
                
            }

            
            else if (_guardSet[i]->state == "return"){
        //        CULog("return");
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
                    Vec2 pos = _guardSet[i]->getNodePosition();
                    _actions->remove(returnAction);
                    _guardSet[i]->updatePosition(pos);
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


        auto elapsed_question = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time_question);
        auto elapsed_lookaround = std::chrono::duration_cast<std::chrono::seconds>(now - last_time_lookaround);
        auto elapsed_question_inSP = std::chrono::duration_cast<std::chrono::seconds>(now - start_question_inSP);

        // CULog("%d", elapsed_question);

#pragma mark Guard action according to state



            if (_guardSet[i]->state == "static") {
                Vec2 pos = _guardSet[i]->getNodePosition();
                _actions->remove(patrolAction);
                _actions->remove(chaseSPAction);
                _actions->remove(chaseDAction);
                _guardSet[i]->updatePosition(pos);
                _guardSet[i]->stopQuestionAnim(id);
                _guardSet[i]->staticGuardAnim(id);
            }
            else if (_guardSet[i]->state == "question") {
                Vec2 pos = _guardSet[i]->getNodePosition();
                _actions->remove(patrolAction);
                _actions->remove(chaseSPAction);
                _actions->remove(chaseDAction);
                _guardSet[i]->updatePosition(pos);
                _guardSet[i]->questionAnim(id, elapsed_question.count());
            }
            else if (_guardSet[i]->state == "lookaround") {
                Vec2 pos = _guardSet[i]->getNodePosition();
                _actions->remove(patrolAction);
                _actions->remove(chaseSPAction);
                _actions->remove(chaseDAction);
                _guardSet[i]->updatePosition(pos);
                _guardSet[i]->lookAroundAnim(id);
            }
            // return state
            else if (_guardSet[i]->state == "return"){
                if (_actions->isActive(returnAction)) {
                    // let it finish
                }
                else {
                    _guardSet[i]->updateReturnTarget(_guardSet[i]->returnVec[0]);
                    _guardSet[i]->returnGuard(returnAction);
                    // erase from return vector
                    _guardSet[i]->eraseReturnVec();
                }

                _guardSet[i]->returnGuardAnim(id);
            }
            //patrol state
            else if (_guardSet[i]->state == "patrol" and _guardSet[i]->doesPatrol){

                if(_actions->isActive(patrolAction)){
                    // guard is moving properly, wait till finished
                    _guardSet[i]->updatePosition(_guardSet[i]->getNodePosition());
                }
                else{
                    // guard is done moving, set next stop
                    _guardSet[i]->nextStop(patrolAction);
                }
                _guardSet[i]->patrolGuardAnim(id);
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
                if (_actions->isActive(chaseDAction)) {
                    // wait for it to finish
                }
                else {

                    Vec2 pos = _guardSet[i]->getNodePosition();

                    _actions->remove(chaseSPAction);
                    _actions->remove(chaseDAction);
                    _guardSet[i]->updatePosition(pos);

                    Vec2 target = guardPos + ((_charPos - guardPos)/distance)*50;
                    // chase
                    _guardSet[i]->updateChaseTarget(target);
                    _guardSet[i]->chaseChar(chaseDAction);
                }
                _guardSet[i]->chaseGuardAnim(id);
            }
            else if (_guardSet[i]->state == "chaseSP"){
                if (_actions->isActive(chaseSPAction)) {
                    // wait for it to finish
            //        CULog("in the process of chase SP");
                    _guardSet[i]->chaseGuardAnim(id);
                }

                else {
                    _guardSet[i]->updateChaseSPTarget(_guardSet[i]->chaseVec[0]);
                    _guardSet[i]->chaseChar(chaseSPAction);
                    _guardSet[i]->chaseGuardAnim(id);
             //       CULog("start chasing from %f  %f to %f  %f ", _guardSet[i]->getNodePosition().x, _guardSet[i]->getNodePosition().y,_guardSet[i]->chaseVec[0].x, _guardSet[i]->chaseVec[0].y );
                    // erase from return vector
                    _guardSet[i]->eraseChaseSPVec();
                }


            }

            Vec2 pos = _guardSet[i]->getNodePosition();
            _guardSet[i]->updatePosition(pos);
            
        }
    }
    
    int findClosestNode(Vec2 pos){
     //   CULog("original postion: %f  %f", pos.x, pos.y);
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


//        CULog("inside shortest path");
//        for(int i=0; i < path.size(); i++) {
//            CULog( "x: %f, y: %f", path.at(i).x, path.at(i).y );
//        }



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
    
    void updatePriority(){
        for(auto &guard : _guardSet){
            guard->updatePriority();
        }
    }


};


#endif /* __GUARDSET_CONTROLLER_H__ */
