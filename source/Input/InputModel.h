//
//  InputModel.h
//
//  This module represents the current input state.
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
    bool _activeTouch;
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
    
#pragma mark Pinch Gesture
    // TOUCH SUPPORT
    bool _activePinch;
    /** The current touch location for the current gesture */
    cugl::Vec2 _currTouch;
    /** The last touch location for the current gesture */
    cugl::Vec2 _prevTouch;
    float _currAngle;
    float _prevAngle;
    float _currSpread;
    float _prevSpread;
    // INPUT RESULTS
    /** The panning distance from the player input */
    cugl::Vec2 _pandelta;
    float _angleDelta;
    float _pinchDelta;
    
    cugl::Vec2 _anchor;
    
    /** Whether or not are in an active mouse pan */
    bool _mousepan;
    bool _prevPan;
    
    cugl::Size _screensize;

#pragma mark Main Functions
public:
    /** Creates the model state */
    InputModel():
        _activeTouch(false),
        _currDown(false),
        _prevDown(false),
        // touch
        // _touchKey(0),
        _touchId(-1),
        // pan
        _activePinch(false),
        _mousepan(false), // ??
        _currAngle(0),
        _prevAngle(0),
        _currSpread(0),
        _prevSpread(0),
        _prevPan(false),
        _angleDelta(0),
        _pinchDelta(0) {}
};



#endif /* InputModel_h */

