#ifndef EXPLOSION_OBJECT_H_
#define EXPLOSION_OBJECT_H_

#include"CommonFunc.h"
#include"BaseObject.h"
using namespace std;

class ExplosionObject :public BaseObject {
public:
	ExplosionObject();
	~ExplosionObject();

	void set_clip();
	void set_frame(const int& frame) {
		frame_ = frame;
	}
	virtual bool LoadImg(string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	int get_frame_width() {
		return frame_width_;
	}
	int get_frame_height() {
		return frame_height_;
	}
private:
	int frame_width_;
	int frame_height_;
	int frame_;
	SDL_Rect frame_clip_[8];

};



#endif