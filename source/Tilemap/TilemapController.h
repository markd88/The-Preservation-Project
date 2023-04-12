#ifndef __TILEMAP_CONTROLLER_H__
#define __TILEMAP_CONTROLLER_H__
// These are all in the same directory
#include "TilemapModel.h"
#include "TilemapView.h"
// This is NOT in the same directory
#include <Tile/TileController.h>
#include <memory>

//namespace MVC {
/**
 * A class communicating between the model and the view. It controls
 * the entire tile map.
 */
class TilemapController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<TilemapModel> _model;
    /** View reference */
    std::unique_ptr<TilemapView> _view;
    
#pragma mark External References
private:
    /** Tilemape is a 2D vector list of tiles */
    typedef std::unique_ptr<TileController> Tile;
    typedef std::vector<std::vector<Tile>> Tilemap;
    Tilemap _tilemap;
    int _vertices; //number of vertices for adjaceny matrix
    std::unordered_map<int, Vec2> _nodes; //nodes for the matrix
    
#pragma mark Main Methods
public:
    /** Creates the default model, view and tilemap vector. */
    TilemapController();
    
    /**
     * Creates the model, view and tilemap vector.
     *
     * @param position      The center position of the tilemap
     * @param dimensions    The number of columns and rows in the tilemap
     * @param color         The background color of the tilemap
     * @param tileSize      The width and height of a tile
     */
    TilemapController(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize);
    
    void init(Vec2 position, Vec2 dimensions, Color4 color, Size tileSize);
    
#pragma mark Model Methods
public:
    /**
     * Updates the model and view with the position of this tilemap.
     *
     * @param position The center of the tilemap
     */
    void updatePosition(Vec2 position);
    
    /**
     * Updates the model and view with the dimensions of the tilemap.
     *
     * Note this function will do nothing if any of the dimensions provided
     * are negative.
     *
     * @param dimensions   The number of columns and rows in the tilemap
     */
    void updateDimensions(Vec2 dimensions);
    
    /**
     * Updates the model and view with the color of the tilemap.
     *
     * @param color    The color of the tilemap
     */
    void updateColor(Color4 color);
    
    /**
     * Updates the size of all tiles in the tilemap.
     *
     * Note this function will do nothing if any of the sizes provided
     * are negative.
     *
     * @param tileSize  The width and height of a tile
     */
    void updateTileSize(Size tileSize);
    
#pragma mark View Methods
public:
    /**
     * Adds the TilemapView as a child to the given scene.
     *
     * @param scene The scene to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::Scene2>& scene);
    
    /**
     * Removes the TilemapView child from the given scene.
     *
     * @param scene The scene to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::Scene2>& scene);
    
#pragma mark Controller Methods
public:
    /**
     * Adds a tile to (`col`, `row`) in the tilemap.
     *
     * The position is computed with the origin at the bottom right of
     * the tilemap.
     *
     * Precondition: The columns and rows must be at least 0 and at most
     * their corresponding `tileDimensions` - 1.
     *
     * @param col   The column to place the tile starting from left to right
     * @param row   The row to place the tile starting from bottom to top
     * @param color   The color of the tile.
     * @param is_obs if the tile is obstacle
     */
    void addTile(int col, int row, Color4 color, bool is_obs);

    void addTile2(int col, int row, bool is_obs,
                 const std::shared_ptr<cugl::AssetManager>& assets, std::string textureKey);

    void setTexture(const std::shared_ptr<cugl::AssetManager>& assets);
    std::string getTextureKey();
    /**
     * Inverts the color of the tilemap and it's tiles.
     *
     * Examples:
     *      Inverting white (255, 255, 255, 0) gives black (0, 0, 0, 0)
     *      Inverting red (255, 0, 0, 0) gives cyan (0, 255, 255, 0)
     *      Inverting light purple (150, 100, 200, 0) gives a dull green
     *          (105, 155, 55, 0)
     */
    void invertColor();
    
    /**
     * Modifies the current number of columns and rows by
     *
     * The values are modified by `colIncrement` and `rowIncrement`,
     *  respectively. The values can be negative.
     *
     * @param colIncrement The number of columns to increment
     * @param rowIncrement The number of rows to increment
     */
    void modifyDimensions(int colIncrement, int rowIncrement);
    
    /**
     * Modifies the current width and height by `xFactor` and `yFactor`.
     *
     * @param xFactor The factor to multiply the current width by
     * @param yFactor The factor to multiply the current height by
     */
    void modifyTileSize(float xFactor, float yFactor);
    
    /**
     * Clears the tilemap of all tiles.
     *
     * This function is implemented for you to ensure that templates are
     * removed in an efficient manner. Here we remake the entire view rather
     * than removing tiles manually. That is because removing a child
     * from the scenegraph requires changing indices which makes this an
     * O(n^2) operation. This is notably slow when working with the large
     * procedural generation templates.
     */
    void clearMap();
    
