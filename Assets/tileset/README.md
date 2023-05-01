# Designers:

When you create a new level, please follow the instructions below: 

1. Follow level-0 folder structure

2. When creating a new map, import the .tsj tileset under /levels.
	- Note: the .tsj tileset files were specified by the developer. If you want to add new tile or image, please contact developer first. 

3. Use non-Tiled mode (create **object layer**) to create a map only. 
	- Tutorial: [Tiled Map Editor Tutorial Part 5: Non-Tile Maps](https://www.youtube.com/watch?time_continue=2&v=ORhC48773bw&embeds_euri=https%3A%2F%2Fgamefromscratch.com%2Ftiled-map-editor-tutorial-series%2F&feature=emb_logo)

4. After you finish your design, please use "Export As": level-X-present.json
	- Note: the save button (Ctrl + S) won't generate the "type" of each object to the JSON file, so you have to use "Export As..." after you complete your work. 

# Programmers:

When a new asset is provided by a desinger, you need to do the following: 

1. Open tpp-tileset.tsj 
2. Add the asset (image) to the tpp-tileset.tsj 
3. Add the "Class" (i.e. the Image name)
4. Push the updated tpp-tileset.tsj to GitHub for designers to use it

When you want to import a newly added object layer with newly added assets to XCode project: 

1. Under LevelModel.cpp, write a new loadX() function
2. Add the newly added asset names to /Assets/json/assets.json under textures
3. Add the newly added assets (images) under /Assets/textures

# Example: LEVEL 0

## map size: 
width: 20
height: 20

## tile size: 
width: 64 px
height: 64 px
