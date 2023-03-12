//
//  GuardSetController.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef __GUARDSET_CONTROLLER_H__
#define __GUARDSET_CONTROLLER_H__

#include "GuardSetModel.h"
#include "GuardSetView.h"
#include "Guard/GuardModel.h"
#include "Guard/GuardView.h"
#include "Guard/GuardController.h"

/**
 * A class communicating between the model and the view. It only
 * controls a single tile.
 */
class GuardSetController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<GuardSetModel> _model;
    /** View reference */
    std::unique_ptr<GuardSetView> _view;
    
    
#pragma mark External References
private:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<GuardController> Guard;
    typedef std::vector<std::vector<Guard>> GuardSet;
    GuardSet _guardSet;
    
#pragma mark Main Methods
public:
    /** Creates the default model, view and tilemap vector. */
    GuardSetController();
    
    /**
     * Creates the model, view and tilemap vector.
     *
     * @param position      The center position of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    GuardSetController(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize);
    
#pragma mark Update Methods
public:

    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updatePosition(Vec2 position);
    
    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updateDimensions(Vec2 dimensions);
    
    /**
     *  Updates the model and view with the size of this tile.
     *
     *  @param size  The bottom left corner of the tile
     */
    void updateSize(Size size);
    
    /**
     *  Updates the model and view with the color of this tile.
     *
     *  @param color The tile color
     */
    void updateColor(Color4 color);
    
    /**
     * Updates the size of all tiles in the tilemap.
     *
     * Note this function will do nothing if any of the sizes provided
     * are negative.
     *
     * @param tileSize  The width and height of a tile
     */
    void updateTileSize(Size tileSize);

    
#pragma mark View Methods
public:
    /**
     * Adds the TilemapView as a child to the given scene.
     *
     * @param scene The scene to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene);
    
    /**
     * Removes the TilemapView child from the given scene.
     *
     * @param scene The scene to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene);

    
#pragma mark Controller Methods
public:
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
     * @param color   The color of the tile.
     * @param is_obs if the tile is obstacle
     */
    void addTile(int col, int row, Color4 color, bool is_static);

};


#endif /* __GUARDSET_CONTROLLER_H__ */
