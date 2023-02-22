//
//  CMVTilemapController.cpp
//  TileMap Lab
//
//  This module provides the CMV version of the TilemapController class.
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#include "CMVTilemapController.h"
using namespace CMV;

#pragma mark Main Functions
/** Creates the default model and tilemap vector. */
TilemapController::TilemapController() {
    _model = std::make_unique<TilemapModel>();
    initializeTilemap();
}

/**
 * Creates the model and tilemap vector.
 *
 * Use {@link centerToBottomLeftPosition} to figure out the left
 * corner of the tilemap.
 *
 * @param position      The center position of the tilemap
 * @param dimensions    The number of columns and rows in the tilemap
 * @param color         The background color of the tilemap
 * @param tileSize      The width and height of a tile
 */
TilemapController::TilemapController(Vec2 position, Vec2 dimensions,
                                     Color4 color, Size tileSize) {
    // TODO: Implement me
    _model = std::make_unique<TilemapModel>(position, dimensions, color, tileSize);
    Vec2 pos = centerToBottomLeftPosition(position, _model->getNode()->getSize());
    _model->setPosition(pos);
    initializeTilemap();
    
}

#pragma mark -
#pragma mark Model Methods
/**
 * Updates the position of this tilemap by updating the model.
 *
 * @param position The center of the tilemap
 */
void TilemapController::updatePosition(Vec2 position) {
    // TODO: Implement me
    Vec2 pos = centerToBottomLeftPosition(position, _model->dimensions * _model->tileSize);
    _model->setPosition(pos);
}

/**
 * Updates the color of the tilemap by updating the model.
 *
 * @param color    The color of the tilemap
 */
void TilemapController::updateColor(Color4 color) {
    // TODO: Implement me
    _model->setColor(color);
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
 */
void TilemapController::addTile(int col, int row, Color4 color) {
    // TODO: Implement me
    if(col < 0 || row < 0 || col >= _model->dimensions.x || row >= _model->dimensions.y){
        return ;
    }
    Tile temp = std::make_unique<TileController>(Vec2(col * _model->tileSize.width, row * _model->tileSize.height), _model->tileSize, color);
    
    _tilemap[row][col] = std::move(temp);
    
    _tilemap[row][col]->addChildTo(_model->getNode());
}

/**
 * Inverts the color of the tilemap and it's tiles.
 *
 * Examples:
 *      Inverting white (255, 255, 255, 0) gives black (0, 0, 0, 0)
 *      Inverting red (255, 0, 0, 0) gives cyan (0, 255, 255, 0)
 *      Inverting light purple (150, 100, 200, 0) gives a dull green
 *          (105, 155, 55, 0)
 */
void TilemapController::invertColor() {
    // TODO: Implement me
    for(int i=0; i<_tilemap.size(); i++){
        for(int j=0; j<_tilemap[0].size(); j++){
            if(_tilemap[i][j] != nullptr){
                _tilemap[i][j]->invertColor();
            }
        }
    }
    
    Color4 old_color = _model->color;
    Color4 new_color = old_color.complement();
    this->updateColor(new_color);
    
}

/**
 * Modifies the current number of columns and rows by
 *
 * The values are modified by `colIncrement` and `rowIncrement`,
 *  respectively. The values can be negative.
 *
 * @param colIncrement The number of columns to increment
 * @param rowIncrement The number of rows to increment
 */
void TilemapController::modifyDimensions(int colIncrement, int rowIncrement) {
    // TODO: Implement me
    //std::cout<<_model->dimensions.toString()<<std::endl;
    Vec2 dim(_model->dimensions.x + colIncrement, _model->dimensions.y + rowIncrement);
    updateDimensions(dim);
    //std::cout<<_model->dimensions.toString()<<std::endl;
    
}

/**
 * Modifies the current width and height by `xFactor` and `yFactor`.
 *
 * @param xFactor The factor to multiply the current width by
 * @param yFactor The factor to multiply the current height by
 */
void TilemapController::modifyTileSize(float xFactor, float yFactor) {
    // TODO: Implement me
    Size s (_model->tileSize.width * xFactor, _model->tileSize.height * yFactor);
    updateTileSize(s);
}

/**
 * Updates the model and view with the dimensions of the tilemap.
 *
 * Note this function will do nothing if any of the dimensions provided
 * are negative.
 *
 * @param dimensions   The number of columns and rows in the tilemap
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
}

/**
 * Updates the size of all tiles in the tilemap and the model.
 *
 * Note this function will do nothing if any of the sizes provided
 * are negative. This function will also ensure that the tilemap
 * remains centered about it's original center position.
 *
 * @param tileSize  The width and height of a tile
 */
void TilemapController::updateTileSize(Size tileSize) {
    // TODO: Implement me
    if(tileSize.width < 0 || tileSize.height < 0) return ;
    
    Vec2 center = bottomLeftToCenterPosition(_model->position, _model->tileSize*_model->dimensions);
    _model->setTileSize(tileSize);
    
    Vec2 pos = centerToBottomLeftPosition(center, _model->tileSize*_model->dimensions);
    _model->setPosition(pos);
    
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

#pragma mark -
#pragma mark View Methods
/**
 * Adds the TilemapView as a child to the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to add the view to
 */
void TilemapController::addChildTo(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    scene->addChild(_model->getNode());
}

/**
 * Removes the TilemapView child from the given `scene`.
 *
 * The TilemapView is the view reference held within the model.
 *
 * @param scene The scene to remove the view from
 */
void TilemapController::removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene) {
    // TODO: Implement me
    scene->removeChild(_model->getNode());
}

#pragma mark -
#pragma mark Provided Methods
/**
 * Converts position from a center position to a bottom left position
 *
 * @param position Center of the tilemap
 * @param size     Size of the tilemap
 *
 * @returns The bottom left of the tilemap
 */
void TilemapController::initializeTilemap() {
    for(int i = 0; i < _model->dimensions.y; i++) {
        std::vector<Tile> tileVec(_model->dimensions.x);
        // The compiler infers that tileVec contains unique pointers
        // so std::move must be used to avoid copys
        _tilemap.push_back(std::move(tileVec));
    }
}

/**
 * Clears the tilemap of all tiles.
 *
 * This function is implemented for you to ensure that templates are
 * removed in an efficient manner. Here we remake the entire view rather
 * than removing tiles manually. That is because removing a child
 * from the scenegraph requires changing indices which makes this an
 * O(n^2) operation. This is notably slow when working with the large
 * procedural generation templates.
 */ 
void TilemapController::clearMap() {
    auto scene = _model->getNode()->getScene();
    scene->removeChild(_model->getNode());
    _model = std::make_unique<TilemapModel>();
    scene->addChild(_model->getNode());
    _tilemap.clear();
    initializeTilemap();
}

