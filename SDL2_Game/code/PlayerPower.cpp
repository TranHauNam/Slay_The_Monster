#include"stdafx.h"
#include"PlayerPower.h"

PlayerPower::PlayerPower() {
	//number_ = 0;
}

PlayerPower::~PlayerPower() {

}

void PlayerPower::add_pos(const int& x_pos) {
	pos_list_.push_back(x_pos);//các vị trí thêm hình ảnh sinh mạng và đồng tiền
}

void PlayerPower::Init(SDL_Renderer* screen) {
	LoadImg("img//heart.png", screen);
	//number_ = 3;
	if (pos_list_.size() > 0) {
		pos_list_.clear();
		
	}
	add_pos(20);
	add_pos(60);
	add_pos(100);
}

void PlayerPower::Show(SDL_Renderer* screen) {
	for (int i = 0; i < pos_list_.size(); i++) {
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}

void PlayerPower::Decrease() {
	//number_--;
	pos_list_.pop_back();

}


void PlayerPower::InitCrease() {
	//number_++;
	int last_pos = pos_list_.back();
	last_pos += 40;
	pos_list_.push_back(last_pos);
}

PlayerMoney::PlayerMoney() {
	x_pos_ = 0;
	y_pos_ = 0;
}

PlayerMoney::~PlayerMoney() {

}

void PlayerMoney::Init(SDL_Renderer* screen) {
	bool ret = LoadImg("img//money_img.png", screen);
}

void PlayerMoney::Show(SDL_Renderer* screen) {
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}