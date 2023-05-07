#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"
#include"game_map.h"
#include"MainObject.h"
#include"ImpTimer.h"
#include"ThreatsObject.h"
#include"ExplosionObject.h"
#include"TextObject.h"
#include"PlayerPower.h"
using namespace std;

BaseObject g_background;

TTF_Font* font_ = NULL;


bool InitData() {//khởi động thông số cho MT SDL
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);//thiết lập môi trường ban đầu cho SDL
	if (ret < 0) return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");// thiết lập chế độ tỉ lệ và chất lượng
	// tạo window
	g_window = SDL_CreateWindow("Slay The Monster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_window == NULL) success = false;
	else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) {
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
		}
		if (TTF_Init() == -1) {
			success = false;
		}
		font_ = TTF_OpenFont("font//font_.otf", 25);
		if (font_ == NULL) {
			success = false;
		}
	}

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1){/*22050,2,4096*/
		success=false;
	}
	g_sound_bullet = Mix_LoadWAV("sound//shoot1.wav");
	g_sound_explosion=Mix_LoadWAV("sound//explosion_threat.wav");
	g_sound_main_die=Mix_LoadWAV("sound//main_die.wav");
	g_sound_main_jump=Mix_LoadWAV("sound//jump.wav");
	//g_sound_main_footsteps=Mix_LoadWAV("b2.wav");
	g_sound_main_coins=Mix_LoadWAV("sound//coin.wav");
	g_sound_track=Mix_LoadWAV("sound//sound_track.wav");

	if(g_sound_explosion==NULL||g_sound_bullet==NULL||g_sound_main_die==NULL||g_sound_main_jump==NULL/*||g_sound_main_footsteps==NULL*/||g_sound_main_coins==NULL||g_sound_track==NULL){
		success=false;
	}

	return success;
}


bool LoadBackground() {
	bool ret = g_background.LoadImg("img//back_ground.png", g_screen);
	if (ret == false) return false;
	return true;

}

void close() {// giải phóng các đối tượng
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	IMG_Quit();
	SDL_QUIT;
}



vector<ThreatsObject*> MakeThreatsList() {// all mối hiểm họa
	vector<ThreatsObject*> list_threats;

	ThreatsObject* dynamic_threats = new ThreatsObject[40];//mảng chứa 50 mối hiểm họa di chuyển được
	for (int i = 0; i < 40; i++) {
		ThreatsObject* p_threats = (dynamic_threats + i);
		if (p_threats != NULL) {
			p_threats->LoadImg("img//threat_left.png", g_screen);
			p_threats->set_clips();
			p_threats->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threats->set_x_pos(500 + i * 600);//vị trí threats
			p_threats->set_y_pos(200);//rơi từ vị trí 100;
			p_threats->set_input_left(1);

			int pos_a = p_threats->get_x_pos() - 60;
			int pos_b = p_threats->get_x_pos() + 60;
			p_threats->set_animation(pos_a, pos_b);
			list_threats.push_back(p_threats);
		}
	}

	ThreatsObject* threats_object_ = new ThreatsObject[40];// mảng chứa 30 mối hiểm họa
	for (int i = 0; i < 40; i++) {
		ThreatsObject* p_threats = (threats_object_ + i);
		if (p_threats != NULL) {
			p_threats->LoadImg("img//threat_bullet.png", g_screen);
			p_threats->set_clips();
			p_threats->set_x_pos(700 + i * 600);//vị trí threats
			p_threats->set_y_pos(250);//rơi từ vị trí 100;
			p_threats->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threats->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threats->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threats);
		}
	}
	return list_threats;
}

