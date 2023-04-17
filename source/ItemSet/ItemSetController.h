//
//  ItemSetController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef __ITEMSET_CONTROLLER_H__
#define __ITEMSET_CONTROLLER_H__

#include "Item/ItemModel.h"
#include "Item/ItemView.h"
#include "Item/ItemController.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class ItemSetController {
    
#pragma mark External References
public:
    /** Tilemape is a vector list of tiles */
    typedef std::unique_ptr<ItemController> Item;
//    Item _Item;
    typedef std::vector<Item> ItemSet;
    ItemSet _itemSet;
    typedef std::shared_ptr<cugl::Scene2> Scene;
    
#pragma mark Methods
public:
//    ItemSetController(const std::shared_ptr<cugl::AssetManager>& assets) {
//        ItemSet _ItemSet;
//    };
    
    ItemSetController() {
        ItemSet _itemSet;
    };

#pragma mark Update Methods
public:
    
//    void add_this(Vec2 aPos, const std::shared_ptr<cugl::AssetManager>& assets, bool isResource){
//        Item _Item = std::make_unique<ItemController>(aPos, assets, isResource);
////        _Item->addChildTo(s);
//        _ItemSet.push_back(std::move(_Item));
//    }
    
    void add_this(Vec2 aPos, float rot, Size size, bool isArtifact, bool isResource, bool isWall, const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey){
        Item _item = std::make_unique<ItemController>(aPos, rot, size, isArtifact, isResource, isWall, assets, textureKey);
        _itemSet.push_back(std::move(_item));
    }
    

    // idx is the idx of this item in this vec
    void remove_this(int idx, Scene s){
        _itemSet[idx]->removeChildFrom(s);
        _itemSet.erase(_itemSet.begin() + idx);
    }

    
    void addChildTo (Scene s) {
        unsigned int vecSize = _itemSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _itemSet[i]->addChildTo(s);
        }
    }
    
    void removeChildFrom (Scene s) {
        unsigned int vecSize = _itemSet.size();
        // run for loop from 0 to vecSize
        for(unsigned int i = 0; i < vecSize; i++) {
            _itemSet[i]->removeChildFrom(s);
        }
    }
    
    void clearSet () {
        _itemSet.clear();
    }
    
    void setVisibility(bool visible){
        int vecSize = _itemSet.size();
        for(int i = 0; i < vecSize; i++) {
            _itemSet[i]->setVisibility(visible);
        }
    }
    
    void updateTileSize(Size tileSize) {
        // TODO: Implement me
        unsigned int vecSize = _itemSet.size();
        for(unsigned int i = 0; i < vecSize; i++) {
            if(_itemSet[i] != nullptr){
//                Vec2 temp = Vec2(j * _model->tileSize.height, i * _model->tileSize.width);
//                _ItemSet[i]->updatePosition(temp);
                _itemSet[i]->updateSize(tileSize);
            }
        }
    }
    
    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets){
        unsigned int vecSize = _itemSet.size();
        for(unsigned int i = 0; i < vecSize; i++) {
            if(_itemSet[i] != nullptr){
                std::string textureKey = _itemSet[i]->getTextureKey();
                if (textureKey != "") {
                    _itemSet[i]->setTexture(assets, textureKey);
                }
            }
        }
    }
    
    bool inObstacle(Vec2 point){
        unsigned int vecSize = _itemSet.size();
        for(unsigned int i = 0; i < vecSize; i++) {
            if(_itemSet[i] != nullptr && _itemSet[i]->isObs() && _itemSet[i]->contains(point)){
                return true;
            }
        }
        return false;
    }
};


#endif /* __ITEMSET_CONTROLLER_H__ */
