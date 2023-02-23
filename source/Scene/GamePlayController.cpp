//
//  GameController.cpp
//  TileMap Lab
//
//  This is a the controller for the gameplay loop. Note that is is NOT
//  a scene. Instead it is a subcontroller that references a scene. This
//  is a legitimate design choice.
//
//  Authors: Walker White and Gonzalo Gonzalez
//  Version: 1/8/23
//
#include <chrono>
// This is in the same directory
#include "GameController.h"
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
GameController::GameController(const Size displaySize,
                               const std::shared_ptr<std::mt19937>& randoms):
_scene(cugl::Scene2::alloc(displaySize)) {
    _randoms = randoms;
    
    /// Initialize the tilemap and add it to the scene
    _tilemap = std::make_unique<TilemapController>();
    _tilemap->addChildTo(_scene);
    _template = 0;
    
    /// Initialize generator class with a boolean to seed random generator
    _generator = std::make_unique<GeneratorController>(true,_randoms);
}

/**
 * Responds to the keyboard commands.
 *
 * This method allows us to regenerate the procedurally generated tilemap
 * upon command.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void GameController::update(float dt) {
    auto inputController = InputController::getInstance();
    inputController->update(dt);
    
    /// Pre-made templates
    if (inputController->isKeyPressed(KeyCode::NUM_1)) {
        generateTemplate(1);
    } else if (inputController->isKeyPressed(KeyCode::NUM_2)) {
        generateTemplate(2);
    } else if (inputController->isKeyPressed(KeyCode::NUM_3)) {
        generateTemplate(3);
    } else if (inputController->isKeyPressed(KeyCode::NUM_4)) {
        generateTemplate(4);
    } else if (inputController->isKeyPressed(KeyCode::NUM_5)) {
        generateTemplate(5);
    }
    
    /// Tile size modifier with `-` and `=`
    if (inputController->isKeyPressed(KeyCode::EQUALS)) { _tilemap->modifyTileSize(2, 2); }
    else if (inputController->isKeyPressed(KeyCode::MINUS)) { _tilemap->modifyTileSize(0.5, 0.5); }
    
    /// Dimension modifier with `[` and `]` for modifying columns
    /// and `;` and `'` for modifying rows
    if (inputController->isKeyPressed(KeyCode::LEFT_BRACKET)) { _tilemap->modifyDimensions(-1, 0);
        
    } else if (inputController->isKeyPressed(KeyCode::RIGHT_BRACKET)) { _tilemap->modifyDimensions(1, 0);
        
    } else if (inputController->isKeyPressed(KeyCode::SEMICOLON)) { _tilemap->modifyDimensions(0, -1);
    } else if (inputController->isKeyPressed(KeyCode::QUOTE)) { _tilemap->modifyDimensions(0, 1);
    }
    
    /// Color inversion with `\`
    if (inputController->isKeyPressed(KeyCode::BACKSLASH)) {
        _tilemap->invertColor();
    }
    
    // Regenerating tile map with a new (random) seed using 'S'
    if (inputController->isKeyPressed(KeyCode::S)) {
        unsigned seed = static_cast<unsigned>(time(0));
        _randoms->seed(seed);
        CULog("Seed = %d", seed);
        _generator = std::make_unique<GeneratorController>(true,_randoms);
        generateTemplate(_template);
    }
}


#pragma mark -
#pragma mark Generation Helpers

/** Generates a yellow smiley face with a black smile. */
void GameController::generateSmileyFace() {
    _tilemap->updateDimensions(Vec2(20, 20));
    _tilemap->updateColor(Color4::YELLOW);
    _tilemap->updateTileSize(Size(10, 10));
    _tilemap->updatePosition(_scene->getSize() / 2);
    
    Color4 tileColor = Color4::BLACK;
    
    // Eyes
    _tilemap->addTile(5, 15, tileColor);
    _tilemap->addTile(15, 15, tileColor);
    
    // Nose
    _tilemap->addTile(10, 8, tileColor);
    
    // Smile
    _tilemap->addTile(3, 5, tileColor);
    _tilemap->addTile(4, 4, tileColor);
    for(int i = 5; i <= 15; i++) {
        _tilemap->addTile(i, 3, tileColor);
    }
    _tilemap->addTile(16, 4, tileColor);
    _tilemap->addTile(17, 5, tileColor);
}

/**
 * Generates tiles randomly on the tilemap with probability `p`.
 *
 * @param p The probability that a tile is generated.
 */
void GameController::generateRandomTiles(float p) {
    Vec2 dimensions = Vec2(10, 10);
    _tilemap->updateDimensions(dimensions);
    _tilemap->updateColor(Color4::WHITE);
    _tilemap->updateTileSize(Size(30, 30));
    _tilemap->updatePosition(_scene->getSize() / 2);
    
    if (p == 0) return;
    for (int col = 0; col < dimensions.x; col++) {
        for (int row = 0; row < dimensions.y; row++) {
            float random = generateRandomFloat();
            if (p >= random) {
                float r = generateRandomFloat() * 255;
                float g = generateRandomFloat() * 255;
                float b = generateRandomFloat() * 255;
                _tilemap->addTile(col, row, Color4(r, g, b));
            }
        }
    }
}