int main(int argc, char* argv[]) {
	ImpTimer fps_timer;
	if (InitData() == false) {
		return -1;
	}
	if (LoadBackground() == false) {
		return -1;
	}

	GameMap game_map;
	game_map.LoadMap("map//map.dat");
	game_map.LoadTiles(g_screen);

	MainObject p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();

	PlayerPower player_power;
	player_power .Init(g_screen);

	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.set_pos(SCREEN_WIDTH * 0.5 - 75, 8);


	vector<ThreatsObject*> list_threats = MakeThreatsList();

	ExplosionObject exp_threat, exp_main;
	bool tRet = exp_threat.LoadImg("img//explosion.png", g_screen);
	bool pRet = exp_main.LoadImg("img//explosion.png", g_screen);
	if (tRet == false || pRet == false) {
		return -1;
	}
	exp_threat.set_clip();
	exp_main.set_clip();

	//int num_die = 0;// số lần bị chết

	//Time text
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);

	TextObject def_game;
	def_game.SetColor(TextObject::WHITE_TEXT);
	int def_val = 0;

	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);


	bool is_quit = false;

	Mix_PlayChannel(-1,g_sound_track,0);

	//viết vòng lặp vô hạn để background chạy vô hạn
	while (!is_quit) {
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_QUIT) {
				is_quit = true;
			}
			p_player.HandelInputAction(g_event, g_screen,g_sound_bullet,g_sound_main_jump/*g_sound_main_footsteps*/);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);
		g_background.Render(g_screen, NULL);

		Map map_data = game_map.getMap();

		p_player.HandelBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data,g_sound_main_coins);
		p_player.Show(g_screen);

		game_map.SetMap(map_data);//cập nhật lại vị trí mới cho start_x_ và start_y_;
		game_map.DrawMap(g_screen);//vẽ lại

		player_power.Show(g_screen);
		player_money.Show(g_screen);

		if(num_heart<3&&p_player.get_heart_count()>0){
						num_heart++;
						player_power.InitCrease();// tăng mạng
						player_power.Render(g_screen); 
		}
		p_player.set_heart_count(0);

		for (int i = 0; i < list_threats.size(); i++) {
			ThreatsObject* p_threat = list_threats.at(i);
			if (p_threat != NULL) {
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				p_threat->Show(g_screen);
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool ret1 = false;//biến để check xem giữa nhân vật và đạn của mối hiểm họa đã va chạm hay chưa
				vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();// đạn của mối hiểm họa
				// kiểm tra va chạm giữa viên đạn và nhân vật
				for (int jj = 0; jj < tBullet_list.size(); ++jj) {
					BulletObject* pt_bullet = tBullet_list.at(jj);
					if (pt_bullet) {
						SDL_Rect bl = pt_bullet->GetRect();
						ret1 = SDLCommonFunc::CheckCollision(bl, rect_player);//kiểm tra va chạm giữa các mối hiểm họa và nhân vật game

						if (ret1) {// bị bắn
							p_threat->RemoveBullet(jj);//xóa viên đạn
							break;
						}
					}
				}
				//kiểm tra va chạm giữa mối hiểm họa và nhân vật
				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool ret2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
				//ret1=0;
				//ret2=false;
				if (ret1 || ret2||p_player.get_rod()||p_player.get_encounter_obstacles()) {// nhân vật chết
					p_player.set_rod(false);
					p_player.set_encounter_obstacles(false);
					for (int ex = 0; ex < 4; ex++) {
						// vị trí nổ
						int x_pos = p_player.GetRect().x + p_player.get_frame_width() * 0.5 - exp_main.get_frame_width() * 0.5;
						int y_pos = p_player.GetRect().y + p_player.get_frame_height() * 0.5 - exp_main.get_frame_height() * 0.5;

						exp_main.set_frame(ex);
						exp_main.SetRect(x_pos, y_pos);
						exp_main.Show(g_screen);
						SDL_RenderPresent(g_screen);
					}
					Mix_PlayChannel(-1,g_sound_main_die,0);
					num_heart--;
					

					if (num_heart > 0) {// hồi sinh lại nhân vật
						p_player.SetRect(0, 0);
						p_player.set_come_back_time(50);// time quay trở lại
						SDL_Delay(1000); 
						player_power.Decrease();// giảm mạng
						player_power.Render(g_screen); 

						continue;
					}
					else {
						if (MessageBox(NULL, L"GAME OVER", L"INFO", MB_OK | MB_ICONSTOP) == IDOK) {
							p_threat->Free();
							close();
							SDL_Quit();
							return 0;
						}
					}
				}

				if(num_heart>3){
					num_heart=3;
				}

			}
		}
		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_height();

		vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for (int r = 0; r < bullet_arr.size(); ++r) {
			BulletObject* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL) {
				for (int t = 0; t < list_threats.size(); ++t) {
					ThreatsObject* obj_threat = list_threats.at(t);
					if (obj_threat != NULL) {
						//mối hiểm họa
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool ret = SDLCommonFunc::CheckCollision(bRect, tRect);

						if (ret == true) {// có va chạm
							//xảy ra vụ nổ trước khi xóa viên đạn
							def_val++;
							for (int ex = 0; ex < 4; ex++) {
								// vị trí nổ
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
							}
							p_player.RemoveBullet(r);//xóa viên đạn
							Mix_PlayChannel(-1,g_sound_explosion,0);
							obj_threat->Free();
							list_threats.erase(list_threats.begin() + t);// xóa nhân vật
						}
					}
				}
			}
		}

		//text time
		string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 200 - time_val;// đếm ngược
		if (val_time <= 0) {
			Mix_HaltMusic();
			//game_over
			if (MessageBox(NULL, L"GAME OVER", L"INFO", MB_OK | MB_ICONSTOP) == IDOK) {
				is_quit = true;
				break;
			}

		}
		else {// hiển thị time text
			string str_val = to_string(val_time);
			str_time += str_val;

			time_game.set_text(str_time);//nội dung hiển thị
			time_game.LoadFromRenderText(font_, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 160, 15);//hiển thị trên màn hình tại các vị trí
		}

		string val_str_def = to_string(def_val);
		string str_def = "Defeat: ";
		str_def += val_str_def;

		def_game.set_text(str_def);
		def_game.LoadFromRenderText(font_, g_screen);
		def_game.RenderText(g_screen, SCREEN_WIDTH - 160, 50);

		int money = p_player.get_money_count();
		string str_money=": ";
		string money_str = to_string(money);
		str_money+=money_str;
		money_game.set_text(str_money);
		money_game.LoadFromRenderText(font_, g_screen);
		money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 40, 15);


		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAMER_PER_SECOND;//milis
		// 1s=1000ms load đc 25 frame vậy 1 frame thì cần time_one_frame ms
		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time > 0) {
				SDL_Delay(delay_time);
			}
		}

		if(p_player.get_item_win()){
			if (MessageBox(NULL, L"VICTORY", L"INFO", MB_OK | MB_ICONSTOP) == IDOK) {
				is_quit = true;
				break;
			}
		}
	}


	for (int i = 0; i < list_threats.size(); i++) {
		ThreatsObject* p_threat = list_threats.at(i);
		if (p_threat) {
			p_threat->Free();
			p_threat = NULL;
		}
	}
	list_threats.clear();
	close();
	return 0;
}