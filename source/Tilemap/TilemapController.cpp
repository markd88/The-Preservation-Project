#include "TilemapController.h"
//using namespace MVC;

#pragma mark Main Functions
/** Creates the default model, view and tilemap vector. */
TilemapController::TilemapController() {
    _model = std::make_unique<TilemapModel>();
    _view = std::make_unique<TilemapView>(_model->position,
                                          _model->dimensions,
                                          _model->color,
                                          _model->tileSize);
    initializeTilemap();
 }

/**
 * Creates the model, view and tilemap vector.
 *
 * @param position      The center position of the tilemap
 * @param dimensions    The number of columns and rows in the tilemap
 * @param color         The background color of the tilemap
 * @param tileSize      The width and height of a tile
 */
TilemapController::TilemapController(Vec2 position, Vec2 dimensions,
                                     Color4 color, Size tileSize) {
    // TODO: Implement me
    Vec2 pos = centerToBottomLeftPosition(position, dimensions*tileSize);
    
    _model = std::make_unique<TilemapModel>(pos, dimensions, color, tileSize);
    _view = std::make_unique<TilemapView>(pos, dimensions, color, tileSize);

    initializeTilemap();
}

void TilemapController::init(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize) {
    
};

#pragma mark -
#pragma mark Model Methods
/**
 *  Updates the model and view with the position of this tilemap.
 *
 *  @param position The center of the tilemap
 */
void TilemapController::updatePosition(Vec2 position) {
    // TODO: Implement me
    Vec2 pos = centerToBottomLeftPosition(position, _model->dimensions * _model->tileSize);
    _model->setPosition(pos);
    _view->setPosition(pos);
}

/**
 *  Updates the model and view with the color of the tilemap.
 *
 *  @param color    The color of the tilemap
 */
void TilemapController::updateColor(Color4 color) {
    // TODO: Implement me
    _model->setColor(color);
    _view->setColor(color);
}

#pragma mark -
#pragma mark Controller Methods
/**
 * Adds a tile to (`col`, `row`) in the tilemap.
 *
 * The position is computed with the origin at the bottom right of
 * the tilemap.
 *
 * Precondition: The columns and rows must be at least 0 and at most
 * their corresponding `tileDimensions` - 1.
 *
 * @param col   The column to place the tile starting from left to right
 * @param row   The row to place the tile starting from bottom to top
 * @param color The color of the tile.
 * @param is_obs If the tile is obstacle
 */
void TilemapController::addTile(int col, int row, Color4 color, bool is_obs) {
    // TODO: Implement me
    if(col < 0 || row < 0 || col >= _model->dimensions.x || row >= _model->dimensions.y){
        return ;
    }
    Tile temp = std::make_unique<TileController>(Vec2(col * _model->tileSize.width, row * _model->tileSize.height), _model->tileSize, color, is_obs);
    
    _tilemap[row][col] = std::move(temp);
    
    _tilemap[row][col]->addChildTo(_view->getNode());
}

void TilemapController::addTile2(int col, int row, bool is_obs,
             const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey) {

    Vec2 pos;
//    pos = Vec2(col * _model->tileSize.width, row * _model->tileSize.height);
    
    int x = col*128;
    int y = 896 - row*128;
    pos = Vec2 (x,y);
    
    Tile temp = std::make_unique<TileController>(pos, _model->tileSize, is_obs, assets, textureKey);
    _tilemap[row][col] = std::move(temp);

    _tilemap[row][col]->addChildTo(_view->getNode());
};

void TilemapController::setTexture(const std::shared_ptr<cugl::AssetManager>& assets){
    for(auto& tile_vec : _tilemap){
        for(auto& tile : tile_vec){
            if(tile != nullptr){
                std::string textureKey = tile->getTextureKey();
                if (textureKey != "") {
                    tile->setTexture(assets, textureKey);
                }
            }
        }
    }
}

/**
 *  Updates the model and view with the dimensions of the tilemap.
 *
 *  Note this function will do nothing if any of the dimensions provided
 *  are negative.
 *
 *  @param dimensions   The number of columns and rows in the tilemap
 */
