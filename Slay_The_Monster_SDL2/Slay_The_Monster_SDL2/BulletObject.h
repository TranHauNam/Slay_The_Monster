#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include"BaseObject.h"
#include"CommonFunc.h"

class BulletObject :public BaseObject {
public:

	BulletObject();
	~BulletObject();

	enum BulletDir {
		DIR_RIGHT = 10,
		DIR_LEFT = 11,
	};

	enum BulletType {
		TYPE_0 = 20,
		TYPE_1 = 21,
		TYPE_2 = 22,
	};
	
	void set_x_val(const int& x_val) {
		x_val_ = x_val;
	}
	void set_y_val(const int& y_val) {
		y_val_ = y_val;
	}
	int get_x_val() {
		return x_val_;
	}
	int get_y_val() {
		return y_val_;
	}
	void set_is_move(const bool& is_move) {
		is_move_ = is_move;
	}
	bool get_is_move() {
		return is_move_;
	}
	void set_bullet_dir(const int& bullet_dir) {
		bullet_dir_ = bullet_dir;
	}
	int get_bullet_dir() {
		return bullet_dir_;
	}
	void set_bullet_type(const int& bullet_type) {
		bullet_type_ = bullet_type;
	}
	int get_bullet_type() {
		return bullet_type_;
	}
	void HandleMove(const int& x_border,const int& y_border); //xử lí vấn đề di chuyển của viên đạn, x_border và y_border là giới hạn của viên đạn đạt đến theo chiều ngang và chiều dọc
	bool LoadImageBullet(SDL_Renderer* des);
private:
	int x_val_;//đạn di chuyển sang trái sang phải 1 đoạn bao nhiêu
	int y_val_;//đạn di chuyển lên xuống 1 đoạn bao nhiêu
	bool is_move_;//trạng thái hiển thị ở trên màn hình
	int bullet_dir_;//hướng của đạn
	int bullet_type_;//loại đạn

};




#endif 