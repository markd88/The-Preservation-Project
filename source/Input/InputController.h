//
//  InputController.h
//
//  This module represents the input handlers.
//
#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__
#include <unordered_set>
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
    
    /** The unique key for the touch listeners */
    Uint32 _touchListener;
    
    /** The unique key for the gensture listeners */
    Uint32 _pinchListener;
    
#pragma mark External References
private:
    /** Touchscreen reference */
    Touchscreen* _touch;
    
public:
#pragma mark -
#pragma mark Constructors
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
    
    /**
     * Disposes of this input controller, releasing all listeners.
     */
    ~InputController() { dispose(); }
    
    /**
     * Deactivates this input controller, releasing all listeners.
     *
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();
    
    /** Returns a singleton instance of InputController. */
    static std::shared_ptr<InputController> getInstance();

    // touch
    bool initTouch();

    // gesture
    bool initPinch(const cugl::Size& size);

    cugl::Vec2 screenToScenePinch(const cugl::Vec2& position) const;
    
#pragma mark -
#pragma mark Input Detection
    /**
     * Returns true if this control is active.
     *
     * An active control is one where all of the listeners are attached
     * and it is actively monitoring input. An input controller is only
     * active if {@link #init} is called, and if {@link #dispose} is not.
     *
     * @return true if this control is active.
     */
    bool isActiveTouch() const { return _model->_activeTouch; }
    bool isActivePinch() const { return _model->_activePinch; }
    
    /**
     * Aligns inputs detected through callbacks with frame updates.
     *
     * @param dt  The amount of time (in seconds) since the last frame
     */
    void update(float dt);
    void updateTouch();
    void updatePinch(float dt);

    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clearPinch();

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
    
    // pan callbacks
    /**
     * Callback for the end of a pan event
     *
     * @param event The associated event
     * @param focus     Whether the listener currently has focus
     */
    void pinchEndedCB(const cugl::CoreGestureEvent& event, bool focus);

    /**
     * Callback for a pan movement event
     *
     * @param event The associated event
     * @param focus     Whether the listener currently has focus
     */
    void pinchMovedCB(const cugl::CoreGestureEvent& event, bool focus);

#pragma mark Input State Getters
public:
    // touch
    /**
     * Returns the current touch position
     *
     * @return the current touch position
     */
    const cugl::Vec2& getPosition() const {
        return _model->_currPos;
    }
    
    /**
     * Returns the previous touch position
     *
     * @return the previous touch position
     */
    const cugl::Vec2& getPrevious() const {
        return _model->_prevPos;
    }
    
    /**
     * Return true if the user initiated a press this frame.
     *
     * A press means that the user is pressing (button/finger) this
     * animation frame, but was not pressing during the last frame.
     *
     * @return true if the user initiated a press this frame.
     */
    bool didPress() const {
        return !_model->_prevDown && _model->_currDown;
    }
    
    /**
     * Return true if the user initiated a release this frame.
     *
     * A release means that the user was pressing (button/finger) last
     * animation frame, but is not pressing during this frame.
     *
     * @return true if the user initiated a release this frame.
     */
    bool didRelease() const {
        return !_model->_currDown && _model->_prevDown;
    }
    
    /**
     * Return true if the user is actively pressing this frame.
     *
     * This method only checks that a press is active or ongoing.
     * It does not care when the press was initiated.
     *
     * @return true if the user is actively pressing this frame.
     */
    bool isDown() const {
        return _model->_currDown;
    }
    
    // gesture
    const cugl::Vec2& getAnchor() const { return _model->_anchor; }

    void setAnchor(const cugl::Vec2& anchor) { _model->_anchor = anchor; }
    
    /**
     * Returns the current pan delta.
     *
     * The delta is the amount that the user has moved a two-finger touch
     * since the last animation frame. This distance is measured according
     * to the first-finger touch.
     *
     * @return The input thrust
     */
    const cugl::Vec2& getPanDelta() const { return _model->_pandelta; }
    
    float getPinchDelta() const { return _model->_pinchDelta; }

    float getAngleDelta() const { return _model->_angleDelta; }
};

#endif /* __INPUT_CONTROLLER_H__ */