void TilemapController::updateDimensions(Vec2 dimensions) {
    // TODO: Implement me
    if(dimensions.x < 0 || dimensions.y < 0) return ;
    Tilemap temp_map;
    // create a temp map with new dimensions
    for(int i = 0; i < dimensions.y; i++) {
        std::vector<Tile> tileVec(dimensions.x);
        temp_map.push_back(std::move(tileVec));
    }
    // move the old existing tiles to the new map
    for(int i=0; i<_model->dimensions.y; i++){
        for(int j=0; j<_model->dimensions.x; j++){
            if(i < dimensions.y && j < dimensions.x){
                if(_tilemap[i][j] != nullptr) temp_map[i][j] = std::move(_tilemap[i][j]);
            }
        }
    }
    _tilemap = std::move(temp_map);
    _model->setDimensions(dimensions);
    _view->setSize(dimensions * _model->tileSize);
}

/**
 * Updates the size of all tiles in the tilemap.
 *
 * Note this function will do nothing if any of the sizes provided
 * are negative.
 *
 * @param tileSize  The width and height of a tile
 */
void TilemapController::updateTileSize(Size tileSize) {
    // TODO: Implement me
    if(tileSize.width < 0 || tileSize.height < 0) return ;
    
//    Vec2 center = bottomLeftToCenterPosition(_model->position, _model->tileSize*_model->dimensions);
    
    _model->setTileSize(tileSize);
    _view->setSize(_model->dimensions * tileSize);
    
//    Vec2 pos = centerToBottomLeftPosition(center, _model->tileSize*_model->dimensions);
    
    Vec2 pos = Vec2 (_model->position.x -128, _model->position.y -150);
    
    _model->setPosition(pos);
    _view->setPosition(pos);
    
    for(int i=0; i<_tilemap.size(); i++){
        for(int j=0; j<_tilemap[0].size(); j++){
            if(_tilemap[i][j] != nullptr){
                Vec2 temp = Vec2(j * _model->tileSize.height, i * _model->tileSize.width);
                _tilemap[i][j]->updatePosition(temp);
                
                _tilemap[i][j]->updateSize(tileSize);
            }
        }
    }
}

#pragma mark View Methods
/**
 * Adds the TilemapView as a child to the given scene.
 *
 * @param scene The scene to add the view to
 */
void TilemapController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    _view->addChildTo(scene);
}

/**
 * Removes the TilemapView child from the given scene.
 *
 * @param scene The scene to remove the view from
 */
void TilemapController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    _view->removeChildFrom(scene);
}

#pragma mark -
#pragma mark Provided Methods
/**
 *  Initializes the tilemap with empty tiles to match the current dimensions.
 *
 *  This function is implemented for you because it can be particularly
 *  tricky if you don't understand how to work with unique pointers. It
 *  is difficult to initialize a 2D vector of unique pointers since when
 *  adding rows to the vector, the compiler infers you're trying to copy
 *  this into the vector. Doing this will lead to an error about a `call
 *  to an implicitly deleted copy-constructor`. The copy constructor for
 *  unique pointers is deleted because they're unique! To instead directly
 *  move these pointers without copying them, use `std::move`.
 */
void TilemapController::initializeTilemap() {
    // TODO: change hard coded dimensions
    for(int i = 0; i < _model->dimensions.y; i++) {
        std::vector<Tile> tileVec(_model->dimensions.x);
        // The compiler infers that tileVec contains unique pointers so std::move must be used to avoid copys
        _tilemap.push_back(std::move(tileVec));
    }
}

/**
 *  Clears the tilemap of all tiles.
 *
 *  This function is implemented for you to ensure that templates are
 *  removed in an efficient manner. Here we remake the entire view rather
 *  than removing tiles manually. That is because removing a child
 *  from the scenegraph requires changing indices which makes this an
 *  O(n^2) operation. This is notably slow when working with the large
 *  procedural generation templates.
 */
void TilemapController::clearMap() {
    /// It is much more efficient to remake the view than to remove tiles one by one.
    auto scene = _view->getNode()->getScene();
    scene->removeChild(_view->getNode());
    _model = std::make_unique<TilemapModel>();
    _view = std::make_unique<TilemapView>(_model->position,
                                          _model->dimensions,
                                          _model->color,
                                          _model->tileSize);
    scene->addChild(_view->getNode());
    _tilemap.clear();
    initializeTilemap();
}
