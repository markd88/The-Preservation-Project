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
    
    // Get each object in each layer
    for (int i = 0; i < json->get("layers")->size(); i++) {
        // Get the objects per layer
        auto objects = json->get("layers")->get(i)->get("objects");
        std::string type = json->get("layers")->get(i)->get("name")->asString();
        for (int j = 0; j < objects->size(); j++) {
            // For each object, determine what it is and load it
            std::cout<<type<<std::endl;
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
}


bool LevelModel::loadObject(const std::string type, const std::shared_ptr<JsonValue>& json) {
    if (type == WALLS_FIELD) {
        return loadWall(json);
    }
    if (type == TILEMAP_FILED) {
        return loadTilemap(json);
    }
    if (type == ARTIFACTS_FIELD) {
        return loadArtifact(json);
    }
    if (type == CHARACTER_FIELD) {
        return loadCharacter(json);
    }
    if (type == GUARD_FIELD) {
        return loadGuard(json);
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

    int width = json->get("height")->asInt();
    int height = json->get("width")->asInt();
    int x = json->get("x")->asInt() / width;
    int y = json->get("y")->asInt() / height - 1;
    
//    x = 1536 -x;
//    y = 896 -y;
    
    // TODO: replace below
    _world->addTile2(x, y, false, _assets, textureType);
    
    success = success && x >= 0 && y >= 0;
    return success;
}


/**
* Loads a single wall object
*/
bool LevelModel::loadWall(const std::shared_ptr<JsonValue>& json) {
    bool success = true;
    
    std::string textureType = json->get("type")->asString();

    int width = json->get("height")->asInt();
    int height = json->get("width")->asInt();
    int x = json->get("x")->asInt() / width;
    int y = json->get("y")->asInt() / height -1;
    
    
    // TODO: replace below
    _world->addTile2(x, y, true, _assets, textureType);
    
    success = success && x >= 0 && y >= 0;
    return success;
}

/**
* Loads an artifact object
*/
bool LevelModel::loadArtifact(const std::shared_ptr<JsonValue>& json) {
    bool success = true;
    std::string textureType = json->get("type")->asString();
    
    int width = json->get("height")->asInt() /2;
    int height = json->get("width")->asInt() /2;
//    int x = json->get("x")->asInt() -738;
//    int y = 896 - json->get("y")->asInt() -430.5;
    int x = json->get("x")->asInt();
    int y = 896 - json->get("y")->asInt() +40; 
    
    Vec2 pos = Vec2 (x, y);
    Size size = Size(width, height);
    if (textureType == RESOURCE_FIELD) {
        _item->add_this(pos, size, true, _assets, textureType);
    } else {
        _item->add_this(pos, size, false, _assets, textureType);
    }

    success = success && x >= 0 && y >= 0;
    return success;
}


/**
* Loads character initial position
*/
bool LevelModel::loadCharacter(const std::shared_ptr<JsonValue>& json) {
    bool success = true;
    
    std::string textureType = json->get("type")->asString();

    int x = json->get("x")->asInt() - 50;
    int y = 896 - json->get("y")->asInt() + 80;

    _characterPos.set(x, y);
    
    return success;
}

/**
* Loads a guard object
*/
bool LevelModel::loadGuard(const std::shared_ptr<JsonValue>& json) {
    bool success = true;
    
    // in case old tileset is loaded
    if (json->get("properties") == nullptr) {
        return false;
    }
    
    std::string textureType = json->get("type")->asString();
    bool isStatic = json->get("properties")->get(0)->get("value")->asBool();
    
    if (isStatic) {
        int x = json->get("x")->asInt();
        int y = 896 - json->get("y")->asInt() + 40;
        _staticGuardsPos.push_back(Vec2(x, y));
    } else {
        std::vector<Vec2> patrolPoints;
        // parse input path with format x1,y1:x2,y2...
        std::string inputPath = json->get("properties")->get(1)->get("value")->asString();
        size_t start = 0;
        size_t end = inputPath.find(':');

        while (end != std::string::npos) {
            Vec2 vec;
            size_t comma = inputPath.find(',', start);
            vec.x = std::stoi(inputPath.substr(start, comma - start));
            vec.y = 896 - std::stoi(inputPath.substr(comma + 1, end - comma - 1)) + 40;
            patrolPoints.push_back(vec);
            start = end + 1;
            end = inputPath.find(':', start);
        }

        Vec2 vec;
        size_t comma = inputPath.find(',', start);
        vec.x = std::stoi(inputPath.substr(start, comma - start));
        vec.y = std::stoi(inputPath.substr(comma + 1));
        patrolPoints.push_back(vec);

        _movingGuardsPos.push_back(patrolPoints);
        
        // Print the vector of Vec2s
        for (const auto& v : patrolPoints) {
            std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
        }

    }

    return success;
}

void LevelModel::setTilemapTexture() {
    _world->setTexture(_assets);
    _item->setTexture(_assets);
};
