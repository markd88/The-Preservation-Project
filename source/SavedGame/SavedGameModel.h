//
//  SavedGameModel.h
//  The Preservation Project
//
//  Created by Ann Zhou on 4/27/23.
//

#ifndef SavedGameModel_h
#define SavedGameModel_h

class SavedGameModel : public Asset {
protected:
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

    /**
     * Loads a single wall object
     *
     * The wall will be retained and stored in the vector _walls.  If the
     * wall fails to load, then it will not be added to _walls.
     *
     * @param  reader   a JSON reader with cursor ready to read the wall
     *
     * @retain the wall
     * @return true if the wall was successfully loaded
     */
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
    static std::shared_ptr<SavedGameModel> alloc() {
        std::shared_ptr<SavedGameModel> result = std::make_shared<SavedGameModel>();
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
    static std::shared_ptr<SavedGameModel> alloc(std::string file) {
        std::shared_ptr<SavedGameModel> result = std::make_shared<SavedGameModel>();
        return (result->init(file) ? result : nullptr);
    }

#pragma mark Model Access
    
    /** Get world map */
    std::shared_ptr<TilemapController> getWorld() {return _world;};
    std::shared_ptr<ItemSetController> getItem() {return _item->copy();};
    std::shared_ptr<ItemSetController> getObs() {return _obs;};
    std::shared_ptr<ItemSetController> getWall() {return _wall;};
    cugl::Vec2 getCharacterPos() {return _characterPos;};
    std::vector<std::vector<cugl::Vec2>> getMovingGuardsPos() {return _movingGuardsPos;};
    std::vector<std::vector<int>> getStaticGuardsPos() {return _staticGuardsPos;};

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


    //#pragma mark -
    //#pragma mark Initializers
    /**
     * Creates a new, empty level.
     */
    SavedGameModel(void);

    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~SavedGameModel(void);
    
    
};


#endif /* SavedGameModel_h */
