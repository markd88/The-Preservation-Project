//
//  common.h
//  Tilemap
//
//  Created by Yichen Qiu on 3/28/23.
//

#ifndef COMMON_H
#define COMMON_H


using namespace std;
enum ActiveScene { LOADING, MENU, GAMEPLAY };

// for controlling the scene transition
extern ActiveScene nextScene;
extern ActiveScene curScene;


// menu tells which level the user picked
extern int level;
#endif
