#ifndef __CAMERA_ACTION_H__
#define __CAMERA_ACTION_H__

#include <cugl/math/cu_math.h>
#include <cugl/cugl.h>
using namespace std;
using namespace cugl;

/**
 * This class provides an template for an animation action template.
 *
 * An action contains only the definition of the animation. This can include
 * information about the transform to use or the duration of the animation.
 * However, it does not contain any attribute of the target. Hence, an action
 * can be reapplied to different targets.
 *
 * By itself, an action does nothing.  It only specifies an action that may
 * take place. To use an action, it must be passed to the ActionManager.  The
 * manager will create an action instance and animate that instance.  While an
 * action may be reused many times, an action instance corresponds to a single
 * animation.
 *
 * To define a custom action, simply create a subclass and override the uppdate
 * method.  This is the method that an action uses to update its target Node.
 */
class CameraAction {
protected:
    /** The duration (in seconds) of the animation */
    float _duration;

#pragma mark -
#pragma mark Constructors
public:
    /**
     * Creates an uninitialized action.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
     * the heap, use one of the static constructors instead.
     */
    CameraAction() {}
    
    /**
     * Deletes this action, disposing all resources
     */
    ~CameraAction() {}

#pragma mark -
#pragma mark Attributes
    /**
     * Returns the duration (in seconds) of this animation
     *
     * Changing this value in mid-animation has undefined side-effects.
     *
     * @return the duration (in seconds) of this animation
     */
    float getDuration() const { return _duration; }

    /**
     * Sets the duration (in seconds) of this animation
     *
     * Changing this value in mid-animation has undefined side-effects.
     *
     * @param time  the duration (in seconds) of this animation
     */
    void setDuration(float time) { _duration = time; }
    
#pragma mark -
#pragma mark Animation Methods
    /**
     * Returns a newly allocated copy of this Action.
     *
     * @return a newly allocated copy of this Action.
     */
    virtual std::shared_ptr<CameraAction> clone();

    /**
     * Prepares a target for action
     *
     * The important state of the target should be allocated and stored in the given
     * state reference. The semantics of this state is action-dependent.
     *
     * @param target    The node to act on
     * @param state     The address to store the node state
     */
    virtual void start(const std::shared_ptr<Camera>& target, void** state) {}

    /**
     * Cleans up a target after an action
     *
     * The target state in the given address should be deallocated, just as it was
     * allocated in {@link #start}. The semantics of this state is action-dependent.
     *
     * @param target    The node to act on
     * @param state     The address to deallocate the node state
     */
    virtual void stop(const std::shared_ptr<Camera>& target, void** state) {}

    /**
     * Executes an (incremental) action on the given target node.
     *
     * The important state of the target is stored in the given state parameter.
     * The semantics of this state is action-dependent.
     *
     * @param target    The node to act on
     * @param state     The relevant node state
     * @param dt        The elapsed time to animate.
     */
    virtual void update(const std::shared_ptr<Camera>& target, void* state, float dt) {}

    
#pragma mark -
#pragma mark Debugging Methods
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
    virtual std::string toString(bool verbose = false) const;
    
    /** Cast from an Action to a string. */
    operator std::string() const { return toString(); }
};
    
#endif /* __CAMERA_ACTION_H__ */

