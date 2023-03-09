#ifndef __CAMERA_MANAGER_H__
#define __CAMERA_MANAGER_H__

#include "CameraAction.h"
#include <SDL.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cugl/cugl.h>
using namespace cugl;
using namespace std;

/**
 * This class provides an action manager for instantiating animations.
 *
 * To create an animation, the manager attaches an action to a scene graph node
 * via a key.  This key allows the user to pause an animation or query when it
 * is complete. Each update frame, the manager moves the animation further along
 * until it is complete.
 *
 * An action manager is not implemented as a singleton.  However, you typically
 * only need one manager per application.
 */
class CameraManager {
#pragma mark ActionInstance
    /**
     * This internal class represents and action being actively animated.
     *
     * The instance contains the state of an action assigned to a node including
     * duration, elapsed time, the target, the interpolation function, and any
     * internal state. This class is only meant to be used by CameraManager, not
     * directly by the user.
     *
     * Because this is an internal class, it is used as a struct.  There are
     * no methods other than the constructor.
     */
    class ActionInstance {
    public:
        /** The node the action is performed on */
        std::shared_ptr<Camera> target;
        
        /** The action template associated with this instance */
        std::shared_ptr<CameraAction> action;
        
        /** The interpolation function on [0,1] to allow non-linear behavior */
        std::function<float(float)> interpolant;
        
        /** Any internal state needed by this action */
        void* state;
        
        /* The desired completion time of the action */
        float duration;
        
        /** The execution time since initialization */
        float elapsed;
        
        /** Whether or not this instance is currently paused */
        bool  paused;
        
    public:
        /**
         * Creates a new degenerate ActionInstance on the stack.
         *
         * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
         * the heap, use one of the static constructors instead.
         */
        ActionInstance() : state(NULL), duration(0.0f), elapsed(0.0f), paused(false) {}
        
        /**
         * Deletes this action instance, disposing all resources
         */
        ~ActionInstance();
    };
    
#pragma mark Values
protected:
    /** A map that associates nodes with their (multiple) animations */
    std::unordered_map<Camera*, std::unordered_set<std::string>> _keys;
    
    /** A map that associates keys with animations */
    std::unordered_map<std::string, ActionInstance*> _actions;
    

public:
#pragma mark Constructors
    /**
     * Creates a new degenerate CameraManager on the stack.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
     * the heap, use one of the static constructors instead.
     */
    CameraManager() {}
    
    /**
     * Deletes this action manager, disposing all resources
     */
    ~CameraManager() { dispose(); }
    
    /**
     * Disposes all of the resources used by this action manager.
     *
     * A disposed action manager can be safely reinitialized. Any animations
     * owned by this action manager will immediately stop and be released.
     */
    void dispose();
    
    /**
     * Initializes an action manager.
     *
     * @return true if initialization was successful.
     */
    bool init() { return true; }
    
#pragma mark Static Constructors
    /**
     * Returns a newly allocated action manager.
     *
     * @return a newly allocated action manager.
     */
    static std::shared_ptr<CameraManager> alloc() {
        std::shared_ptr<CameraManager> result = std::make_shared<CameraManager>();
        return (result->init() ? result : nullptr);
    }

#pragma mark -
#pragma mark Action Management
    /**
     * Returns true if the given key represents an active animation
     *
     * @param key       The identifying key
     *
     * @return true if the given key represents an active animation
     */
    bool isActive(std::string key) const;
    
    /**
     * Actives an animation with the given target and action
     *
     * This method will fail if the provided key is already in use.
     *
     * @param key       The identifying key
     * @param action    The action to animate with
     * @param target    The node to animate on
     *
     * @return true if the animation was successfully started
     */
    bool activate(std::string key,
                  const std::shared_ptr<CameraAction>& action,
                  const std::shared_ptr<Camera>& target) {
        return activate(key,action,target, nullptr);
    };


    /**
     * Actives an animation with the given target and action
     *
     * The easing function allows for effects like bouncing or elasticity in
     * the linear interpolation. If null, the animation will use the standard
     * linear easing.
     *
     * This method will fail if the provided key is already in use.
     *
     * @param key       The identifying key
     * @param action    The action to animate with
     * @param target    The node to animate on
     * @param easing    The easing (interpolation) function
     *
     * @return true if the animation was successfully started
     */
    bool activate(std::string key,
                  const std::shared_ptr<CameraAction>& action,
                  const std::shared_ptr<Camera>& target,
                  std::function<float(float)> easing);
    
    /**
     * Removes the animation for the given key.
     *
     * This act will immediately stop the animation.  The animated node will
     * continue to have whatever state it had when the animation stopped.
     *
     * If there is no animation for the give key (e.g. the animation is complete)
     * this method will return false.
     *
     * @param key       The identifying key
     *
     * @return true if the animation was successfully removed
     */
    bool remove(std::string key);

    /**
     * Updates all non-paused animations by dt seconds
     *
     * Each animation is moved forward by dt second.  If this causes an animation
     * to reach its duration, the animation is removed and the key is once
     * again available.
     *
     * @param dt    The number of seconds to animate
     */
    void update(float dt);

#pragma mark -
#pragma mark Pausing
    /**
     * Returns true if the animation for the given key is paused
     *
     * This method will return false if there is no active animation with the
     * given key.
     *
     * @param key       The identifying key
     *
     * @return true if the animation for the given key is paused
     */
    bool isPaused(std::string key);

    /**
     * Pauses the animation for the given key.
     *
     * If there is no active animation for the given key, or if it is already
     * paused, this method does nothing.
     *
     * @param key       The identifying key
     */
    void pause(std::string key);

    /**
     * Unpauses the animation for the given key.
     *
     * If there is no active animation for the given key, or if it is not
     * currently paused, this method does nothing.
     *
     * @param key       The identifying key
     */
    void unpause(std::string key);

#pragma mark -
#pragma mark Node Management
    /**
     * Removes all animations for the given target.
     *
     * If the target has no associated animations, this method does nothing.
     *
     * @param target    The node to stop animating
     */
    void clearAllActions(const std::shared_ptr<Camera>& target);
    
    /**
     * Pauses all animations for the given target.
     *
     * If the target has no associated animations, or if all of its animations
     * are already paused, this method does nothing.
     *
     * @param target    The node to pause animating
     */
    void pauseAllActions(const std::shared_ptr<Camera>& target);

    /**
     * Unpauses all animations for the given target.
     *
     * If the target has no associated animations, or if none of its animations
     * are currenly paused, this method does nothing.
     *
     * @param target    The node to pause animating
     */
    void unpauseAllActions(const std::shared_ptr<Camera>& target);

    /**
     * Returns the keys for all active animations of the given target
     *
     * The returned vector is a copy of the keys.  Modifying it has no affect
     * on the underlying animation.
     *
     * @param target    The node to query animations
     *
     * @return the keys for all active animations of the given target
     */
    std::vector<std::string> getAllActions(const std::shared_ptr<Camera>& target) const;

};
    
#endif /* __CAMERA_MANAGER_H__ */