#pragma mark Helpers
    /**
     * Converts position from a center position to a bottom left position
     *
     * @param position Center of the tilemap
     * @param size     Size of the tilemap
     *
     * @returns The bottom left of the tilemap
     */
    Vec2 centerToBottomLeftPosition(Vec2 position, Size size) {
        return position - size / 2;
    }
    
    /**
     * Converts position from a bottom left position to a center position
     *
     * @param position Bottom left of the tilemap
     * @param size     Size of the tilemap
     *
     * @returns The center of the tilemap
     */
    Vec2 bottomLeftToCenterPosition(Vec2 position, Size size) {
        return position + size / 2;
    }
    
    /**
     * Initializes the tilemap with empty tiles to match the current dimensions.
     *
     * This function is implemented for you because it can be particularly
     * tricky if you don't understand how to work with unique pointers. It
     * is difficult to initialize a 2D vector of unique pointers since when
     * adding rows to the vector, the compiler infers you're trying to copy
     * this into the vector. Doing this will lead to an error about a `call
     * to an implicitly deleted copy-constructor`. The copy constructor for
     * unique pointers is deleted because they're unique! To instead directly
     * move these pointers without copying them, use `std::move`.
     */
    void initializeTilemap();
    
    Size getSize(){
        Size size = _model->dimensions * _model->tileSize;
        return size;
    }
    
    const std::shared_ptr<scene2::PolygonNode>& getNode() const {
        // TODO: Implement me
        return _view->getNode();
    }
    
    

