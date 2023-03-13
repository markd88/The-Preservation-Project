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
    std::vector<Guard> _guardSet;
    
#pragma mark Methods
public:
    GuardSetController();
    
    // add one guard
    Guard add_this(Vec2 gPos){
        Guard _guard = std::make_unique<GuardController>(gPos);
        _guardSet.push_back(std::move(_guard));
        return _guard;
    }
    
    
//    void clearMap();
};


#endif /* __GUARDSET_CONTROLLER_H__ */