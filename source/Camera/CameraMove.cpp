#include "CameraMove.h"
#include <string>
#include <sstream>

using namespace cugl;
using namespace cugl::scene2;

#pragma mark -
#pragma mark CameraMoveTo

/**
 * Initializes a movement animation towards towards the given position.
 *
 * The animation will take place over the given number of seconds.
 *
 * @param target    The target position
 * @param time      The animation duration
 *
 * @return true if initialization was successful.
 */
bool CameraMoveTo::init(const Vec2 target, float time) {
    _target = target;
    _duration = time;
    return true;
}

/**
 * Returns a newly allocated copy of this Action.
 *
 * @return a newly allocated copy of this Action.
 */
std::shared_ptr<CameraAction> CameraMoveTo::clone() {
    auto action = CameraMoveTo::alloc();
    action->setTarget(_target);
    return action;
}

/**
 * Prepares a target for action
 *
 * The important state of the target should be allocated and stored in the given
 * state reference. The semantics of this state is action-dependent.
 *
 * @param target    The node to act on
 * @param state     The address to store the node state
 */
void CameraMoveTo::start(const std::shared_ptr<Camera>& target, void** state) {
    // We need both position AND anchor
    Vec2* data = (Vec2*)malloc(sizeof(Vec2)*2);
    data[1] = target->getPosition();
    *state = data;
}

/**
 * Cleans up a target after an action
 *
 * The target state in the given address should be deallocated, just as it was
 * allocated in {@link #start}. The semantics of this state is action-dependent.
 *
 * @param target    The node to act on
 * @param state     The address to deallocate the node state
 */
void CameraMoveTo::stop(const std::shared_ptr<Camera>& target, void** state) {
    Vec2* data = (Vec2*)(*state);
    target->setPosition(_target);
    free(*state);
    *state = NULL;
}

/**
 * Executes an action on the given target node.
 *
 * The important state of the target is stored in the given state parameter.
 * The semantics of this state is action-dependent.
 *
 * @param target    The node to act on
 * @param state     The relevant node state
 * @param dt        The elapsed time to animate.
 */
void CameraMoveTo::update(const std::shared_ptr<Camera>& target, void* state, float dt) {
    Vec2* data = (Vec2*)state;
    Vec2  pos  = target->getPosition();
    Vec2  diff = _target-data[1];
    target->setPosition(pos+diff*dt);
    target->update();
}

/**
 * Returns a string representation of the action for debugging purposes.
 *
 * If verbose is true, the string will include class information.  This
 * allows us to unambiguously identify the class.
 *
 * @param verbose Whether to include class information
 *
 * @return a string representation of this action for debuggging purposes.
 */
std::string CameraMoveTo::toString(bool verbose) const {
    std::stringstream ss;
    ss << "MoveTo{";
    ss << _target.toString();
    ss << "}'";
    return ss.str();
}
