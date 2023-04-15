//
//  WallSetController.h
//  Tilemap
//
//  Created by Hao Chen on 4/15/23.
//

#ifndef WallSetController_h
#define WallSetController_h

#include "Wall/WallModel.h"
#include "Wall/WallView.h"
#include "Wall/WallController.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class WallSetController {
    
#pragma mark External References
public:
    /** Tilemape is a vector list of tiles */
    typedef std::unique_ptr<WallController> Wall;
    typedef std::vector<Wall> WallSet;
    WallSet _WallSet;
    typedef std::shared_ptr<cugl::Scene2> Scene;
    
#pragma mark Methods
public:
    
    WallSetController() {
        WallSet _WallSet;
    };

#pragma mark Update Methods
public:
    
//    void add_this(Vec2 aPos, const std::shared_ptr<cugl::AssetManager>& assets, bool isResource){
//        Wall _Wall = std::make_unique<WallController>(aPos, assets, isResource);
////        _Wall->addChildTo(s);
//        _WallSet.push_back(std::move(_Wall));
//    }
    
    void add_this(Vec2 aPos, float rot, Size size, bool isResource, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey){
        Wall _Wall = std::make_unique<WallController>(aPos, rot, size, isResource, assets, textureKey);
        _WallSet.push_back(std::move(_Wall));
    }
    

    // idx is the idx of this item in this vec
    void remove_this(int idx, Scene s){
        _WallSet[idx]->removeChildFrom(s);
        _WallSet.erase(_WallSet.begin() + idx);
    }

    
    void addChildTo (Scene s) {
        unsigned int vecSize = _WallSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _WallSet[i]->addChildTo(s);
        }
    }
    
    void removeChildFrom (Scene s) {
        unsigned int vecSize = _WallSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _WallSet[i]->removeChildFrom(s);
        }
    }
    
    void clearSet () {
        _WallSet.clear();
    }
    
    void setVisibility(bool visible){
        int vecSize = _WallSet.size();
        for(int i = 0; i < vecSize; i++) {
            _WallSet[i]->setVisibility(visible);
        }
    }
    
    void updateTileSize(Size tileSize) {
        // TODO: Implement me
        unsigned int vecSize = _WallSet.size();
        for(unsigned int i = 0; i < vecSize; i++) {
            if(_WallSet[i] != nullptr){
                _WallSet[i]->updateSize(tileSize);
            }
        }
    }
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets){
        unsigned int vecSize = _WallSet.size();
        for(unsigned int i = 0; i < vecSize; i++) {
            if(_WallSet[i] != nullptr){
                std::string textureKey = _WallSet[i]->getTextureKey();
                if (textureKey != "") {
                    _WallSet[i]->setTexture(assets, textureKey);
                }
            }
        }
    }
};

#endif /* WallSetController_h */
