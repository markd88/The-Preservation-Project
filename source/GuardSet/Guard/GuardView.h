//
//  GuardView.h
//  Tilemap
//
//  Created by Hao Chen on 3/12/23.
//

#ifndef GuardView_h
#define GuardView_h

#include <cugl/cugl.h>
using namespace cugl;

#include <math.h>



class GuardView{
private:
    /** Main character view */
    /** The node is attached to the root-scene*/
    std::shared_ptr<scene2::SpriteNode> _node;


    /** Manager to process the animation actions */
    std::shared_ptr<cugl::scene2::ActionManager> _actions;

    // questions mark
    std::shared_ptr<scene2::SpriteNode> _question_node;
    std::vector<int> q_anim = {1,2,3,4,5,6,7,0};
    std::shared_ptr<cugl::scene2::Animate> q_animation = cugl::scene2::Animate::alloc(q_anim, 1.0f);










#pragma mark Main Functions
public:
    /** contructor */
    GuardView(Vec2 position, Size size, Color4 color, const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::scene2::ActionManager> actions, bool isPast) {
        // Get the image and add it to the node.
        _actions = actions;
        float scale = GAME_WIDTH/size.width;
        // size *= scale;
        string a;

        if (isPast) {
            a = "guard_past";
        }
        else{
            a = "guard_present";
        }
        std::shared_ptr<Texture> guard  = assets->get<Texture>(a);
        _node = scene2::SpriteNode::allocWithSheet(guard, 16, 16, 256); // SpriteNode for animation
        _node->setScale(0.6f); // Magic number to rescale asset

        _node->setRelativeColor(false);
        _node->setVisible(true);
        _node->setAnchor(Vec2::ANCHOR_CENTER);
        _node->setPosition(position + _node->getSize()/2);


        std::shared_ptr<Texture> question = assets->get<Texture>("question_mark_anim");
        _question_node = scene2::SpriteNode::allocWithSheet(question, 1,8,8);
        _question_node->setVisible(false);
        _question_node->setRelativeColor(false);
        _node->addChildWithName(_question_node, "question_mark");

        // fixed, but should be from initialization( level editor)
        // _last_direction = 0;



    }
    
    ~GuardView(){
        auto parent = _node->getParent();
        if (parent != nullptr && _node != nullptr) {
            parent->removeChild(_node);
        }
    }
    
#pragma mark Scene Methods
public:
    /**
     * Adds the view components as children to the given `sceneNode`.
     *
     * @param sceneNode The scenenode to add the view to
     */
    void addChildTo(const std::shared_ptr<cugl::scene2::OrderedNode>& scene) {
        scene->addChild(_node);
        
    }
    
    /**
     * Removes the view component children from the given `sceneNode`.
     *
     * @param sceneNode The scenenode to remove the view from
     */
    void removeChildFrom(const std::shared_ptr<cugl::scene2::OrderedNode>& scene) {
        scene->removeChild(_node);
    }

#pragma mark Setters
public:
    void setPosition(Vec2 position){
        _node->setPosition(position);
    }
    
    void setSize(Size size){
        _node->setContentSize(size);
    }

    Vec2 nodePos(){
        return _node->getPosition();
    }
    
    Size nodeSize() {
        return _node->getSize();
    }
    
    void performAction(string actionName, const std::shared_ptr<cugl::scene2::MoveTo>& action){
        _actions->activate(actionName, action, _node);
        _question_node->setVisible(false);
    }

    void stopQuestionAnim(string id){
        // _actions->remove("question"+id);
        _question_node->setVisible(false);
    }

//    void startQuestionAnim(string id) {
//        if (_actions->isActive("question"+id)) {
//            // let it finish
//        }
//        else {
//            std::vector<int> frames;
//            for(int ii = 1 ; ii < 8; ii++) {
//                frames.push_back(ii);
//            }
//            frames.push_back(0);
//
//            std::shared_ptr<cugl::scene2::Animate> animation = cugl::scene2::Animate::alloc(frames, 1.0f);
//            _question_node->setVisible(true);
//            _actions->activate("question"+id, animation, _question_node);
//
//        }
//    }
//
    void startQuestionAnim(string id, float time){
        _question_node->setVisible(true);
        // CULog("%s  %f", id.c_str(), time);
        int num_frame = (time * 8) / 1000;
        if (num_frame == 8) {
            num_frame = 7;
        }
        _question_node->setFrame(num_frame);
    }

    void performAnimation(int current_d, string state, int last_direction, string last_state, string id) {
        //CULog("%d", d);
        if (_actions->isActive("guard_animation"+id) and current_d == last_direction and state == last_state) {
            // continue the current animation
            return;
        }

        if (_actions->isActive("guard_animation"+id) and (current_d != last_direction or state != last_state)) {
            _actions->remove("guard_animation"+id);
       //     CULog("remove current animation, start a new one curent d:%d last d: %d  current state: %s   last state: %s", current_d, last_direction, state.c_str(), last_state.c_str());
        }

        int direction;
        // walk or run or lookaround or static. this is the starting index in spritesheet
        int start_index;
        float duration;

        if (state == "chaseD" or state == "chaseSP") {
            // running
            start_index = 64;
            direction = current_d;
            duration = 0.5f;
        } else if (state == "patrol" or state=="return") {
            // walking
            start_index = 0;
            direction = current_d;
            duration = 1.0f;
        } else if (state == "lookaround") {
            // look around, need to use the last direction as the direction
            start_index = 192;
            direction = last_direction;
            duration = 1.0f;
        } else if (state == "static" or state == "question") {
            start_index = 128;
            direction = last_direction;
            duration = 1.0f;
        }
        // looping frames
        std::vector<int> frames;
        for(int ii = 1 + start_index + 8*direction; ii < 8 + start_index + 8*direction; ii++) {
            frames.push_back(ii);
        }
        frames.push_back(start_index + 8*direction);

        std::shared_ptr<cugl::scene2::Animate> animation = cugl::scene2::Animate::alloc(frames, duration);
//        CULog("animation cycle");
//        for(int i=0; i < frames.size(); i++) {
//            CULog( "%d",frames.at(i));
//        }
        _actions->activate("guard_animation"+id, animation, _node);

    }
    
#pragma mark Helpers
public:
    
   
    void setVisibility(bool visible){
        _node->setVisible(visible);

    }

    void updatePriority(){
        _node->setPriority(_node->getPosition().y);
    }

};

#endif /* GuardView_h */
