//
//  CharacterView.h
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef CharacterView_h
#define CharacterView_h
#include <cugl/cugl.h>
using namespace cugl;

class CharacterView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::PolygonNode> _node;
    
public:
    /** contructor */
    CharacterView(){
        
    }
    
    
    ~CharacterView(){
        
    }
    
    void setPosition(Vec2 position){
        
    }
    
};


#endif /* CharacterView_h */
