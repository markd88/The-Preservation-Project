//
//  LevelModel.cpp
//  tilemap-ios
//
//  Created by Ann Zhou on 3/19/23.
//

#include "LevelModel.h"
#include "LevelConstants.h"

#include <cugl/assets/CUJsonLoader.h>
//#include "TDExitModel.h"
//#include "TDCrateModel.h"
//#include "TDWallModel.h"
#include <string>

#pragma mark -
#pragma mark Static Constructors

/**
* Creates a new, empty level.
*/
LevelModel::LevelModel(void) : Asset()
{
    _primaryWorld = std::make_unique<TilemapController>();
        
    _secondaryWorld = std::make_unique<TilemapController>();
}

/**
* Destroys this level, releasing all resources.
*/
LevelModel::~LevelModel(void) {
    unload();
    clearRootNode();
}


#pragma mark -
#pragma mark Drawing Methods
/**
* Sets the drawing scale for this game level
*
* The drawing scale is the number of pixels to draw before Box2D unit. Because
* mass is a function of area in Box2D, we typically want the physics objects
* to be small.  So we decouple that scale from the physics object.  However,
* we must track the scale difference to communicate with the scene graph.
*
* We allow for the scaling factor to be non-uniform.
*
* @param value  the drawing scale for this game level
*/
//void LevelModel::setDrawScale(float value) {
//    if (_rocket != nullptr) {
//        _rocket->setDrawScale(value);
//    }
//}

/**
* Clears the root scene graph node for this level
*/
void LevelModel::clearRootNode() {
}


#pragma mark -
#pragma mark Asset Loading
/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel::preload(const std::string& file) {
    std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);
    return preload(reader->readJson());
}

/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel:: preload(const std::shared_ptr<cugl::JsonValue>& json) {
    if (json == nullptr) {
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    // Initial map
    int mapHeight = json->get(MAP_HEIGHT)->asInt() / 2;
    int mapWidth = json->get(MAP_WIDTH)->asInt() / 2;
    
    int tileHeight = json->get(TILE_HEIGHT)->asInt() * 2;
    int tileWidth = json->get(TILE_WIDTH)->asInt() * 2;
    _primaryWorld->updateDimensions(Vec2(mapWidth, mapHeight));
    _primaryWorld->updateTileSize(Size(tileWidth, tileHeight));


    // Get each object in each layer
    for (int i = 0; i < json->get("layers")->size(); i++) {
        // Get the objects per layer
        auto objects = json->get("layers")->get(i)->get("objects");
        std::string type = json->get("layers")->get(i)->get("name")->asString();
        for (int j = 0; j < objects->size(); j++) {
            // For each object, determine what it is and load it
            loadObject(type, objects->get(j));
        }
    }

    return true;
}

/**
* Unloads this game level, releasing all sources
*
* This load method should NEVER access the AssetManager.  Assets are loaded and
* unloaded in parallel, not in sequence.  If an asset (like a game level) has
* references to other assets, then these should be disconnected earlier.
*/
void LevelModel::unload() {
//    _crates.clear();
//    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
//        if (_world != nullptr) {
//            _world->removeObstacle((*it).get());
//        }
//    (*it) = nullptr;
//    }
//    _walls.clear();
//    if (_world != nullptr) {
//        _world->clear();
//        _world = nullptr;
//    }
}


bool LevelModel::loadObject(const std::string type, const std::shared_ptr<JsonValue>& json) {
//    if (json->get("class") == nullptr) {
//        CULog("json->get(class) is nullptr...");
//    }
//    auto type = json->get("class")->asString();
    if (type == WALLS_FIELD) {
        return loadWall(json);
    } else if (type == TILEMAP_FILED) {
        return loadTilemap(json);
    }
    return false;
}

#pragma mark -
#pragma mark Individual Loaders

/**
* Loads a single tile object
*/
bool LevelModel::loadTilemap(const std::shared_ptr<JsonValue>& json) {
    bool success = true;
    
    std::string textureType = json->get("type")->toString();

    int width = json->get("width")->asInt();
    int height = json->get("height")->asInt();
    int x = json->get("x")->asInt() / width;
    int y = json->get("y")->asInt() / height - 1;
    
    // TODO: replace below
//    _primaryWorld->addTile(x, y, Color4::BLACK, false);
    _primaryWorld->addTile2(x, y, false, _assets, textureType);
//    int &test = 999;
//    _primaryWorld->addTile2(x, y, false, _assets, test);

    success = success && x >= 0 && y >= 0;
    return success;
}


/**
* Loads a single wall object
*/
bool LevelModel::loadWall(const std::shared_ptr<JsonValue>& json) {
    bool success = true;

    std::string textureType = json->get("type")->toString();
    
    int width = json->get("width")->asInt();
    int height = json->get("height")->asInt();
    int x = json->get("x")->asInt() / width;
    int y = json->get("y")->asInt() / height - 1;
    
    // TODO: replace below
    _primaryWorld->addTile(x, y, Color4::RED, true);
    
    success = success && x >= 0 && y >= 0;
    return success;
}

//void LevelModel::setTilemapTexture() {
//    _primaryWorld->setTexture(_assets);
//};
