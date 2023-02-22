//
//  GeneratorController.cpp
//  TileMap Lab
//
//  This module for the procedural content generator
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#include "GeneratorController.h"
#include <cmath>

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
GeneratorController::GeneratorController(bool seeded,
                                         const std::shared_ptr<std::mt19937>& randoms) {
    std::array<int, PERM_LENGTH> p;
    if (seeded) {
        _randoms = randoms;
        p = generatePermutation();
    } else {
        p = kenPermutations;
        for (int i=0; i < 256 ; i++) p[256+i] = p[i];
    }
    _model = std::make_unique<GeneratorModel>(p);
}

/**
 * Calculates the Perlin noise for a cube given a point.
 *
 * @param x    The x position in the cube
 * @param y    The y position in the cube
 * @param z    The z position in the cube
 *
 * @returns The Perlin noise at (`x`, `y`, `z`)
 */
float GeneratorController::noise3D(float x, float y, float z) {
    /// Find unit cube that contains point.
    int X = static_cast<int>(x) & 255;
    int Y = static_cast<int>(y) & 255;
    int Z = static_cast<int>(z) & 255;
    
    /// Find relative x, y, z of point in cube.
    x -= static_cast<int>(x);
    y -= static_cast<int>(y);
    z -= static_cast<int>(z);
    
    /// Compute fade curves for each of x, y, z.
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);
    
    /// Hash coordinates of the 8 cube corners
    int A  = _model->p[X  ] + Y;
    int AA = _model->p[A  ] + Z;
    int AB = _model->p[A+1] + Z;
    int B  = _model->p[X+1] + Y;
    int BA = _model->p[B  ] + Z;
    int BB = _model->p[B+1] + Z;
    
    /// And add blended results from 8 corners of cube
    float corner1 = grad(_model->p[AA  ], x  , y  , z  );
    float corner2 = grad(_model->p[BA  ], x-1, y  , z  );
    float corner3 = grad(_model->p[AB  ], x  , y-1, z  );
    float corner4 = grad(_model->p[BB  ], x-1, y-1, z  );
    float corner5 = grad(_model->p[AA+1], x  , y  , z-1);
    float corner6 = grad(_model->p[BA+1], x-1, y  , z-1);
    float corner7 = grad(_model->p[AB+1], x  , y-1, z-1);
    float corner8 = grad(_model->p[BB+1], x-1, y-1, z-1);
    
    float side12 = lerp(u, corner1, corner2);
    float side34 = lerp(u, corner3, corner4);
    float side56 = lerp(u, corner5, corner6);
    float side78 = lerp(u, corner7, corner8);
    
    float square1234 = lerp(v, side12, side34);
    float square5678 = lerp(v, side56, side78);
    
    return lerp(w, square1234, square5678);
}

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
float GeneratorController::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

/**
 * Linear interpolates between `a` and `b` with `t`.
 *
 * @param a    The left bound
 * @param b    The right bound
 * @param t    The percentage between `a` and `b`
 *
 * @return Linear interpolation of `a` and `b` by `t`
 */
float GeneratorController::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

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
float GeneratorController::grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

#pragma mark -
#pragma mark Provided Methods
/**
 * Generates a Perlin noise permutation array of length `PERM_LENGTH`
 *
 * The numbers are in the range 0 <= x <= (PERM_LENGTH / 2) - 1>, and
 * each unique number occurs exactly twice.
 *
 * @returns a Perlin noise permutation array of length `PERM_LENGTH`
 */
std::array<int, PERM_LENGTH> GeneratorController::generatePermutation() {
    std::array<int, PERM_LENGTH> p;
    for (int i = 0; i < PERM_LENGTH; i++) { p[i] = i & 255; }
    
    if (_randoms) {
        /// Fisher-Yates shuffle https://programming-idioms.org/idiom/10/shuffle-a-list/1249/cpp
        for (int i = 0; i < PERM_LENGTH; ++i) {
            int j = (((*_randoms)()) % (PERM_LENGTH-i)) + i;
            int temp = p[i];
            p[i] = p[j];
            p[j] = temp;
        }
    }
    return p;
}
