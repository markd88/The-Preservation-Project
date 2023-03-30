//
//  LevelConstants.h
//  tilemap-ios
//
//  Created by Ann Zhou on 3/19/23.
//

#ifndef LevelConstants_h
#define LevelConstants_h

/** The global fields of the level model */
#define MAP_WIDTH           "width"
#define MAP_HEIGHT          "height"
#define TILE_WIDTH          "tilewidth"
#define TILE_HEIGHT         "tileheight"

/** The drawing fields for each object */
#define TEXTURE_FIELD       "texture"

/** The character specific fields */
#define MAIN_SOUND_FIELD    "main-sound"
#define LEFT_SOUND_FIELD    "left-sound"
#define RIGHT_SOUND_FIELD   "right-sound"

/** Map specific fields */
#define TILEMAP_FILED       "tilemap"

/** Wall specific fields */
#define WALLS_FIELD         "wall"

/** Artifact specific fields */
#define ARTIFACTS_FIELD     "artifact"

/** Guard specific fields */
#define MAIN_SOUND_FIELD    "main-sound"
#define LEFT_SOUND_FIELD    "left-sound"
#define RIGHT_SOUND_FIELD   "right-sound"

/** The source for level files */
#define LEVEL_ZERO_PAST_FILE      "tileset/levels/level-0-past/level-0-past.json"
#define LEVEL_ZERO_PRESENT_FILE      "tileset/levels/level-0-present/level-0-present.json"
#define LEVEL_ONE_PAST_FILE      "tileset/levels/level-1-past/level-1-past.json"
#define LEVEL_ONE_PRESENT_FILE      "tileset/levels/level-1-present/level-1-present.json"

/** The key for loaded levels */
#define LEVEL_ZERO_PAST_KEY       "level-0-past"
#define LEVEL_ZERO_PRESENT_KEY       "level-0-present"
#define LEVEL_ONE_PAST_KEY       "level-1-past"
#define LEVEL_ONE_PRESENT_KEY       "level-1-present"

#endif /* LevelConstants_h */
