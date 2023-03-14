//
//  artifactSetController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef __ARTIFACTSET_CONTROLLER_H__
#define __ARTIFACTSET_CONTROLLER_H__

#include "Artifact/ArtifactModel.h"
#include "Artifact/ArtifactView.h"
#include "Artifact/ArtifactController.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class ArtifactSetController {
    
#pragma mark External References
public:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<ArtifactController> Artifact;
    Artifact _artifact;
    typedef std::vector<Artifact> ArtifactSet;
    ArtifactSet _artifactSet;
    typedef std::shared_ptr<cugl::Scene2> Scene;
    
//#pragma mark Methods
//public:
//    ArtifactSetController();

#pragma mark Update Methods
public:
    
    void add_this(Vec2 aPos, Scene s, bool isResource){
        Artifact _artifact = std::make_unique<ArtifactController>(aPos, isResource);
        _artifact->addChildTo(s);
        _artifactSet.push_back(std::move(_artifact));
    }
    
    // idx is the idx of this item in this vec
    void remove_this(int idx, Scene s){
        _artifactSet[idx]->removeChildFrom(s);
        _artifactSet.erase(_artifactSet.begin() + idx);
    }
};


#endif /* __ARTIFACTSET_CONTROLLER_H__ */