/** Generates tiles using Perlin noise on the tilemap. */
void GameController::generatePerlinTiles() {
    Vec2 dimensions = Vec2(500, 500);
    _tilemap->updateDimensions(dimensions);
    _tilemap->updateTileSize(Size(1, 1));
    _tilemap->updatePosition(_scene->getSize() / 2);
    
    /// Adapted from https://github.com/rtouti/rtouti.github.io/blob/gh-pages/examples/perlin-noise.html
    for (int col = 0; col < dimensions.x; col++) {
        for (int row = 0; row < dimensions.y; row++) {
            float n = _generator->noise2D(col*0.01, row*0.01);
            n += 1.0;
            n *= 0.5;
            int rgb = round(255*n);
            _tilemap->addTile(col, row, Color4(rgb, rgb, rgb));
        }
    }
}

/** Generates colored tiles using Perlin noise on the tilemap. */
void GameController::generateColoredPerlinTiles() {
    Vec2 dimensions = Vec2(500, 500);
    _tilemap->updateDimensions(dimensions);
    _tilemap->updateTileSize(Size(1, 1));
    _tilemap->updatePosition(_scene->getSize() / 2);
    
    /// Adapted from https://github.com/rtouti/rtouti.github.io/blob/gh-pages/examples/perlin-noise.html
    for (int col = 0; col < dimensions.x; col++) {
        for (int row = 0; row < dimensions.y; row++) {
            float n = _generator->noise2D(col*0.01, row*0.01);
            n += 1.0;
            n *= 0.5;
            int rgb = round(255*n);
            Color4 color;
            if (n < 0.5) {
                color = Color4(0, 0, rgb*2);
            } else if (n < 0.9) {
                color = Color4(0, rgb, round(rgb*0.5));
            } else {
                color = Color4(rgb, rgb, rgb);
            }
            _tilemap->addTile(col, row, color);
        }
    }
}

/**
 * Generates colored tiles using FBM with `o` octaves on the tilemap.
 *
 * See https://thebookofshaders.com/13/ for an interactive demo on FBM
 * and octaves.
 *
 * @param octaves The octaves of noise to add.
 */
void GameController::generateFractalBrownianMotionTiles(int octaves) {
    Vec2 dimensions = Vec2(500, 500);
    _tilemap->updateDimensions(dimensions);
    _tilemap->updateTileSize(Size(1, 1));
    _tilemap->updatePosition(_scene->getSize() / 2);
    
    /// Adapted from https://github.com/rtouti/rtouti.github.io/blob/gh-pages/examples/perlin-noise.html
    for (int col = 0; col < dimensions.x; col++) {
        for (int row = 0; row < dimensions.y; row++) {
            float n = 0;
            float a = 1.0;
            float f = 0.005;
            for(int i = 0; i < octaves; i++) {
                float v = a * _generator->noise2D(col*f, row*f);
                n += v;
                a *= 0.5;
                f *= 2.0;
            }
            n += 1.0;
            n *= 0.5;
            int rgb = round(255*n);
            Color4 color;
            if (n < 0.5) {
                color = Color4(0, 0, rgb*2);
            } else if (n < 0.9) {
                color = Color4(0, rgb, round(rgb*0.5));
            } else {
                color = Color4(rgb, rgb, rgb);
            }
            _tilemap->addTile(col, row, color);
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
void GameController::generateTemplate(int choice) {
    /// Pre-made templates
    switch (choice) {
        case 1:
            _tilemap->clearMap();
            printExecution("generateSmileyFace", [this](){
                generateSmileyFace();
            });
            _template = 1;
            break;
        case 2:
            _tilemap->clearMap();
            printExecution("generateRandomTiles", [this](){
                generateRandomTiles(0.5);
            });
            _template = 2;
            break;
        case 3:
            _tilemap->clearMap();
            printExecution("generatePerlinTiles", [this](){
                generatePerlinTiles();
            });
            _template = 3;
            break;
        case 4:
            _tilemap->clearMap();
            printExecution("generateColoredPerlinTiles", [this](){
                generateColoredPerlinTiles();
            });
            _template = 4;
            break;
        case 5:
            _tilemap->clearMap();
            printExecution("generateFractalBrownianMotionTiles", [this](){
                generateFractalBrownianMotionTiles(8);
            });
            _template = 5;
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
void GameController::printExecution(std::string name, std::function<void()> wrapper) {
    auto before = std::chrono::high_resolution_clock::now();
    CULog("Running %s", name.data());
    wrapper();
    auto after = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
    CULog("Generated after %lld milliseconds", duration);
}
