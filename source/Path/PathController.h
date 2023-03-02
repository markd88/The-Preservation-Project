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
    
#pragma mark Main Methods
public:
    /**
     * Creates a standard PathController with color set to black and line segments of length 15
     *
     * @param batch   The spritebatch used for drawing segments
     */
    
    PathController(){
        std::vector<cugl::Vec2> Path;
        std::vector<std::shared_ptr<scene2::PolygonNode>> pathLines;
        _isDrawing = false;
        _model = std::make_unique<PathModel>(Color4::BLACK, 15, Vec2::ZERO, Path);
        _view = std::make_unique<PathView>(pathLines);
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
    }
    
    void updateSize(int size){
        _model->setSize(size);
    }
    
    /**
     *  Updates the the location of where the last spline was created
     *
     *  @param position  The bottom left corner of the tile
     */
    void updateLastPos(Vec2 lastPos){
        _model->setLastPos(lastPos);
    }
    
    void clearPath(const std::shared_ptr<cugl::Scene2>& scene){
        _model->clearPath();
        _view->removeChildren(scene);
    }
    
    void setIsDrawing(bool isDrawing){
        _isDrawing = isDrawing;
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
        _view->addToPathLines(spline, point, scene);
        updateLastPos(pos);
    }
    
        
    bool farEnough(Vec2 pos){
        return (_model->lastPos.distance(pos) > _model->size);
    }
    
    void addSegments(Vec2 pos, const std::shared_ptr<cugl::Scene2>& scene){
        // a loop to handle more than one segments in one frame
        while(farEnough(pos)){
            Vec2 checkpoint = _model->lastPos + (pos - _model->lastPos) / _model->lastPos.distance(pos) * _model->size;
            addSegment(checkpoint, scene);
        }
    }
    
    
    
};

#endif /* PathController_hpp */
