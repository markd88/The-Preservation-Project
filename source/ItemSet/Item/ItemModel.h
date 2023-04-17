//
//  ItemModel.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef ItemModel_h
#define ItemModel_h

#include <cugl/cugl.h>
using namespace cugl;

class ItemModel {
private:
    /** Center of the character */
    Vec2 _position;
    Size _size;
    bool _isArtifact;
    bool _isResource;
    bool _isObs;
    
    std::string _textureKey;
    int radius;

//public:
//    /** A public accessible, read-only version of the color */
//    const Color4& color;
    
#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param position  The bottom left corner of the Item
     * @param size      The width and height of a Item
     * @param color     The Item color
     */
    ItemModel(Vec2 position, Size size, bool isArtifact, bool isResource, bool isObs, std::string textureKey){
        setPosition(position);
        setSize(size);
        setArtifact(isArtifact);
        setResource(isResource);
        setObs(isObs);
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
     *  Sets the position of the bottom left corner of the Item.
     *
     *  @param position Bottom left corner of tile
     */
    void setPosition(Vec2 position) {
        _position = position;
    }

    /**
     *  Sets the size of the Item.
     *
     *  @param size Width and height of an Item
     */
    void setSize(Size size) {
        _size = size;
    }
    
    bool isArtifact(){
        return _isArtifact;
    }
    
    bool isResource(){
        return _isResource;
    }
    
    bool isObs(){
        return _isObs;
    }
    
    void setArtifact(bool isArtifact) {
        _isArtifact = isArtifact;

    }

    /**
     *  Sets the type of this item, Item or resource.
     *
     *  @param isResource The type of the item
     */
    void setResource(bool isResource) {
        _isResource = isResource;

    }
    
    void setObs(bool isObs) {
        _isObs = isObs;

    }
    
    void setTextureKey(std::string textureKey) {
        this->_textureKey = textureKey;
    }
};

#endif /* ItemModel_h */
