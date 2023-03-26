//
//  GuardModel.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef GuardModel_h
#define GuardModel_h

#include <cugl/cugl.h>
using namespace cugl;

class GuardModel {
private:
    /** Center of the character */
    Vec2 _position;
    Size _size;
    Color4 _color;
    bool _isStatic;
    int radius;

public:
    /** A public accessible, read-only version of the color */
    const Color4& color;
    
#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param position  The bottom left corner of the Artifact
     * @param size      The width and height of a Artifact
     * @param color     The Artifact color
     */
    GuardModel(Vec2 position, Size size, Color4 color) : color(_color) {
        setPosition(position);
        setSize(size);
    }
    
#pragma mark Setters
public:
    
    Vec2 getPosition() {
        return _position;
    }
    
    /**
     *  Sets the position of the bottom left corner of the Artifact.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }
    
    /**
     *  Sets the size of the Artifact.
     *
     *  @param size Width and height of an Artifact
     */
    void setSize(Size size) {
        _size = size;
    }

};

#endif /* GuardModel_h */
