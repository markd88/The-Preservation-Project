//
//  InputController.cpp
//
//  This module represents the input handlers.
//
#include "InputController.h"

#pragma mark Input Constants

/** The maximum zoom amount */
#define MAX_ZOOM 5
/** Keyboard turning amount */
#define TURN_UNIT M_PI/128
/** Keyboard pinch amount in pixels */
#define PINCH_UNIT 5

#pragma mark -
#pragma mark Input Controller
/**
 * Creates the handler, initializing the input devices.
 *
 * This method will create the mouse listeners. In the case of failure
 * an assertion will crash the program.
 *
 * Note, the constructor should not be called directly. Retrieve a
 * singleton with {@link #getInstance} instead.
 */
InputController::InputController(){
    _model = std::make_unique<InputModel>();
};

/**
* Deactivates this input controller, releasing all listeners.
*
* This method will not dispose of the input controller. It can be reused
* once it is reinitialized.
*/
void InputController::dispose() {
    if (_model->_activePinch) {
        CoreGesture* gesture = Input::get<CoreGesture>();
        gesture->removeBeginListener(_pinchListener);
        gesture->removeChangeListener(_pinchListener);
        gesture->removeEndListener(_pinchListener);
        _model->_activePinch = false;
    }
}

bool InputController::initTouch() {
    // touch init
    _touch = Input::get<Touchscreen>();
    if (_touch) {
        _touchListener = _touch->acquireKey();
        bool addedKey = _touch->addBeginListener(_touchListener, [=](const cugl::TouchEvent& event, bool focus) {
            this->touchDownCB(event,focus);
        });
        CUAssertLog(addedKey, "failed adding key listener");
        
        bool addedPress = _touch->addEndListener(_touchListener, [=](const cugl::TouchEvent& event, bool focus) {
            this->touchUpCB(event,focus);
        });
        CUAssertLog(addedPress, "failed adding end listener");
        
        bool addedMotion = _touch->addMotionListener(_touchListener, [=](const cugl::TouchEvent& event, const Vec2 previous, bool focus) {
            this->motionCBtouch(event,previous,focus);
        });
        CUAssertLog(addedMotion, "failed adding motion listener");
        _model->_activeTouch = true;
    }
    return _model->_activeTouch;
}

bool InputController::initPinch(const Size &size) {
    // pan gesture init
    bool success = true;
    _model->_screensize = size;
    CoreGesture* gesture = Input::get<CoreGesture>();
    gesture->setSpinActive(true);
    _pinchListener = gesture->acquireKey();
    
    gesture->addBeginListener(_pinchListener,[=](const CoreGestureEvent& event, bool focus) {
        _model->_currTouch = screenToScenePinch(event.origPosition);
        _model->_currTouch.y = _model->_screensize.height-_model->_currTouch.y;
        _model->_prevTouch = _model->_currTouch;
        _model->_currAngle = event.origAngle;
        _model->_prevAngle = event.origAngle;
        _model->_currSpread = event.origSpread;
        _model->_prevSpread = event.origSpread;
        _model->_anchor = _model->_currTouch;
    });
    gesture->addChangeListener(_pinchListener,[=](const CoreGestureEvent& event, bool focus) {
        switch (event.type) {
            case CoreGestureType::PAN:
                _model->_mousepan = true;
                _model->_currTouch = screenToScenePinch(event.currPosition);
                _model->_currTouch.y = _model->_screensize.height-_model->_currTouch.y;
                _model->_anchor = _model->_currTouch;
                break;
            case CoreGestureType::PINCH:
                _model->_currSpread = event.currSpread;
                break;
            case CoreGestureType::SPIN:
                _model->_currAngle = event.currAngle;
                break;
            case CoreGestureType::NONE:
                break;
        }
    });
    gesture->addEndListener(_pinchListener,[=](const CoreGestureEvent& event, bool focus) {
        _model->_mousepan = false;
        _model->_currTouch.setZero();
        _model->_prevTouch.setZero();
        _model->_anchor.setZero();
        _model->_currAngle = 0;
        _model->_prevAngle = 0;
        _model->_currSpread = 0;
        _model->_prevSpread = 0;
    });
    
    _model->_activePinch = success;
    return success;
}

bool InputController::init(const Size &size) {
    bool registerTouch = initTouch();
    bool registerPinch = initPinch(size);
    return registerTouch && registerPinch;
}

/** Returns a singleton instance of InputController. */
std::shared_ptr<InputController> InputController::getInstance() {
    static std::shared_ptr<InputController> inputController;
    if (inputController == nullptr) {
        inputController = std::make_shared<InputController>();
    }
    return inputController;
}

void InputController::update(float dt){
    updateTouch();
    updatePinch(dt);
}

/**
 * Aligns inputs detected through callbacks with frame updates.
 *
 * @param dt  The amount of time (in seconds) since the last frame
 */
void InputController::updateTouch() {
    _model->_prevDown = _model->_currDown;
    _model->_currDown = _model->_touchDown;
    _model->_prevPos = _model->_currPos;
    _model->_currPos = _model->_touchPos;
}

/**
* Processes the currently cached inputs.
*
* This method is used to to poll the current input state.  This will poll the
* keyboad and accelerometer.
*
* This method also gathers the delta difference in the touches. Depending on
* the OS, we may see multiple updates of the same touch in a single animation
* frame, so we need to accumulate all of the data together.
*/
void InputController::updatePinch(float dt) {
    if (_model->_mousepan) {
        _model->_pandelta = _model->_currTouch - _model->_prevTouch;
    } else {
        _model->_pandelta.setZero();
    }
    _model->_angleDelta = _model->_currAngle-_model->_prevAngle;
    _model->_pinchDelta = _model->_currSpread-_model->_prevSpread;
    
    _model->_prevTouch = _model->_currTouch;
    _model->_prevAngle = _model->_currAngle;
    _model->_prevSpread = _model->_currSpread;
}

/**
* Clears any buffered inputs so that we may start fresh.
*/
void InputController::clearPinch() {
    _model->_pandelta = Vec2::ZERO;
    _model->_pinchDelta = 0;
    _model->_angleDelta = 0;
    _model->_mousepan = false;
}

Vec2 InputController::screenToScenePinch(const Vec2& position) const {
    Vec2 result = position/Application::get()->getDisplaySize();
    result *= _model->_screensize;
    return result;
}

#pragma mark -
#pragma mark Touch Callbacks
/**
 * Callback for the beginning of a touch down event.
 *
 * @param event     The event with the mouse information
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
 * Callback for when the touch is pressed down.
 *
 * @param event     The event with the touch information
 * @param previous    The previous position of the touch
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
