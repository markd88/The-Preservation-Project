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

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class GuardSetController {
    
#pragma mark External References
private:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<GuardController> Guard;
//    Guard _guard;
    std::vector<Guard> _guardSet;
    typedef std::shared_ptr<cugl::Scene2> Scene;
    

//#pragma mark Main Methods
//public:
//    GuardSetController();
    
#pragma mark Update Methods
public:
    // add one guard
    void add_this(Vec2 gPos, Scene s){
        Guard _guard = std::make_unique<GuardController>(gPos);
        _guard->addChildTo(s);
        _guardSet.push_back(std::move(_guard));
    }
    
//    void addChildTo(Scene2 s) {
//        unsigned int vecSize = _guardSet.size();
//        // run for loop from 0 to vecSize
//        for(unsigned int i = 0; i < vecSize; i++)
//        {
//            _guardSet[i];
//        }
//    }

};


#endif /* __GUARDSET_CONTROLLER_H__ */
