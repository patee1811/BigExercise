
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 20

class TileMat : public BaseObject // doi tuong ke thua tu baseobject
{
public:
    TileMat() {;}
    ~TileMat() {;}

};

class GameMap
{
public:
    GameMap() {;}
    ~GameMap() {;}

    void LoadMap(char* name); // Doc thong tin trong file text map
    void LoadTiles(SDL_Renderer* screen); // load anh cho tile map
    void DrawMap(SDL_Renderer* screen); // Xay dung map
    Map getMap() const {return game_map_;};
    void SetMap(Map& map_data) {game_map_ = map_data;};

private:
    Map game_map_;
    TileMat tile_mat[MAX_TILES];

};


#endif // GAME_MAP_H
