//
//  CharacterModel.h
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef CharacterModel_h
#define CharacterModel_h
#include <cugl/cugl.h>
using namespace cugl;


#include <cugl/cugl.h>
using namespace cugl;

class CharacterModel{
private:
    /** Center of the character */
    Vec2 _position;
    Size _size;
    Color4 _color;
    
    /** Number of Artifacts collected */
    int _n_art;
    
    /** Number of Resources collected */
    int _n_res;
    
    /** The radius of the character in which a touch will initiate the path creation*/
    int radius;
    
public:
    /** A public accessible, read-only version of the color */
    const Color4& color;

#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    CharacterModel(Vec2 position, Size size, Color4 color): color(_color) {
        setPosition(position);
        radius = 15 * 4;
        _n_art = 0;
        _n_res = 0;
    }

#pragma mark Setters & Getteres
public:
    
    Vec2 getPosition(){
        return _position;
    }
    
    /**
     *  Sets the position of the bottom left corner of the tile.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }
    
    
    int getNumRes(){
        return _n_res;
    }
    
    void setNumRes(int n){
        _n_res = n;
    }
    
    int getNumArt(){
        return _n_art;
    }
    
    void setNumArt(int n){
        _n_art = n;
    }


    
#pragma mark Helpers
public:
    /**
     *  See if the touch point is within the character
     *
     *  @param point The position of the touchpoint
     */
    bool contains(Vec2 point){
        if(_position.distance(point) <= radius){
            return true;
        }
        return false;
    }
    
    
    
};
#endif /* CharacterModel_h */
