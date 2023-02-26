#include <chrono>
#include "GamePlayController.h"
// This is NOT in the same directory
#include <Input/InputController.h>

#pragma mark Main Methods
/**
 * Creates the game controller.
 *
 * This constructor will procedurally generate a tilemap immediately
 * on creation.
 *
 * @param displaySize   The display size of the game window
 * @param randoms        Reference to the random number generator
 */
GamePlayController::GamePlayController(const Size displaySize,
                               const std::shared_ptr<std::mt19937>& randoms):
_scene(cugl::Scene2::alloc(displaySize)) {
    /// Initialize the tilemap and add it to the scene
    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);
    _template = 0;
}

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void GamePlayController::update(float dt) {
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    
    /// Pre-made templates
    if (inputController->isKeyPressed(KeyCode::NUM_1)) {
        generateTemplate(1);
    } else if (inputController->isKeyPressed(KeyCode::NUM_2)) {
        generateTemplate(2);
    }
}

#pragma mark -
#pragma mark Generation Helpers

/** Generates a yellow smiley face with a black smile. */
void GamePlayController::generatePrimaryWorld() {
    _tilemap->updateDimensions(Vec2(65, 35));
    _tilemap->updateColor(Color4::WHITE);
    _tilemap->updateTileSize(Size(15, 15));
    _tilemap->updatePosition(_scene->getSize()/2);
    
    Color4 tileColor = Color4::BLACK;
    // lower-right line
    for(int i = 65; i >= 10; i--) {
        _tilemap->addTile(i, 12, tileColor);
    }
    // upper-left line
    for(int i = 0; i <= 50; i++) {
        _tilemap->addTile(i, 23, tileColor);
    }
    
    // lower guard
    tileColor = Color4::RED;
    for (int i = 2; i <= 7; i++) {
        for(int j = 7; j <= 17; j++) {
            _tilemap->addTile(i, j, tileColor);
        }
    }
    
    // upper guard
    for (int i = 50; i <= 60; i++) {
        for(int j = 15; j <= 20; j++) {
            _tilemap->addTile(i, j, tileColor);
        }
    }
    
}

/**
 * Generates tiles randomly on the tilemap with probability `p`.
 *
 * @param p The probability that a tile is generated.
 */
void GamePlayController::generateSecondaryWorld() {
    _tilemap->updateDimensions(Vec2(65, 35));
    _tilemap->updateColor(Color4::WHITE);
    _tilemap->updateTileSize(Size(15, 15));
    _tilemap->updatePosition(_scene->getSize()/2);
    
    Color4 tileColor = Color4::BLACK;
    
    // lower block
    for (int i = 65; i >= 10; i--) {
        for(int j = 0; j <= 12; j++) {
            _tilemap->addTile(i, j, tileColor);
        }
    }
    
    // middle block
    for (int i = 22; i <= 27; i++) {
        for(int j = 12; j <= 23; j++) {
            _tilemap->addTile(i, j, tileColor);
        }
    }
    
    // upper block
    for (int i = 0; i <= 50; i++) {
        for(int j = 35; j >= 23; j--) {
            _tilemap->addTile(i, j, tileColor);
        }
    }

}



#pragma mark -
#pragma mark Helpers
/**
 * Creates a new tile map with the given template number
 *
 * @param template    The template number
 */
void GamePlayController::generateTemplate(int choice) {
    /// Pre-made templates
    switch (choice) {
        case 1:
            _tilemap->clearMap();
            printExecution("generatePrimaryWorld", [this](){
                generatePrimaryWorld();
            });
            _template = 1;
            break;
        case 2:
            _tilemap->clearMap();
            printExecution("generateSecondaryWorld", [this](){
                generateSecondaryWorld();
            });
            _template = 2;
            break;
        default:
            break;
    }
}

/**
 * Executes a function with debugging information.
 *
 * This function runs function `name` wrapped in `wrapper` and will call
 * CULog twice. The information from CLog will indicate
 *
 * - when the function starts
 * - how long it took to execute
 *
 * @param name      The name of the wrapped function
 * @param wrapper   The function wrapper to execute
 */
void GamePlayController::printExecution(std::string name, std::function<void()> wrapper) {
    auto before = std::chrono::high_resolution_clock::now();
    CULog("Running %s", name.data());
    wrapper();
    auto after = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
    CULog("Generated after %lld milliseconds", duration);
}
