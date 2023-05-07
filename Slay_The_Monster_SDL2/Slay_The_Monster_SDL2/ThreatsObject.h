#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include"CommonFunc.h"
#include"BaseObject.h"
#include"BulletObject.h"
using namespace std;

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define THREAT_SPEED 3

class ThreatsObject :public BaseObject
{
public:
	ThreatsObject();
	~ThreatsObject();

	enum TypeMove {
		STATIC_THREAT = 0,//không di chuyển
		MOVE_IN_SPACE_THREAT = 1,//có di chuyển
		
	};

	void set_x_val(const float& x_val) {
		x_val_ = x_val;
	}
	void set_y_val(const float& y_val) {
		y_val_ = y_val;
	}
	void set_x_pos(const float& x_pos) {
		x_pos_ = x_pos;
	}
	void set_y_pos(const float& y_pos) {
		y_pos_ = y_pos;
	}
	float get_x_pos() {
		return x_pos_;
	}
	float get_y_pos() {
		return y_pos_;
	}
	void SetMapXY(const int& map_x, const int& map_y) {
		map_x_ = map_x;
		map_y_ = map_y;
	}
	int get_width_frame() {
		return width_frame_;
	}
	int get_height_frame() {
		return height_frame_;
	}
	void set_clips();//set từng frame từ 0 đến 8
	bool LoadImg(string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void DoPlayer(Map& gMap);// xử lí dịch chuyển, đứng yên, va chạm bản đồ
	void CheckToMap(Map& gMap);

	void set_type_move(const int& type_move) {
		type_move_ = type_move;
	}
	void set_animation(const int& pos_a, const int& pos_b) {
		animation_a_ = pos_a;
		animation_b_ = pos_b;
	}
	void set_input_left(const int& p_left) {
		input_type_.left_ = p_left;
	}
	void ImpMoveType(SDL_Renderer* screen);// xử lí vấn đề di chuyển từ a đến b  
	vector<BulletObject*> get_bullet_list() {
		return bullet_list_;
	}
	void set_bullet_list(vector<BulletObject*>& bullet_list) {
		bullet_list_ = bullet_list;
	}
	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);//tạo ra các thông số của bullet
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);// x_limit và y_limit là giới hạn của đạn có thể bắn đến
	void RemoveBullet(const int& idx);//vị trí viên đạn thứ bao nhiêu được xóa
	SDL_Rect GetRectFrame();
private:
	int frame_;//chỉ số frame
	//kích thước từng frame
	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];//mảng lưu 8 frame
	bool on_ground_;//nằm trên mặt đất chưa
	int come_back_time_;
	float x_pos_, y_pos_;//vị trí hiện tại
	float x_val_, y_val_;//di chuyển 1 đoạn bao nhiêu
	int map_x_, map_y_;//giới hạn của bản đồ để check với map
	int type_move_;//kiểu di chuyển
	//giới hạn di chuyển từ a đến b
	int animation_a_;
	int animation_b_;
	Input input_type_;//quán lí việc di chuyển
	vector<BulletObject*> bullet_list_;




};



#endif
