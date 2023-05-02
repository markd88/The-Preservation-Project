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
#define WALL_FIELD          "wall"
#define OBS_FIELD           "obs"

/** Artifact specific fields */
#define ITEM_FIELD          "item"
#define RESOURCE_FIELD      "resource_clock"

/** Character specific fields */
#define CHARACTER_FIELD         "character"

/** Guard specific fields */
#define GUARD_FIELD         "guard"

#define MAIN_SOUND_FIELD    "main-sound"
#define LEFT_SOUND_FIELD    "left-sound"
#define RIGHT_SOUND_FIELD   "right-sound"

/** The source for level files */
#define LEVEL_ONE_PAST_FILE      "tileset/levels/level-1/level-1-past.json"
#define LEVEL_ONE_PRESENT_FILE      "tileset/levels/level-1/level-1-present.json"

#define LEVEL_TWO_PAST_FILE      "tileset/levels/level-2/level-2-past.json"
#define LEVEL_TWO_PRESENT_FILE      "tileset/levels/level-2/level-2-present.json"

#define LEVEL_THREE_PAST_FILE      "tileset/levels/level-3/level-3-past.json"
#define LEVEL_THREE_PRESENT_FILE      "tileset/levels/level-3/level-3-present.json"

#define LEVEL_FOUR_PAST_FILE      "tileset/levels/level-4/level-4-past.json"
#define LEVEL_FOUR_PRESENT_FILE      "tileset/levels/level-4/level-4-present.json"

#define SAVED_GAME_FILE      "json/savedGame.json"

/** The key for loaded levels */
#define LEVEL_ONE_PAST_KEY       "level-1-past"
#define LEVEL_ONE_PRESENT_KEY       "level-1-present"
#define LEVEL_TWO_PAST_KEY       "level-2-past"
#define LEVEL_TWO_PRESENT_KEY       "level-2-present"
#define LEVEL_THREE_PAST_KEY       "level-3-past"
#define LEVEL_THREE_PRESENT_KEY       "level-3-present"
#define LEVEL_FOUR_PAST_KEY       "level-4-past"
#define LEVEL_FOUR_PRESENT_KEY       "level-4-present"

#define SAVED_GAME_KEY      "savedGame"

#endif /* LevelConstants_h */
