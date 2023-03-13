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
    
    void add_this(Artifact a) {
        _artifactSet.push_back(std::move(a));
    }
    
//    void addTile(int col, int row, Color4 color, bool is_obs) {
//        if(col < 0 || row < 0 || col >= _model->dimensions.x || row >= _model->dimensions.y){
//            return;
//        }
//        Artifact temp = std::make_unique<ArtifactController>(Vec2(col * _model->tileSize.width, row * _model->tileSize.height), _model->tileSize, color, is_obs);
//
//        _artifactSet[row][col] = std::move(temp);
//        _artifactSet[row][col]->addChildTo(_view->getNode());
//    }
};


#endif /* __ARTIFACTSET_CONTROLLER_H__ */
