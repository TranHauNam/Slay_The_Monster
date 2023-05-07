#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include"CommonFunc.h"
#include"BaseObject.h"
//hình ảnh map
#define MAX_TILES 100
class TileMat : public BaseObject {
public:
	TileMat(){;}
	~TileMat(){;}
};
//quản lí hình ảnh , đẩy các hình ảnh map vào vị trí các ô vuông
class GameMap {
public:
	GameMap(){;}
	~GameMap(){;}
	
	void LoadMap(char* name);// đọc toàn bộ thông tin từ file text map
	void LoadTiles(SDL_Renderer* screen);//load toàn bộ hình ảnh cho tilemap Vd : hình ảnh 1 là 1.png,2 là 2.png,....
	void DrawMap(SDL_Renderer* screen); //đẩy các hình ảnh vào các vị trí   
	Map getMap() const { return game_map_; };
	void SetMap(Map& map_data) {
		game_map_ = map_data;
	};
private:
	Map game_map_;//chứa all thông tin trạng thái, vị trí, chỉ số;
	TileMat tile_mat[MAX_TILES];// mảng chứa 20 hình ảnh của map tương ứng 20 bản đồ khác nhau
	Input input_type_;
};

#endif

