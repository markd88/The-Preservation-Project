//
//  PathController.hpp
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef PathController_hpp
#define PathController_hpp

#include <stdio.h>
#include "PathModel.h"
#include "PathView.h"

class PathController{
    
#pragma mark Internal References
public:
    /** Model reference */
    std::unique_ptr<PathModel> _model;
    /** View reference */
    std::unique_ptr<PathView> _view;
    /** Whether or not path should be drawing*/
    bool _isDrawing;
    
    /** Whether a path is initiating: player starts to draw the path but the touch still too close to character
     *  This is an appoach to address the issue that the character will flash to the initial touchpoint
     */
    bool _isInitiating;
    
public:
    /** A public accessible, read-only version of the fields*/
    const bool& isDrawing;
    const bool& isInitiating;
    
#pragma mark Main Methods
public:
    
    /**
     * Creates a standard PathController with color set to black and line segments of length 15
     *
     * @param batch   The spritebatch used for drawing segments
     */
    
    PathController():
    isDrawing(_isDrawing),
    isInitiating(_isInitiating)
    {
        std::vector<cugl::Vec2> Path;
        std::vector<std::shared_ptr<scene2::PolygonNode>> pathLines;
        _isDrawing = false;
        _isInitiating = false;
        _model = std::make_unique<PathModel>(Color4::BLACK, 50, Vec2::ZERO, Path);
        _view = std::make_unique<PathView>(pathLines, Color4::BLACK, 8);
    }
    
#pragma mark Update Methods
public:
    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updateColor(Color4 color) {
        _model->setColor(color);
        _view->setColor(color);
    }
    
    void updateSize(int size){
        _model->setSize(size);
        _view->setSize(size);
    }
    
    /**
     *  Updates the the location of where the last spline was created
     *
     *  @param position  The bottom left corner of the tile
     */
    void updateLastPos(Vec2 lastPos){
        _model->setLastPos(lastPos);
    }
    
    // now only remove from scene
    void clearPath(){
        _model->clearPath();
    }
    
    void removeFrom(const std::shared_ptr<cugl::Scene2>& scene){
        _view->removeChildren(scene);
    }
    
    void setIsDrawing(bool isDrawing){
        _isDrawing = isDrawing;
    }
    
    // for initial segments of the path from character
    void setIsInitiating(bool isInitiating){
        _isInitiating = isInitiating;
    }
    
    std::vector<Vec2> getPath(){
        return _model->Path;
    }
    
    int getSize(){
        return _model->size;
    }
    
    Vec2 getLastPos(){
        return _model->lastPos;
    }
    
    
#pragma mark Path Creation Methods
    
    /**
     *  Updates the the location of where the last spline was created
     *
     *  @param position  The bottom left corner of the tile
     */
    
    void addSegment(Vec2 pos, const std::shared_ptr<cugl::Scene2>& scene){
        Spline2 spline = Spline2(_model->lastPos, pos);
        Vec2 point = _model->lastPos.getMidpoint(pos);
        _model->addToPath(point);
        if (_model->Path.size() % 2 == 0){
            _view->addToPathLines(spline, point, scene);
        }
        updateLastPos(pos);
    }
    
        
    bool farEnough(Vec2 pos) {
        return (_model->lastPos.distance(pos) > _model->size);
    }
    
    void addSegments(Vec2 pos, const std::shared_ptr<cugl::Scene2>& scene){
        // a loop to handle more than one segments in one frame
        while(farEnough(pos)){
            Vec2 checkpoint = _model->lastPos + (pos - _model->lastPos) / _model->lastPos.distance(pos) * _model->size;
            addSegment(checkpoint, scene);
        }
    }
    
    // remove first segment in both model and view
    void removeFirst(const std::shared_ptr<cugl::Scene2>& scene){
        _model->removeFirst();
        // _view->removeFirst(scene);
    }
    
    
};

#endif /* PathController_hpp */
