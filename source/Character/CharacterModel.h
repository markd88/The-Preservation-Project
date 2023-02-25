//
//  CharacterModel.h
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef CharacterModel_h
#define CharacterModel_h

#include <cugl/cugl.h>
using namespace cugl;

class CharacterModel{
private:
    /** Center of the character */
    Vec2 _position;

public:
    /** Constructor */
    CharacterModel(){
        
    }
    
    void setPosition(Vec2 posi){
        _position = posi;
    }
    
};
#endif /* CharacterModel_h */
