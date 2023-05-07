#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_

#include"CommonFunc.h"
#include"BaseObject.h"

class PlayerPower :public BaseObject {
public:
	PlayerPower();
	~PlayerPower();

	void add_pos(const int& x_pos);
	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);//Load ảnh

	void InitCrease();//tăng mạng
	void Decrease();// giảm mạng
private:
	vector<int> pos_list_;
};

class PlayerMoney : public BaseObject {
public:
	PlayerMoney();
	~PlayerMoney(); 

	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);//Load ảnh
	void set_pos(const int& x_pos, const int& y_pos) {
		x_pos_ = x_pos;
		y_pos_ = y_pos;
	}


private:
	int x_pos_;
	int y_pos_;

};

#endif 

