//
//  PathModel.h
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef PathModel_h
#define PathModel_h
#include <cugl/cugl.h>
using namespace cugl;

class PathModel {
private:
    /** Vector of splines for splinepather to extrude and add to _pathlines */
    std::vector<Vec2> _Path;
            
    Color4 _color;
    
    /** length of each spline segment */
    int _size;
    
    //** position of where the last spline was created*/
    Vec2 _lastPos;
    
public:
    /** A public accessible, read-only version of the color */
    const Color4& color;
    
    /** A public accessible, read-only version of the lastSplinePos */
    const Vec2& lastPos;
    
    /** length of each spline segment */
    const int& size;
    
    /** A public accessible, read-only version of the splinePath*/
    const  std::vector<Vec2>& Path;
    
    
    
#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param lastSplinePos  Position of where the last spline was created
     * @param size      The width and height of a tile
     * @param color     The tile color
     */
    PathModel(Color4 color, int size, Vec2 lastSplinePos, std::vector<Vec2> Path):
    color(_color),
    lastPos(_lastPos),
    size(_size),
    Path(_Path)
    {
        setColor(color);
        setSize(size);
        setLastPos(lastPos);
    }
    
#pragma mark Setters
public:
    /**
     *  Sets the bottom left corner of the tilemap
     *
     *  @param position Bottom left corner of the tilemap
     */
    void setColor(Color4 color) {
        _color = color;
    }
    
    void setSize(int size) {
        _size = size;
    }
    
    void setLastPos(Vec2 lastPos) {
        _lastPos = lastPos;
    }
    
    void addToPath(Vec2 point){
        _Path.push_back(point);
    }
    
    void clearPath(){
        _Path.clear();
    }
    
    void removeAfterIndex(int index){
        _Path.erase(_Path.begin() + index + 1, _Path.end());
    }
    
};

#endif /* PathModel_h */

