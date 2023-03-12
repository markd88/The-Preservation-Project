//
//  GuardSetModel.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef GuardSetModel_h
#define GuardSetModel_h
#include <cugl/cugl.h>
using namespace cugl;

class GuardSetModel{
    
#pragma mark State
private:
    /** Bottom left corner of the tilemap */
    Vec2 _position;
    /** The number of tiles per column and row   */
    Vec2 _dimensions;
    /** Tilemap color */
    Color4 _color;
    /** Width and height of a single tile */
    Size _tileSize;
    
public:
    /// Instead of writing getters for private states that need setters, you
    /// can create constant references to force users of this class to modify
    /// state through setters
    const Vec2& position;
    const Vec2& dimensions;
    const Color4& color;
    const Size& tileSize;
    
#pragma mark Main Functions
public:
    /**
     *  Creates the default tilemap.
     *
     *  The default tilemap is a black square whose bottom left corner is
     *  aligned with the bottom left corner of the screen. It's dimensions
     *  are 10 by 10 with tiles of size 5 by 5.
     */
    GuardSetModel():
    position(_position),
    dimensions(_dimensions),
    color(_color),
    tileSize(_tileSize) {
        setPosition(Vec2(0,0));
        setDimensions(Vec2(10, 10));
        setColor(Color4::BLACK);
        setTileSize(Size(5,5));
    }
    
    /**
     * Creates the model state.
     *
     * @param position      The bottom left corner of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    GuardSetModel(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize):
    position(_position),
    dimensions(_dimensions),
    color(_color),
    tileSize(_tileSize) {
        setPosition(position);
        setDimensions(dimensions);
        setColor(color);
        setTileSize(tileSize);
    }

#pragma mark Setters
public:

    /**
     *  Sets the position of the bottom left corner of the Artifact.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }
    
    /**
     *  Sets the dimensions of the guard set
     *
     *  @param position Bottom left corner of tile
     */
    void setDimensions(Vec2 dimensions) {
        _dimensions = dimensions;
    }
    
    /**
     *  Sets the size of the Artifact.
     *
     *  @param size Width and height of an Artifact
     */
    void setTileSize(Size tileSize) {
        _tileSize = tileSize;
    }
    
    /**
     *  Sets the color of the Artifact.
     *
     *  @param color The color of the Artifact
     */
    void setColor(Color4 color) {
        _color = color;
    }
    

    
};

#endif /* GuardSetModel_h */
