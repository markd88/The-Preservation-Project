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
LevelModel::LevelModel(void) : Asset(),
_root(nullptr),
//_world(nullptr),
_worldnode(nullptr),
//_debugnode(nullptr),
//_rocket(nullptr),
//_goalDoor(nullptr),
_scale(32, 32)
{
    _bounds.size.set(1.0f, 1.0f);
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
    if (_root == nullptr) {
        return;
    }
    _worldnode->removeFromParent();
    _worldnode->removeAllChildren();
    _worldnode = nullptr;
  
//    _debugnode->removeFromParent();
//    _debugnode->removeAllChildren();
//    _debugnode = nullptr;

    _root = nullptr;
}

/**
* Sets the scene graph node for drawing purposes.
*
* The scene graph is completely decoupled from the physics system.  The node
* does not have to be the same size as the physics body. We only guarantee
* that the node is positioned correctly according to the drawing scale.
*
* @param value  the scene graph node for drawing purposes.
*
* @retain  a reference to this scene graph node
* @release the previous scene graph node used by this object
*/
void LevelModel::setRootNode(const std::shared_ptr<scene2::SceneNode>& node) {
    if (_root != nullptr) {
        clearRootNode();
    }

    // Set content size to match the size of the game level
    _root = node;
//    float xScale = (_world->getBounds().getMaxX() * _scale.x) / _root->getContentSize().width;
//    float yScale = (_world->getBounds().getMaxY() * _scale.y) / _root->getContentSize().height;
    float xScale = 0.5;
    float yScale = 0.5;
    
    _root->setContentSize(_root->getContentSize().width * xScale, _root->getContentSize().height * yScale);

    _scale.set(_root->getContentSize().width/_bounds.size.width,
             _root->getContentSize().height/_bounds.size.height);

    // Create, but transfer ownership to root
    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setPosition(Vec2::ZERO);
  
//    _debugnode = scene2::SceneNode::alloc();
//    _debugnode->setScale(_scale); // Debug node draws in PHYSICS coordinates
//    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
//    _debugnode->setPosition(Vec2::ZERO);
  
    _root->addChild(_worldnode);
//    _root->addChild(_debugnode);

    // Add the individual elements
    std::shared_ptr<scene2::PolygonNode> poly;
    std::shared_ptr<scene2::WireNode> draw;

//    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
//        std::shared_ptr<WallModel> wall = *it;
//        auto sprite = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>(wall->getTextureKey()),
//                                                            wall->getPolygon() * _scale);
//        addObstacle(wall,sprite);  // All walls share the same texture
//    }
        
}

/**
* Toggles whether to show the debug layer of this game world.
*
* The debug layer displays wireframe outlines of the physics fixtures.
*
* @param  flag whether to show the debug layer of this game world
*/
//void LevelModel::showDebug(bool flag) {
//    if (_debugnode != nullptr) {
//        _debugnode->setVisible(flag);
//    }
//}


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
    // Initial geometry
    float w = json->get(WIDTH_FIELD)->asFloat();
    float h = json->get(HEIGHT_FIELD)->asFloat();
//    float g = json->get("properties")->get(0)->getFloat("value", -4.9);
    _bounds.size.set(w, h);
//    _gravity.set(0,g);

    /** Create the physics world */
//    _world = physics2::ObstacleWorld::alloc(getBounds(),getGravity());

    // Get each object in each layer, then decide what to do based off of what
    // type the object is.
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

Vec2 LevelModel::getObjectPos(const std::shared_ptr<JsonValue>& json) {
    Vec2 pos = Vec2(json->getFloat("x") / _scale.x, ((_bounds.getMaxY() * _scale.y) - json->getFloat("y") + _scale.y) / _scale.y);
    return pos;
}


std::vector<float> LevelModel::getVertices(const std::shared_ptr<JsonValue>& json) {
    std::vector<float> vertices = std::vector<float>();
    for (auto it = json->get(VERTICES_FIELD)->children().begin(); it != json->get(VERTICES_FIELD)->children().end(); it++) {
        vertices.push_back(((_bounds.getMaxY() * _scale.y) - ((*it)->get(1)->asFloat() + json->getFloat("y"))) / _scale.y);
        vertices.push_back(((*it)->get(0)->asFloat() + json->getFloat("x")) / _scale.x);
    }
    // reversing is necessary so that the resulting polygon is in the right draw order
    std::reverse(vertices.begin(), vertices.end());
    return vertices;
}

bool LevelModel::loadObject(const std::string type, const std::shared_ptr<JsonValue>& json) {
//    if (json->get("class") == nullptr) {
//        CULog("json->get(class) is nullptr...");
//    }
    std::cout<<type<<std::endl;
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
    std::cout<<textureType<<std::endl;
    int x = json->get("x")->asInt();
    int y = json->get("y")->asInt();
    int width = json->get("width")->asInt();
    int height = json->get("height")->asInt();
    
    // TODO: replace below
    _primaryWorld->updateDimensions(Vec2(144, 84));
    _primaryWorld->updateColor(Color4::WHITE);
    _primaryWorld->updateTileSize(Size(8, 8));

    _primaryWorld->addTile(x, y, Color4::BLACK, false);
    
    success = success && x >= 0 && y >= 0;
    return success;
}


/**
* Loads a single wall object
*/
bool LevelModel::loadWall(const std::shared_ptr<JsonValue>& json) {
    bool success = true;

    std::string textureType = json->get("type")->toString();
    int x = json->get("x")->asInt();
    int y = json->get("y")->asInt();
    int width = json->get("width")->asInt();
    int height = json->get("height")->asInt();
    
    // TODO: replace below
    _primaryWorld->addTile(x, y, Color4::RED, true);
    
    success = success && x >= 0 && y >= 0;
    return success;
}
