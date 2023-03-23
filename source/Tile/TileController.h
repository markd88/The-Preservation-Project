#include "TileModel.h"
#include "TileView.h"

class TileController {
    
#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<TileModel> _model;
    /** View reference */
    std::unique_ptr<TileView> _view;
    
#pragma mark Main Methods
public:
    /**
     * Creates a controller for the model and view.
     *
     * @param position  The bottom left corner of the tile
     * @param size      The width and height of a tile
     * @param color     The tile color
     * @param is_obs     if the tile is an obstacle
     */
    TileController(Vec2 position, Size size, Color4 color, bool is_obs = false) {
        // TODO: Implement me
        this->_model = std::make_unique<TileModel>(position, size, color, is_obs);
        _view = std::make_unique<TileView>(position, size, color);
    }
    
#pragma mark Update Methods
public:
    /**
     *  Updates the model and view with position of this tile.
     *
     *  @param position  The bottom left corner of the tile
     */
    void updatePosition(Vec2 position) {
        // TODO: Implement me
        this->_model->setPosition(position);
        _view->setPosition(position);
    }
    
    /**
     *  Updates the model and view with the size of this tile.
     *
     *  @param size  The bottom left corner of the tile
     */
    void updateSize(Size size) {
        // TODO: Implement me
        this->_model->setSize(size);
        this->_view->setSize(size);
    }
    
    /**
     *  Updates the model and view with the color of this tile.
     *
     *  @param color The tile color
     */
    void updateColor(Color4 color) {
        // TODO: Implement me
        _model->setColor(color);
        _view->setColor(color);
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view as a child to the given `node`.
     *
     * @param node The scenenode to add the view to
     */
    void addChildTo(std::shared_ptr<scene2::SceneNode> node) {
        // TODO: Implement me
        _view->addChildTo(node);
    }
    
    /**
     * Removes the view child from the given `node`.
     *
     * @param node The scenenode to remove the view from
     */
    void removeChildFrom(std::shared_ptr<scene2::SceneNode> node) {
        // TODO: Implement me
        _view->removeChildFrom(node);
    }
    
#pragma mark Getters
public:
    /**
     *  Get if the tile is obstacle
     *
     *  @param is_obs
     */
    bool is_obs(){
        return _model->is_obs();
    }
    
    /**
     *  Detect if this file contains a point
     *
     *  @param point, the position of the point
     */
    bool contains(Vec2 point){
        return _view->contains(point);
    }
    
    bool containsLine(Vec2 a, Vec2 b){
        return _view->containsLine(a, b);
    }
    
};
