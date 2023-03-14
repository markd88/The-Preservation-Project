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
//    Artifact _artifact;
    typedef std::vector<Artifact> ArtifactSet;
    ArtifactSet _artifactSet;
    typedef std::shared_ptr<cugl::Scene2> Scene;
    
#pragma mark Methods
public:
    ArtifactSetController(const std::shared_ptr<cugl::AssetManager>& assets) {
        ArtifactSet _artifactSet;
    };

#pragma mark Update Methods
public:
    
    void add_this(Vec2 aPos, Scene s, const std::shared_ptr<cugl::AssetManager>& assets, bool isResource){
        Artifact _artifact = std::make_unique<ArtifactController>(aPos, assets, isResource);
        _artifact->addChildTo(s);
        _artifactSet.push_back(std::move(_artifact));
    }
};


#endif /* __ARTIFACTSET_CONTROLLER_H__ */
