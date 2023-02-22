//
//  GeneratorController.h
//  TileMap Lab
//
//  This module for the procedural content generator
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __GENERATOR_CONTROLLER_H__
#define __GENERATOR_CONTROLLER_H__
#include <random>
// This is in the same folder
#include "GeneratorModel.h"

/**
 * A controller class to perform procedural content generation using
 * Perlin noise.
 */
class GeneratorController {

#pragma mark Internal References
private:
    /** Model reference */
    std::unique_ptr<GeneratorModel> _model;
    /** The shared random number generator */
    std::shared_ptr<std::mt19937> _randoms;
    
#pragma mark Main Methods
public:
    /**
     * Creates a new content generator.
     *
     * This initializes all parameters necessary to assist with
     * procedural generation. You should set seed to false when
     * testing.
     *
     * @param seeded   Whether or not to use random generation
     * @param randoms  The random number generator
     */
    GeneratorController(bool seeded, const std::shared_ptr<std::mt19937>& randoms);

#pragma mark Procedural Generation Noise
public:
    /**
     * Calculates the Perlin noise for a cube given a point.
     *
     * @param x    The x position in the cube
     * @param y    The y position in the cube
     * @param z    The z position in the cube
     *
     * @returns The Perlin noise at (`x`, `y`, `z`)
     */
    float noise3D(float x, float y, float z);
    
    /**
     * Calculates the Perlin noise for a grid given a point.
     *
     * @param x    The x position in the grid
     * @param y    The y position in the grid
     *
     * @returns The Perlin noise at (`x`, `y`)
     */
    float noise2D(float x, float y) {
        return noise3D(x, y, 0);
    }

#pragma mark Internal Helpers
private:
    /**
     * Returns the value at `t` of the ease curve
     *
     * This is an implementation of an ease curve between 0 and 1, inclusive,
     * for smoother transitions than linear interpolation.
     *
     * @param t   The time ratio of the ease curve
     *
     * @return The value at `t` of the ease curve
     */
    float fade(float t);
    
    /**
     * Linear interpolates between `a` and `b` with `t`.
     *
     * @param a    The left bound
     * @param b    The right bound
     * @param t    The percentage between `a` and `b`
     *
     * @return Linear interpolation of `a` and `b` by `t`
     */
    float lerp(float t, float a, float b);
    
    /**
     * Performs a dot product to calculate a constant vector at corners of a cube.
     *
     * @param hash  A hash to denote which corner the vector is for
     * @param x     The x position in the cube
     * @param y     The y position in the cube
     * @param z     The z position in the cube
     *
     * @return The constant vector for the corner denoted by `hash`
     */
    float grad(int hash, float x, float y, float z);
    
    /**
     * Generates a Perlin noise permutation array of length `PERM_LENGTH`
     *
     * The numbers are in the range 0 <= x <= (PERM_LENGTH / 2) - 1>, and
     * each unique number occurs exactly twice.
     * 
     * @returns a Perlin noise permutation array of length `PERM_LENGTH`
     */
    std::array<int, PERM_LENGTH> generatePermutation();
    
#pragma mark Constants
private:
    /** The original permutation used in Ken Perlin's paper https://cs.nyu.edu/~perlin/noise/ */
    std::array<int, PERM_LENGTH> kenPermutations = {151,160,137,91,90,15,
        131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
        190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
        88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
        77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
        135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
        5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
        223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
        129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
        251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
        49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };
};

#endif /* __GENERATOR_CONTROLLER_H__ */
