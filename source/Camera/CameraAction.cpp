#include "CameraAction.h"
#include <string>

using namespace cugl;
using namespace cugl::scene2;

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
std::string CameraAction::toString(bool verbose) const {
    return std::string("Undefined Action");
}


/**
 * Returns a newly allocated copy of this Action.
 *
 * @return a newly allocated copy of this Action.
 */
std::shared_ptr<CameraAction> CameraAction::clone() {
    auto action = std::make_shared<CameraAction>();
    return action;
}


