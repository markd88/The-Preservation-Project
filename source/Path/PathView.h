//
//  PathView.h
//  Tilemap
//
//  Created by Yichen Qiu on 2/23/23.
//

#ifndef PathView_h
#define PathView_h
#include <cugl/cugl.h>


using namespace cugl;

class PathView{
private:
    
    std::vector<std::shared_ptr<scene2::PolygonNode>> _pathLines;
    Color4 _color;
    int _size;
    // attach to scene one by the other
    bool _inScene = true;
    
    std::shared_ptr<cugl::AssetManager> _assets;
    
public:
    PathView(std::vector<std::shared_ptr<scene2::PolygonNode>> pathLines, Color4 color, int size, std::shared_ptr<cugl::AssetManager>& assets){
        _pathLines = pathLines;
        _color = color;
        _size = size;
        _assets = assets;
    }
    
    void addToPathLines(Spline2 spline, Vec2 pos, const std::shared_ptr<cugl::Scene2>& scene){
        SplinePather splinePather = SplinePather();
        SimpleExtruder extruder = SimpleExtruder();

        
        splinePather.set(&spline);
        splinePather.calculate();
        
        extruder.set(splinePather.getPath());
        extruder.calculate(5);
        Poly2 line = extruder.getPolygon();
        
        std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
        polyNode->setPolygon(line);
        polyNode->setPosition(pos);
        polyNode->setColor(_color);
        

        std::shared_ptr<cugl::scene2::PolygonNode> brush = std::make_shared<cugl::scene2::PolygonNode>();
        brush->initWithTexture(_assets->get<Texture>("brush"));
        brush->setPosition(pos);
        brush->setScale(1.2);
        
//        _pathLines.push_back(polyNode);
//        if(_inScene)scene->addChild(polyNode);
        _pathLines.push_back(brush);
        if(_inScene)scene->addChild(brush);
        _inScene = !_inScene;
    }
    
    void setColor(Color4 color){
        _color = color;
    }
    
    void setSize(int size){
        _size = size;
    }
    
    void clearPathLines(){
        _pathLines.clear();
    }
    
    void removeChildren(const std::shared_ptr<cugl::Scene2>& scene){
        for (int i = 0; i < _pathLines.size(); i++){
            if(_pathLines[i]->getScene())scene->removeChild(_pathLines[i]);
        }
        _pathLines.clear();
    }
    
    void removeFirst(const std::shared_ptr<cugl::Scene2>& scene){
        if(_pathLines.size() > 0){
            if(_pathLines[0]->getScene()) scene->removeChild(_pathLines[0]);
            _pathLines.erase(_pathLines.begin());
        }
    }
    
};

#endif /* PathView_h */
