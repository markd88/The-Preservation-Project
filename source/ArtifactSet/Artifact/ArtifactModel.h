//
//  ArtifactModel.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef ArtifactModel_h
#define ArtifactModel_h

#include <cugl/cugl.h>
using namespace cugl;

class ArtifactModel {
private:
    /** Center of the character */
    Vec2 _position;
    Size _size;
    bool _isResource;
    
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
     * @param position  The bottom left corner of the Artifact
     * @param size      The width and height of a Artifact
     * @param color     The Artifact color
     */
    ArtifactModel(Vec2 position, Size size, bool isResource, std::string textureKey){
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
    
    bool isResource(){
        return _isResource;
    }

    /**
     *  Sets the type of this item, artifact or resource.
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

#endif /* artifactModel_h */
