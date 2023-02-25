//
//  InputController.h
//
//  This module represents the input handlers.
//
#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__
#include <unordered_set>

// This is in the same folder
#include "InputModel.h"

/**
 * A class representing a unified input handler.
 *
 * This handler processes both keyboard and mouse input. Notice that it
 * caches the result of callbacks, and never polls, even with keyboard
 * events. That is because PCG can slow the game down to 6 FPS (no, there
 * is not a missing 0 there), causing us to miss key events. This
 * architecture allows us to buffer keypresses even when FPS is slow.
 */
class InputController {
    
#pragma mark Internal References
private:
    /** Model reference*/
    std::unique_ptr<InputModel> _model;
    /** Track the keys pressed this animation frame */
    std::unordered_set<KeyCode> _current;
    /** Track the keys pressed the previous animation frame */
    std::unordered_set<KeyCode> _previous;
    
    /** The unique key for the touch listeners */
    Uint32 _touchKey;
    cugl::TouchID _touchId; 
    
#pragma mark External References
private:
    /** Touchscreen reference */
    Touchscreen* touch;
    
#pragma mark Main Functions
public:
    /**
     * Creates the handler, initializing the input devices.
     *
     * This method will create the touch listeners. In the case of failure
     * an assertion will crash the program.
     *
     * Note, the constructor should not be called directly. Retrieve a
     * singleton with {@link #getInstance} instead.
     */
    InputController();
    
    /** Returns a singleton instance of InputController. */
    static std::shared_ptr<InputController> getInstance();
    
    /**
     * Aligns inputs detected through callbacks with frame updates.
     *
     * @param dt  The amount of time (in seconds) since the last frame
     */
    void update(float dt);

#pragma mark Touch Callbacks
private:
    /**
     * Call back to execute when a touch is pressed.
     *
     * This function will record a press only if the left button is pressed.
     *
     * @param event     The event with the touch information
     * @param focus     Whether this device has focus (UNUSED)
     */
    void touchDownCB(const cugl::TouchEvent& event, bool focus);
    
    /**
     * Call back to execute when a touch is released.
     *
     * @param event     The event with the touch information
     * @param focus     Whether this device has focus (UNUSED)
     */
    void touchUpCB(const cugl::TouchEvent& event, bool focus);
    
    /**
     * Call back to execute when the finger moves.
     *
     * This input controller sets the pointer awareness only to monitor a touch
     * when it is dragged (moved with button down), not when it is moved. This
     * cuts down on spurious inputs. In addition, this method only pays attention
     * to drags initiated with the touch.
     *
     * @param event     The event with the touch information
     * @param previous  The previously reported touch location
     * @param focus     Whether this device has focus (UNUSED)
     */
    void motionCBtouch(const cugl::TouchEvent& event, const cugl::Vec2 previous, bool focus);
    
#pragma mark Input State Getters
public:
    /**
     * Returns whether `key` is held down.
     *
     * @param key   The keyboard key
     */
    bool isKeyPressed(KeyCode key) {
        // Previous was the result before the start of this frame
        return _previous.find(key) != _previous.end();
    }
    
    /** Returns whether the finger was held down during this frame. */
    bool isTouchDown() { return _model->_touchDown; }
    
    /** Returns the touch's last recorded position. */
    Vec2 getLastTouchPos() { return _model->_touchPos; }
    
};

#endif /* __INPUT_CONTROLLER_H__ */
