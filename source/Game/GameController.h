//
//  GameController.h
//
//  This is a the controller for the gameplay loop. Note that is is NOT
//  a scene. Instead it is a subcontroller that references a scene. This
//  is a legitimate design choice.
//
//  Authors: Walker White and Gonzalo Gonzalez
//  Version: 1/8/23
//
#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__
#include <random>
#include <climits>

#include <cugl/cugl.h>
using namespace cugl;
// Uncomment to activate (but comment out MVC)
//#include <CMV/Tilemap/CMVTilemapController.h>
// using namespace CMV;
// Uncomment to activate (but comment out CMV)
#include <MVC/Tilemap/MVCTilemapController.h>
using namespace MVC;

// Again, need angle brackets
#include <Generator/GeneratorController.h>

/**
 * The primary controller for the game logic.
 *
 * In class, we have typically broken up our game logic into scenes,
 * with each such class being a subclass of Scene2. However, as this
 * class shows, this is not necessary. You could have the scene as
 * an attribute of a more general class.
 */
class GameController {

#pragma mark Internal References
private:
    /** The Game scene */
    std::shared_ptr<cugl::Scene2> _scene;
    /** The random number generator */
    std::shared_ptr<std::mt19937> _randoms;
    /** The current tile map template (for regeneration) */
    int _template;

#pragma mark External References
private:
    /** The tilemap to procedurally generate */
    std::unique_ptr<TilemapController> _tilemap;
    /** Controller with useful methods for procedural generation */
    std::unique_ptr<GeneratorController> _generator;

#pragma mark Main Methods
public:
    /**
     * Creates the game controller.
     *
     * This constructor will procedurally generate a tilemap immediately
     * on creation.
     *
     * @param displaySize   The display size of the game window
     * @param randoms		Reference to the random number generator
     */
    GameController(const Size displaySize, const std::shared_ptr<std::mt19937>& randoms);
    
    /**
     * Responds to the keyboard commands.
     *
     * This method allows us to regenerate the procedurally generated tilemap
     * upon command.
     *
     * @param dt  The amount of time (in seconds) since the last frame
     */
    void update(float dt);
    
    /**
     * Renders the game elements using the`batch.
     *
     * @param batch The SpriteBatch used to render this scene
     */
    void render(std::shared_ptr<SpriteBatch> batch) { _scene->render(batch); }

#pragma mark Generation Helpers
private:
    /** Generates a yellow smiley face with a black smile. */
    void generateSmileyFace();
    
    /**
     * Generates tiles randomly on the tilemap with probability `p`.
     *
     * @param p The probability that a tile is generated.
     */
    void generateRandomTiles(float p);
    
    /** Generates tiles using Perlin noise on the tilemap. */
    void generatePerlinTiles();
    
    /** Generates colored tiles using Perlin noise on the tilemap. */
    void generateColoredPerlinTiles();
    
    /**
     * Generates colored tiles using FBM with `o` octaves on the tilemap.
     *
     * See https://thebookofshaders.com/13/ for an interactive demo on FBM
     * and octaves.
     *
     * @param octaves The octaves of noise to add.
     */
    void generateFractalBrownianMotionTiles(int octaves);

#pragma mark Helpers
private:
    /**
     * Returns a (psuedo-)random float, between 0 and 1, inclusive.
     *
     * The value will be generated from the internal random number generator
     * and hence will depend on the current seed.
     *
     * @return a (psuedo-)random float, between 0 and 1, inclusive.
     */
    float generateRandomFloat() {
        float rand  = static_cast<float>((*_randoms)());
        float limit = static_cast<float>(UINT_MAX);
        return rand/limit;
    }

	/**
	 * Creates a new tile map with the given template number
	 *
	 * @param choice	The template number
	 */
	void generateTemplate(int choice);
    
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
    void printExecution(std::string name, std::function<void()> wrapper);
};

#endif /* __GAME_CONTROLLER_H__ */
