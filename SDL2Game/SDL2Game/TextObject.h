#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include"CommonFunc.h"
using namespace std;
class TextObject {
public:
	enum TextColor {// các màu của text
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2,
	};

	TextObject();
	~TextObject();

	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
	void Free();
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);//truyền trực tiếp các giá trị màu
	void SetColor(int type);//truyền các màu cơ bản trong enum

	void RenderText(SDL_Renderer* screen, int x_pos, int y_pos, SDL_Rect* clip = NULL, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);//hiển thị text lên màn hình
	int get_widht() const {
		return width_;
	}
	int get_height() const {
		return height_;
	}
	void set_text(const string& str_val) {
		str_val_ = str_val;
	}
	string get_text()const {
		return str_val_;
	}

private:
	string str_val_;// nội dung của text
	SDL_Color text_color;// màu sắc của text
	SDL_Texture* texture_;
	//chiều rộng và chiều cao của text;
	int width_;
	int height_;
	SDL_Rect rect;


};

#endif