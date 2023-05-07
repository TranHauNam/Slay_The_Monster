#include"stdafx.h"
#include"BulletObject.h"

BulletObject::BulletObject() {
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = TYPE_0;
}
BulletObject::~BulletObject() {

}

void BulletObject::HandleMove(const int& x_border, const int& y_border) {
	if (bullet_dir_ == DIR_RIGHT) {
		rect_.x += x_val_; //vị trí đối tượng hình ảnh hiển thị màn hình theo chiều ngang
		if (rect_.x > x_border) {
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT) {
		rect_.x -= x_val_;
		if (rect_.x < 0) {
			is_move_ = false;
		}
	}
}

bool BulletObject::LoadImageBullet(SDL_Renderer* des) {
	bool ret = false;
	if (bullet_type_ == TYPE_0) {
		ret = LoadImg("img//player_bullet.png", des);
	}
	else if (bullet_type_ == TYPE_1) {
		ret = LoadImg("img//player_bullet1.jpg", des);
	}
	else if (bullet_type_ == TYPE_2) {
		ret = LoadImg("img//player_bullet2.png", des);
	}
	return ret;
}
