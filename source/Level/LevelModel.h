//
//  LevelModel.h
//  tilemap-ios
//
//  Created by Ann Zhou on 3/19/23.
//

#ifndef LevelModel_h
#define LevelModel_h

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/assets/CUAsset.h>
#include <Tilemap/TilemapController.h>
#include <ItemSet/ItemSetController.h>

using namespace cugl;

#pragma mark -
#pragma mark Level Model
/**
* Class that represents a dynamically loaded level in the game
*
* This class is a subclass of Asset so that we can use it with a GenericLoader. As with
* all assets, this class SHOULD NOT make any references to AssetManager in the load/unload
* methods. Assets should be treated as if they load in parallel, not in sequence.  Therefore,
* it is unsafe to assume that one asset loads before another.  If this level needs to connect
* to other assets (sound, images, etc.) this should take place after asset loading, such as
* during scene graph initialization.
*/
class LevelModel : public Asset {
protected:
    std::shared_ptr<TilemapController> _world;
    std::shared_ptr<ItemSetController> _obs;
    std::shared_ptr<ItemSetController> _wall;
    std::shared_ptr<ItemSetController> _item;
    std::shared_ptr<ItemSetController> _exit;
    std::shared_ptr<ItemSetController> _resources;
    std::shared_ptr<ItemSetController> _shadows;

    /** The AssetManager for the game mode */
    std::shared_ptr<cugl::AssetManager> _assets;
    cugl::Vec2 _characterPos;
    
    std::vector<std::vector<cugl::Vec2>> _movingGuardsPos;
    std::vector<std::vector<int>> _staticGuardsPos;


#pragma mark Internal Helper
    /**
    * Determines which object is currently being parsed,
    * and then sends the object over
    * to the respective load function
    *
    * @param reader a JSON reader with cursor ready to read the object
    *
    * @return true if the object was successfully loaded
    */
    bool loadObject(const std::string type, int totalHeight, const std::shared_ptr<JsonValue>& json);

    /** Load single object*/
    bool loadTilemap(const std::shared_ptr<JsonValue>& json);
    bool loadItem(const std::shared_ptr<JsonValue>& json, const std::string type);
    bool load(const std::shared_ptr<JsonValue>& json);
    bool loadCharacter(const std::shared_ptr<JsonValue>& json);
    bool loadGuard(const std::shared_ptr<JsonValue>& json);

    /**
     * Clears the root scene graph node for this level
     */
    void clearRootNode();


public:
#pragma mark Static Constructors
    /**
     * Creates a new game level with no source file.
     *
     * The source file can be set at any time via the setFile() method. This method
     * does NOT load the asset.  You must call the load() method to do that.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<LevelModel> alloc() {
        std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
        return (result->init("") ? result : nullptr);
    }

    /**
     * Creates a new game level with the given source file.
     *
     * This method does NOT load the level. You must call the load() method to do that.
     * This method returns false if file does not exist.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<LevelModel> alloc(std::string file) {
        std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
        return (result->init(file) ? result : nullptr);
    }

#pragma mark Model Access
    
    /** Get world map */
    std::shared_ptr<TilemapController> getWorld() {return _world;};
    std::shared_ptr<ItemSetController> getItem() {return _item->copy();};
    std::shared_ptr<ItemSetController> getObs() {return _obs;};
    std::shared_ptr<ItemSetController> getWall() {return _wall;};
    std::shared_ptr<ItemSetController> getShadow() {return _shadows;};
    cugl::Vec2 getCharacterPos() {return _characterPos;};
    std::vector<std::vector<cugl::Vec2>> getMovingGuardsPos() {return _movingGuardsPos;};
    std::vector<std::vector<int>> getStaticGuardsPos() {return _staticGuardsPos;};
    std::shared_ptr<ItemSetController> getExit() {return _exit;};
    std::shared_ptr<ItemSetController> getResources() {return _resources->copy();};

#pragma mark Drawing Methods

    /**
     * Sets the loaded assets for this game level
     *
     * @param assets the loaded assets for this game level
     */
    void setAssets(const std::shared_ptr<AssetManager>& assets) { _assets = assets;  }
    
    void setTilemapTexture();

#pragma mark -
#pragma mark Asset Loading
    /**
     * Loads this game level from the source file
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded in
     * parallel, not in sequence.  If an asset (like a game level) has references to
     * other assets, then these should be connected later, during scene initialization.
     *
     * @param file the name of the source file to load from
     *
     * @return true if successfully loaded the asset from a file
     */
    virtual bool preload(const std::string& file);


    /**
     * Loads this game level from a JsonValue containing all data from a source Json file.
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded in
     * parallel, not in sequence.  If an asset (like a game level) has references to
     * other assets, then these should be connected later, during scene initialization.
     *
     * @param json the json loaded from the source file to use when loading this game level
     *
     * @return true if successfully loaded the asset from the input JsonValue
     */
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

    /**
     * Unloads this game level, releasing all sources
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded and
     * unloaded in parallel, not in sequence.  If an asset (like a game level) has
     * references to other assets, then these should be disconnected earlier.
     */
    void unload();

    /**
     * Creates a new, empty level.
     */
    LevelModel(void);

    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~LevelModel(void);
    
    
};


#endif /* LevelModel_h */
