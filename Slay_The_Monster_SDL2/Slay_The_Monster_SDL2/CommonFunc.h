
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include<Windows.h>
#include<string>
#include<vector>
#include<fstream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include"TextObject.h"

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
static SDL_Renderer* g_img_menu = NULL;
static Mix_Chunk* g_sound_bullet;
static Mix_Chunk* g_sound_explosion;
static Mix_Chunk* g_sound_main_die;
static Mix_Chunk* g_sound_main_jump;
static Mix_Chunk* g_sound_main_coins;
static Mix_Chunk* g_sound_track;
static bool win;

static int num_heart=3;

const int FRAMER_PER_SECOND = 30;// fps,số frame load được trong 1s
//screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 255;

#define BLANK_TILE 0//trạng thái = 0
//kích thước ô
#define TILE_SIZE 64
//kích thước file
#define MAX_MAP_X 400//chiều rộng
#define MAX_MAP_Y 10//chiều dài
#define STATE_MONEY 7//chỉ số của tile map tiền
//struct thể hiện trạng thái, vị trí, chỉ số của ô nào đó
struct Map {
	// vị trí của ô
	int start_x_;
	int start_y_;
	// chỉ số của ô
	int max_x_;
	int max_y_;
	//mảng 2 chiều lưu trạng thái của các ô
	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_;//ten file
};

struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;//nhảy
};

namespace SDLCommonFunc {
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}


#endif
