//
//  LevelModel.cpp
//  tilemap-ios
//
//  Created by Ann Zhou on 3/19/23.
//

#include "LevelModel.h"
#include "LevelConstants.h"

#include <cugl/assets/CUJsonLoader.h>
#include <string>

#pragma mark -
#pragma mark Static Constructors

int totalHeight = 0;

/**
* Creates a new, empty level.
*/
LevelModel::LevelModel(void) : Asset()
{
    _world = std::make_unique<TilemapController>();
    _item = std::make_shared<ArtifactSetController>();
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
    _world->updateDimensions(Vec2(mapWidth, mapHeight));
    _world->updateTileSize(Size(tileWidth, tileHeight));
//    int totWidth = mapWidth *tileWidth;
    totalHeight = mapHeight *tileHeight;
    
    // Get each object in each layer
    for (int i = 0; i < json->get("layers")->size(); i++) {
        // Get the objects per layer
        auto objects = json->get("layers")->get(i)->get("objects");
        std::string type = json->get("layers")->get(i)->get("name")->asString();
        for (int j = 0; j < objects->size(); j++) {
            // For each object, determine what it is and load it
            loadObject(type, totalHeight, objects->get(j));
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
}


bool LevelModel::loadObject(const std::string type, int totalHeight, const std::shared_ptr<JsonValue>& json) {
    if (type == TILEMAP_FILED) {
        return loadTilemap(json);
    }
    if (type == WALLS_FIELD) {
        return loadArtifact(json);
    }
    if (type == ARTIFACTS_FIELD) {
        return loadArtifact(json);
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
    
    std::string textureType = json->get("type")->asString();

    int width = json->get("height")->asInt(); //128
    int height = json->get("width")->asInt(); //128
    int x = json->get("x")->asInt() / width; // x index
    int y = json->get("y")->asInt() / height - 1; // y index
    
//    x = 1792 -x;
//    y = 1152 -y;
    _world->addTile2(x, y, height, totalHeight, false, _assets, textureType);
    
    success = success && x >= 0 && y >= 0;
    return success;
}


/**
* Loads a single wall object
*/
bool LevelModel::loadWall(const std::shared_ptr<JsonValue>& json, int totalHeight) {
    bool success = true;
    std::string textureType = json->get("type")->asString();
    
    int width = json->get("height")->asInt();
    int height = json->get("width")->asInt();
    int x = json->get("x")->asInt();
    int y = totalHeight - json->get("y")->asInt();
    int rot = json->get("rotation")->asInt();
    
    Vec2 pos = Vec2 (x, y);
    Size size = Size(width, height);
    _item->add_this(pos, rot, size, false, _assets, textureType);

    success = success && x >= 0 && y >= 0;
    return success;
}

/**
* Loads an artifact object
*/
bool LevelModel::loadArtifact(const std::shared_ptr<JsonValue>& json) {
    bool success = true;
    std::string textureType = json->get("type")->asString();
    
    int width = json->get("height")->asInt();
    int height = json->get("width")->asInt();
    int x = json->get("x")->asInt(); // x pos
    int y = totalHeight - json->get("y")->asInt(); // totalHeight - yHeight
    int rot = json->get("rotation")->asInt();
    
    Vec2 pos = Vec2 (x, y);
    Size size = Size(width, height);
    if (textureType == RESOURCE_FIELD) {
        _item->add_this(pos, rot, size, true, _assets, textureType);
    } else {
        _item->add_this(pos, rot, size, false, _assets, textureType);
    }

    success = success && x >= 0 && y >= 0;
    return success;
}

void LevelModel::setTilemapTexture() {
    _world->setTexture(_assets);
    _item->setTexture(_assets);
};
