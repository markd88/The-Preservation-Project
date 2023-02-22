//
//  GeneratorModel.h
//  TileMap Lab
//
//  This module for procedurally generated content
//
//  Author: Gonzalo Gonzalez
//  Version: 1/5/23.
//
#ifndef __GENERATOR_MODEL_H__
#define __GENERATOR_MODEL_H__

#define PERM_LENGTH 256 * 2

#include <array>
#include <cugl/cugl.h>
using namespace cugl;

/**
 * A model class representing the Perlin noise generated
 */
class GeneratorModel {

#pragma mark State
public:
    /** Permutation array */
    std::array<int, PERM_LENGTH> p;

#pragma mark Main Functions
public:
    /**
     * Creates the model state.
     *
     * @param p  The permutation array to use for Perlin noise
     */
    GeneratorModel(std::array<int, PERM_LENGTH> p): p(p) { }

};

#endif /* __GENERATOR_MODEL_H__ */
