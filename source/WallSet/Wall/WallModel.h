//
//  WallModel.h
//  Tilemap
//
//  Created by Hao Chen on 4/15/23.
//

#ifndef WallModel_h
#define WallModel_h

#include <cugl/cugl.h>
using namespace cugl;

class WallModel {
private:
    /** Center of the character */
    Vec2 _position;
    Size _size;
    bool _isResource;
    
    std::string _textureKey;
    int radius;

    
#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param position  The bottom left corner of the Wall
     * @param size      The width and height of a Wall
     * @param color     The Wall color
     */
    WallModel(Vec2 position, Size size, bool isResource, std::string textureKey){
        setPosition(position);
        setSize(size);
        setType(isResource);
        setTextureKey(textureKey);
    }
    
#pragma mark Setters
public:

    Vec2 getPosition() {
        return _position;
    }
    
    std::string getTextureKey() {
        return _textureKey;
    }

    /**
     *  Sets the position of the bottom left corner of the Wall.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }

    /**
     *  Sets the size of the Wall.
     *
     *  @param size Width and height of an Wall
     */
    void setSize(Size size) {
        _size = size;
    }
    
    bool isResource(){
        return _isResource;
    }

    /**
     *  Sets the type of this item, Wall or resource.
     *
     *  @param isResource The type of the item
     */
    void setType(bool isResource) {
        _isResource = isResource;

    }
    
    void setTextureKey(std::string textureKey) {
        this->_textureKey = textureKey;
    }
};


#endif /* WallModel_h */