#pragma mark Helpers
    /**
     * Check if the point is located in some obstacle tile
     * @Param Point    position of the point
     */
    bool inObstacle(Vec2 point){
        for(auto& tile_vec : _tilemap){
            for(auto& tile : tile_vec){
            
                if(tile != nullptr && tile->is_obs() && tile->contains(point)){
                    return true;
                }
            }
        }
        return false;
    }
    
    bool lineInObstacle(Vec2 a, Vec2 b){
        for(auto& tile_vec : _tilemap){
            for(auto& tile : tile_vec){
                if(tile != nullptr && tile->is_obs() && tile->containsLine(a,b)){
                    return true;
                }
            }
        }
        return false;
    }
    
    void makePreview(Vec2 point){
        removePreview();
        for(auto& tile_vec : _tilemap){
            for(auto& tile : tile_vec){
                if(tile != nullptr && tile->contains(point)){
                    tile->setVisibility(false);
                }
            }
        }
    }
    
    void removePreview(){
        for(auto& tile_vec : _tilemap){
            for(auto& tile : tile_vec){
                if(tile != nullptr){
                    tile->setVisibility(true);
                }
            }
        }
    }
    
    void setVisibility(bool active){
        _view->setVisibility(active);
        _model->setActive(active);
    }
    
    bool isActive(){
        return _model->isActive();
    }
    
    void setActive(bool active){
        _model->setActive(active);
    }
    
    std::vector<std::pair<int,int>> getEdges(const std::shared_ptr<cugl::Scene2>& scene){
        std::unordered_map<int, Vec2> nodes;
        std::vector<std::pair<int,int>> edges;
        int count = 0;
        
        int width = _model->dimensions.x * _model->tileSize.width;
        int edgeLength = width / 30;
        
        int numPerRow = (width/edgeLength) + 1;
        
        auto& start = _tilemap[0][0];
        Vec2 startPos = start->getPosition() + Vec2(0, start->getSize().y);
        
        for (int j = startPos.y; j > 0; j -= edgeLength){
            for (int i = startPos.x; i < startPos.x + width; i += edgeLength){
                nodes[count] = Vec2(i,j);
                count += 1;
            }
        }
        
        SplinePather splinePather = SplinePather();
        SimpleExtruder extruder = SimpleExtruder();
        
        for (int i = 0; i < count - 1; i++){
            if ( (i+1) % (numPerRow) != 0){
                Vec2 a = nodes[i];
                Vec2 b = nodes[i + 1];
                bool hitObs = lineInObstacle(a, b);
                
                if (hitObs == false){
                    edges.push_back(std::make_pair(i, i+1));
                }
                /*
                Spline2 spline = Spline2(a, b);
                splinePather.set(&spline);
                splinePather.calculate();
                
                extruder.set(splinePather.getPath());
                extruder.calculate(1);
                Poly2 line = extruder.getPolygon();
                
                std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
                polyNode->setPolygon(line);
                polyNode->setPosition(a.getMidpoint(b));
                scene->addChild(polyNode);
                 */
                 
            }
            
        }
        
        for (int i = 0; i < count - numPerRow; i++){
            splinePather.clear();
            extruder.clear();
            
            Vec2 a = nodes[i];
            Vec2 b = nodes[i + numPerRow];
            bool hitObs = lineInObstacle(a, b);
            
            if (hitObs == false){
                edges.push_back(std::make_pair(i, i + numPerRow));
            }
            /*
            Spline2 spline = Spline2(a, b);
            splinePather.set(&spline);
            splinePather.calculate();
            
            extruder.set(splinePather.getPath());
            extruder.calculate(1);
            Poly2 line = extruder.getPolygon();
            
            std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
            polyNode->setPolygon(line);
        
            polyNode->setPosition(a.getMidpoint(b));
            scene->addChild(polyNode);
             */
            
        }
        
        _vertices = count;
        _nodes = nodes;
        return edges;
        
    }
    
    int getVertices(){
        return _vertices;
    }
    
    std::unordered_map<int, Vec2> getNodes(){
        return _nodes;
    }
    
    void addPoints1(const std::shared_ptr<cugl::Scene2>& scene){
        
        auto origin = scene2::PolygonNode::alloc();
        std::unordered_map<int, Vec2> nodes;
        int count = 0;
        int x = _model->dimensions.x;
        
        for(auto& tile_vec : _tilemap){
            for(auto& tile : tile_vec){
                origin = scene2::PolygonNode::alloc();
                origin->setPolygon(Rect(10, 10, 10, 10));
                int i = tile->getPosition().x;
                int j = tile->getPosition().y;
                origin->setPosition(Vec2(i,j));
                origin->setColor(Color4::RED);
                scene->addChild(origin);
                nodes[count] = Vec2(i,j);
                count += 1;
            }
                
        }
                
        SplinePather splinePather = SplinePather();
        SimpleExtruder extruder = SimpleExtruder();
        
        for (int i = 0; i < count - 1; i++){
            if ((i + 1) % x != 0){
                Vec2 a = nodes[i];
                Vec2 b = nodes[i + 1];
                bool hitObs = lineInObstacle(a, b);
                Spline2 spline = Spline2(a, b);
                splinePather.set(&spline);
                splinePather.calculate();
                
                extruder.set(splinePather.getPath());
                extruder.calculate(1);
                Poly2 line = extruder.getPolygon();
                
                std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
                polyNode->setPolygon(line);
                if (hitObs){
                    polyNode->setColor(Color4::BLUE);
                }else{
                    polyNode->setColor(Color4::GREEN);
                }
                polyNode->setPosition(a.getMidpoint(b));
                scene->addChild(polyNode);
            }
            
        }
        for (int i = 0; i < count - x; i++){
            
                Vec2 a = nodes[i];
                Vec2 b = nodes[i + x];
                bool hitObs = lineInObstacle(a, b);
                Spline2 spline = Spline2(a, b);
                splinePather.set(&spline);
                splinePather.calculate();
                
                extruder.set(splinePather.getPath());
                extruder.calculate(1);
                Poly2 line = extruder.getPolygon();
                
                std::shared_ptr<scene2::PolygonNode> polyNode= scene2::PolygonNode::alloc();
                polyNode->setPolygon(line);
                if (hitObs){
                    polyNode->setColor(Color4::BLUE);
                }else{
                    polyNode->setColor(Color4::GREEN);
                }
                polyNode->setPosition(a.getMidpoint(b));
                scene->addChild(polyNode);
            
        }
        
        
    }
};

//}

#endif
