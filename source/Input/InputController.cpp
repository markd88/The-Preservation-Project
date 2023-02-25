//
//  InputController.cpp
//
//  This module represents the input handlers.
//
//
// This is in the same folder so it is okay
#include "InputController.h"
/**
 * Creates the handler, initializing the input devices.
 *
 * This method will create the mouse listeners. In the case of failure
 * an assertion will crash the program.
 *
 * Note, the constructor should not be called directly. Retrieve a
 * singleton with {@link #getInstance} instead.
 */
InputController::InputController(): _model(std::make_unique<InputModel>()),
    _touch(Input::get<Touchscreen>()) {
    _listener = _touch->acquireKey();
    bool addedKey = _touch->addBeginListener(_listener, [=](const cugl::TouchEvent& event, bool focus) {
        this->touchDownCB(event,focus);
    });
    CUAssertLog(addedKey, "failed adding key listener");

    bool addedPress = _touch->addEndListener(_listener, [=](const cugl::TouchEvent& event, bool focus) {
            this->touchUpCB(event,focus);
        });
    CUAssertLog(addedPress, "failed adding end listener");

    bool addedMotion = _touch->addMotionListener(_listener, [=](const cugl::TouchEvent& event, const Vec2 previous, bool focus) {
        this->motionCBtouch(event,previous,focus);
    });
    CUAssertLog(addedMotion, "failed adding motion listener");
}

/** Returns a singleton instance of InputController. */
std::shared_ptr<InputController> InputController::getInstance() {
    static std::shared_ptr<InputController> inputController;
    if (inputController == nullptr) {
        inputController = std::make_shared<InputController>();
    }
    return inputController;
}

/**
 * Aligns inputs detected through callbacks with frame updates.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void InputController::update(float dt) {
    // Swap the keyboard buffers, removing old keys
    _previous.clear();
    _previous = _current;
    _current.clear();
    
    _model->_prevDown = _model->_currDown;
    _model->_currDown = _model->_touchDown;
    _model->_prevPos = _model->_currPos;
    _model->_currPos = _model->_touchPos;
}

#pragma mark -
#pragma mark Touch Callbacks
/**
 * Callback for the beginning of a mouse press event.
 *
 * This function will record a press only if the left button is pressed.
 *
 * @param event     The event with the mouse information
 * @param clicks    The number of clicks (for double clicking)
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::touchDownCB(const cugl::TouchEvent& event, bool focus) {
    if (!_model->_touchDown && _model->_touchId == -1) {
        _model->_touchId = event.touch;
        _model->_touchDown = true;
        _model->_touchPos = event.position;
    }
}

/**
 * Callback for when the mouse is pressed down.
 *
 * This function will record whenever the left mouse is held down after the initial press.
 *
 * @param event     The event with the mouse information
 * @param previous    The previous position of the mouse
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::motionCBtouch(const cugl::TouchEvent& event, const Vec2 previous, bool focus) {
    if (_model->_touchDown && event.touch == _model->_touchId) {
        _model->_touchPos = event.position;
    }
}

/**
 * Callback for the end of a touch event.
 *
 * @param event     The event with the touch information
 * @param focus     Whether this device has focus (UNUSED)
 */
void InputController::touchUpCB(const cugl::TouchEvent& event, bool focus) {
    // Only recognize the left mouse button
    if (_model->_touchDown && _model->_touchId != -1) {
        _model->_touchDown = false;
        _model->_touchId = -1;
    }
}

