//
//  InputModel.h
//
//  This module represents the current input state. Note that this application
//  does not use the mouse at all. This code is here to simply show you
//  how you might want to organize your state.
//
//
#ifndef __INPUT_MODEL_H__
#define __INPUT_MODEL_H__
#include <cugl/cugl.h>

using namespace cugl;

/**
 * A class representing the current input state
 */
class InputModel {

#pragma mark State
public:
    /** Whether the input device was successfully initialized */
    bool _active;
    /** The current touch/mouse position */
    cugl::Vec2 _currPos;
    /** The previous touch/mouse position */
    cugl::Vec2 _prevPos;
    /** Whether there is an active button/touch press */
    bool _currDown;
    /** Whether there was an active button/touch press last frame*/
    bool _prevDown;
    
    // Touch:
//    Uint32 _touchKey;
    cugl::Vec2 _touchPos;
    bool _touchDown;
    cugl::TouchID _touchId;

#pragma mark Main Functions
public:
    /** Creates the model state */
    InputModel():
        _active(false),
        _currDown(false),
        _prevDown(false),
        // touch
//        _touchKey(0),
        _touchId(-1){}

};



#endif /* InputModel_h */

