//
//  LevelConstants.h
//  tilemap-ios
//
//  Created by Ann Zhou on 3/19/23.
//

#ifndef LevelConstants_h
#define LevelConstants_h

/** The global fields of the level model */
#define WALLS_FIELD         "wall"
#define TILEMAP_FILED       "tilemap"

/** The drawing fields for each object */
#define TEXTURE_FIELD       "texture"
#define DEBUG_COLOR_FIELD   "debugcolor"
#define DEBUG_OPACITY_FIELD "debugopacity"

/** The character specific fields */
#define THRUST_FIELD        "thrust"
#define MAIN_FLAMES_FIELD   "flames"
#define LEFT_FLAMES_FIELD   "flames-left"
#define RIGHT_FLAMES_FIELD  "flames-right"
#define MAIN_SOUND_FIELD    "main-sound"
#define LEFT_SOUND_FIELD    "left-sound"
#define RIGHT_SOUND_FIELD   "right-sound"

/** Wall specific fields */
#define VERTICES_FIELD      "polygon"
#define BOUNDARY_FIELD      "boundary"

/** Guard specific fields */
#define THRUST_FIELD        "thrust"
#define MAIN_FLAMES_FIELD   "flames"
#define LEFT_FLAMES_FIELD   "flames-left"
#define RIGHT_FLAMES_FIELD  "flames-right"
#define MAIN_SOUND_FIELD    "main-sound"
#define LEFT_SOUND_FIELD    "left-sound"
#define RIGHT_SOUND_FIELD   "right-sound"

/** The source for our level file */
//#define LEVEL_ONE_FILE      "json/level.json"
#define LEVEL_ZERO_FILE      "tileset/levels/level-0-primary/level-0-primary.json"

/** The key for our loaded level */
#define LEVEL_ZERO_KEY       "level0"


#endif /* LevelConstants_h */
