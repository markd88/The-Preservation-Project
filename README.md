# MVC and Procedural Generation

## Part 1: IDE

What IDE did you use to complete this project?
Answer: XCode

## Part 2: CRC Cards

For this project we will provide CRC Cards for you in the following format.

### Some Module
#### SomeModel/SomeView/SomeController
Description: Does something.

Justification: We need something.

| Responsibility | Collaborators |
|----------------|---------------|
| Does something from something else | SomethingElseController |
| TODO | TODO |

and we will leave TODOs for where we expect you to fill in content. Take the time to fill this out at the end of each section. Note, the `Tilemap` and `Tile` modules should reflect the `MVC` group. Also note `TileApp` is omitted (you will need to include this for your game's CRC cards).

As per our convention, we typically omit CUGL classes from collaborators.  That is because you are not writing those classes, and collaboration is a way to organize your team.  However, in cases where the CUGL class is really important (like a PolygonNode), we include them.


### Input Module
#### InputModel
Description: Stores input state related to the mouse.

Justification: We need to store time-based information about the mouse (holding, last location), as well as key presses. We also need a place to store when callbacks set state so we can align them with frame updates, particularly when frame rate is low.

| Responsibility | Collaborators |
|----------------|---------------|
| Get/Set callback initiated mouse click | None |
| Get/Set frame-based mouse click | None |
| Get/Set callback initiated mouse hold | None |
| Get/Set frame-based mouse house | None |
| Get/Set callback initiated mouse hold | None | (appeared before)
| Get/Set last mouse position | None |
| (TODO) | None |

#### InputController
Description: A singleton that sets up input devices and provides information about keyboard and mouse state.

Justification: We need to set up the keyboard and mouse to detect keypresses and mouseclicks.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize input devices | Keyboard, Mouse |
| Setup mouse callbacks | None |
| Align callback initiated state with frame-based state | InputModel |
| Get what key is currently pressed | InputModel |
| Get whether the mouse was clicked | InputModel |
| Get whether the mouse is held | InputModel |
| Get the mouse's last position | InputModel |

### Game Module
#### GameController
Description: The main controller which sets the tilemap to display certain templates.

Justification: We need a central controller that allows for regular and procedural generation of tilemaps with input presses.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize tilemap | TileMap |
| Initialize procedural generator | Generator Controller |
| Initialize input manager | InputController |
| Detect and satisfy template modification requests | InputController, TileMap |
| Generate a smiley face template | TilemapController |
| Generate a random tile template | TilemapController |
| Generate Perlin noise tiles | TilemapController |
| Generate colored Perlin noise tiles | TilemapController, TileMap |
| Generate Fractal Brownian Motion tiles | TilemapController, TileMapController |
| Increase/Decrease the size of the tilemap | TileMapController |
| Double/Halve the size of the tilemap | TileMapController |
| Increase/Decrease the dimensions of the tilemap | TileMapController |
| Invert the color of the tilemap and it's tiles | TileController |
| Log running times of template generation | chrono |

### Tilemap Module
#### TilemapModel
Description: Stores the state of the tilemap.

Justification: We need to store the tilemap's properties to allow for repositioning, increasing or decreasing its dimensions or size, and changing its color dynamically.

| Responsibility | Collaborators |
|----------------|---------------|
| Setter for position(left-buttom of tilemap) | None |
| Setter for dimension | None |
| Setter for Tilemap color | None |
| Setter for single-tile size | None |

#### TilemapView
Description: Sets the view state of the tilemap node.

Justification: We need to display the tilemap on the screen.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize tilemap node | PolygonNode |
| Get tilemap node | None |
| Add / Remove view nodes from the scene | Scene2 |
| Set position | PolygonNode |
| Set size | PolygonNode |
| Set color | PolygonNode |

#### TilemapController
Description: Maintains the tilemap's model and view and allows for modification. It also manages the tiles on the tilemap.

Justification: We need an interface to allow modifying the tilemap and we also need to associate a tilemap the many tiles that could be on it.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize model | TilemapModel|
| Initialize view | TilemapView |
| Initialize tiles | TileController |
| Update Position of the Tilemap in model and View | TilemapModel, TilemapView |
| Update Color of the Tilemap in model and View | TilemapModel, TilemapView |
| Add a tile to the tileMap in model and view | TileController, TilemapModel, TilemapView |
| Invert color of the tilemap | TilemapModel |
| Add / Remove view to the scene | TilemapView |
| Modify the tilemap size | TilemapModel |
| Update the dimensions | TilemapModel, TilemapView |
| Update the sizes of all tiles | TilemapModel, TilemapView, TileController |
| Initialize the tile map | TilemapModel |
| Clear the tile map | TilemapModel, TilemapView |

### Tile Module
#### TileModel
Description: Stores the state of the tile.

Justification: We need to store the tile's properties to allow for repositioning, increasing or decreasing its size, and changing its color dynamically.

| Responsibility | Collaborators |
|----------------|---------------|
| Get and Set Position | None |
| Get and Set Size | None |
| Get and Set Color | None |

#### TileView
Description: Sets the view state of the tile node.

Justification: We need to display tiles on the screen.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize tile node | TODO |
| Remove tile node from parent upon destruction | SceneNode |
| Add / Remove view nodes from its scenenode | SceneNode |
| Set Position of the corresponding tile | PolygonNode |
| Set Size of the corresponding tile | PolygonNode |
| Set Color of the corresponding tile | PolygonNode |

#### TileController
Description: Maintains the tile's model and view and allows for modification.

Justification: We need an interface to allow modifying tiles.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize tileModel and tileView object | tileModel, tileView |
| Update Position of tile | TileModel, TileView|
| Update Size of tile | TileModel, TileView |
| Update Color of tile | TileModel, TileView |
| TODO | TODO |
| Add / Remove view to a SceneNode | TileView |
| Invert tile color | TileModel |

### Generator Module
#### GeneratorModel
Description: Stores permutation tables for Perlin noise.

Justification: We need a place to store a seeded permutation table if the user decides not to use Ken's original permutation table.

| Responsibility | Collaborators |
|----------------|---------------|
| Get/Set permutation array | None |

#### GeneratorController
Description: Performs and returns any procedural generation calculations necessary.

Justification: We want to have a sub-controller where we can perform all procedural generation calculations to use for tilemap generation.

| Responsibility | Collaborators |
|----------------|---------------|
| Initialize model | GeneratorModel |
| Calculate and return 2D Perlin noise | None |
| Calculate and return 3D Perlin noise | None |
