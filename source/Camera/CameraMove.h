#ifndef __CAMERA_MOVE_ACTION_H__
#define __CAMERA_MOVE_ACTION_H__

#include "CameraAction.h"
#include <cugl/cugl.h>
using namespace cugl;

#pragma mark -
#pragma mark CameraMoveTo

/**
 * This action represents a movement to a given position
 *
 * An action contains only the definition of the transformation; it does not
 * contain any attribute of the target. Hence, an action it can be reapplied
 * to different targets.
 *
 * By itself, an action does nothing.  It only specifies an action that may
 * take place. To use an action, it must be passed to the ActionManager.  The
 * manager will create an action instance and animate that instance.  While an
 * action may be reused many times, an action instance corresponds to a single
 * animation.
 */
class CameraMoveTo : public CameraAction {
protected:
    /** The target destination for this action */
    Vec2 _target;

public:
#pragma mark Constructors
    /**
     * Creates an uninitialized movement action.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
     * the heap, use one of the static constructors instead.
     */
    CameraMoveTo() { _target = Vec2::ZERO; }

    /**
    * Deletes this action instance, disposing all resources
    */
    ~CameraMoveTo() { dispose(); }
    
    /**
     * Disposes all of the resources used by this action.
     *
     * A disposed action can be safely reinitialized.
     */
    void dispose() { _target = Vec2::ZERO; }
    
    /**
     * Initializes an instantaneous movement towards the origin.
     *
     * The target position is set to (0.0, 0.0), meaning that this action will
     * move a node towards the origin. The animation will be instantaneous.
     *
     * @return true if initialization was successful.
     */
    bool init() {
        return init(Vec2(0.0, 0.0), 0.0f);
    }
    
    /**
     * Initializes an instantaneous movement towards towards the given position.
     *
     * The animation will be instantaneous.
     *
     * @param target    The target position
     *
     * @return true if initialization was successful.
     */
    bool init(const Vec2 target) {
        return init(target,0.0f);
    }
    
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
    bool init(const Vec2 target, float time);

#pragma mark Static Constructors
    /**
     * Returns a newly allocated, instantaneous motion towards the origin.
     *
     * The target position is set to (0.0, 0.0), meaning that this action will
     * move a node towards the origin. The animation will be instantaneous.
     *
     * @return a newly allocated, instantaneous motion towards the origin.
     */
    static std::shared_ptr<CameraMoveTo> alloc() {
        std::shared_ptr<CameraMoveTo> result = std::make_shared<CameraMoveTo>();
        return (result->init() ? result : nullptr);
    }
    
    /**
     * Returns a newly allocated, instantaneous motion towards the given position.
     *
     * The animation will be instantaneous.
     *
     * @param target    The target position
     *
     * @return a newly allocated, instantaneous motion towards the given position.
     */
    static std::shared_ptr<CameraMoveTo> alloc(const Vec2 target) {
        std::shared_ptr<CameraMoveTo> result = std::make_shared<CameraMoveTo>();
        return (result->init(target) ? result : nullptr);
    }
    
    /**
     * Returns a newly allocated motion towards the given position.
     *
     * The animation will take place over the given number of seconds.
     *
     * @param target    The target position
     * @param time      The animation duration
     *
     * @return a newly allocated motion towards the given position.
     */
    static std::shared_ptr<CameraMoveTo> alloc(const Vec2 target, float time) {
        std::shared_ptr<CameraMoveTo> result = std::make_shared<CameraMoveTo>();
        return (result->init(target, time) ? result : nullptr);
    }

#pragma mark Attributes
    /**
     * Returns the movement target for this action.
     *
     * Changing this value for an actively animating action can have
     * undefined effects.
     *
     * @return the movement target for this action.
     */
    const Vec2& getTarget() const { return _target; }
    
    /**
     * Sets the movement target for this action.
     *
     * Changing this value for an actively animating action can have
     * undefined effects.
     *
     * @param target    the movement target for this action.
     */
    void setTarget(const Vec2& target) { _target = target; }
    
#pragma mark Animation Methods
    /**
     * Returns a newly allocated copy of this Action.
     *
     * @return a newly allocated copy of this Action.
     */
    virtual std::shared_ptr<CameraAction> clone() override;
    
    /**
     * Prepares a target for action
     *
     * The important state of the target should be allocated and stored in the given
     * state reference. The semantics of this state is action-dependent.
     *
     * @param target    The node to act on
     * @param state     The address to store the node state
     */
    virtual void start(const std::shared_ptr<Camera>& target, void** state) override;

    /**
     * Cleans up a target after an action
     *
     * The target state in the given address should be deallocated, just as it was
     * allocated in {@link #start}. The semantics of this state is action-dependent.
     *
     * @param target    The node to act on
     * @param state     The address to deallocate the node state
     */
    virtual void stop(const std::shared_ptr<Camera>& target, void** state) override;
    
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
    virtual void update(const std::shared_ptr<Camera>& target, void* state, float dt) override;
    
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
    virtual std::string toString(bool verbose = false) const override;
};

#endif /* __CAMERA_MOVE_ACTION_H__ */

