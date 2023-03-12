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
private:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<ArtifactController> Artifact;
    typedef std::vector<Artifact> ArtifactSet;
    ArtifactSet _artifactSet;
    
#pragma mark Methods
public:
    ArtifactSetController();
    
    void add_one(Artifact a) {
        _artifactSet.push_back(a);
    }
};


#endif /* __ARTIFACTSET_CONTROLLER_H__ */
