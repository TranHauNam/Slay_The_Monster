#include"stdafx.h"
#include"ThreatsObject.h"

ThreatsObject::ThreatsObject() {
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	on_ground_ = false;
	come_back_time_ = 0;
	frame_ = 0;

	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 1;
	type_move_ = STATIC_THREAT;

}
ThreatsObject::~ThreatsObject() {

}

bool ThreatsObject::LoadImg(string path, SDL_Renderer* screen) {
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true) {
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}
	return ret;
}

SDL_Rect ThreatsObject::GetRectFrame()
{
  SDL_Rect rect;
  rect.x = rect_.x;
  rect.y = rect_.y;
  rect.w = rect_.w/8;
  rect.h = rect_.h;
  return rect;
}

void ThreatsObject::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < 8; i++) {
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}
void ThreatsObject::Show(SDL_Renderer* des) {
	if (come_back_time_ == 0) {
		rect_.x = x_pos_ - map_x_;//khi bản đồ cuốn theo nhân vật thì ta phải trừ đi lượng bị cuốn theo
		rect_.y = y_pos_ - map_y_;
		frame_++;// tự động load frame
		if (frame_ >= 8) {
			frame_ = 0;
		}

		SDL_Rect* current_clip = &frame_clip_[frame_];//hiện tại đang ở frame thứ mấy

		SDL_Rect renderQuad = { rect_.x,rect_.y,width_frame_,height_frame_ };//thể hiện đang ở tọa độ nào chiều dài chiều rộng là bao nhiêu

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);// dùng để đẩy all thông số từ p_object_ lên des với thông số là renderQuad và curren_clip

	}
}

void ThreatsObject::DoPlayer(Map& gMap) {
	if (come_back_time_ == 0) {
		x_val_ = 0;
		y_val_ += GRAVITY_SPEED;
		if (y_val_ >= MAX_FALL_SPEED) {
			y_val_ = MAX_FALL_SPEED;
		}
		if (input_type_.left_ == 1) {
			x_val_ -= THREAT_SPEED;
		}
		else if (input_type_.right_ == 1) {
			x_val_ += THREAT_SPEED;
		}

		CheckToMap(gMap);

	}
	else {
		come_back_time_--;
		if (come_back_time_ == 0) {
			x_val_ = 0;
			y_val_ = 0;
			if (x_pos_ >= 256) {
				x_pos_ -= 256;
				animation_a_ -= 256;
				animation_b_ -= 256;
			}
			else x_pos_ = 0;
			y_pos_ = 0;
			come_back_time_ = 0;
			input_type_.left_ = 1;
		}
	}
}

void ThreatsObject::CheckToMap(Map& map_data) {
	//giới hạn kiểm tra từ x1 đến x2 theo chiều x
	int x1 = 0;// ô thứ bao nhiêu
	int x2 = 0;

	//giới hạn kiểm tra từ y1 đến y2 theo chiều y
	int y1 = 0;
	int y2 = 0;

	// check theo chiều ngang
	int height_min = min(height_frame_, TILE_SIZE);//chiều cao nhỏ nhất

	x1 = (x_pos_ + x_val_) / TILE_SIZE;//vị trí của x1 đang ở ô thứ bao nhiêu
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;// nếu chia ra không dư thì nó vừa đúng ở vị trí của tile map vậy nên nếu check va chạm thì nó vừa đúng va chạm vậy nên phải -1

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y2 >= 0 && y2 < MAX_MAP_Y) {//kiểm tra xem vị trí của nhân vật có ở trong bản đồ hay không
		if (x_val_ > 0) {//nhân vật đang di chuyển sang bên phải
			if ((map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != STATE_MONEY&&map_data.tile[y1][x2] != 28) || (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != STATE_MONEY&&map_data.tile[y2][x2] != 28)) {
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
			}
		}
		else if (x_val_ < 0) {
			if ((map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != STATE_MONEY&&map_data.tile[y1][x1] != 28) || (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != STATE_MONEY&&map_data.tile[y2][x1] != 28)) {
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
			}
		}
	}
	//check theo chiều dọc
	int width_min = min(width_frame_, TILE_SIZE);//chiều rộng nhỏ nhất
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min - 1) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y2 >= 0 && y2 < MAX_MAP_Y) {
		if (y_val_ > 0) {//nhân vật đang rơi tự do
			if ((map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != STATE_MONEY&&map_data.tile[y2][x1] != 28) || (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != STATE_MONEY&&map_data.tile[y2][x2] != 28)) {
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= height_frame_ + 1;
				y_val_ = 0;
				on_ground_ = true;
			}
		}
		else if (y_val_ < 0) {//nhân vật đang nhảy
			if ((map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != STATE_MONEY&&map_data.tile[y1][x1] != 28) || (map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != STATE_MONEY&&map_data.tile[y1][x2] != 28)) {
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}

		}
	}
	x_pos_ += x_val_;
	y_pos_ += y_val_;
	if (x_pos_ < 0) {
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_) {
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

	if (y_pos_ > map_data.max_y_) {//rơi xuống vực thẳm
		come_back_time_ = 50;
	}
}

void ThreatsObject::ImpMoveType(SDL_Renderer* screen) {
	if (type_move_ == STATIC_THREAT) {// đang đứng yên
		;
	}
	else {
		if (on_ground_ == true) {// ở trên mặt đất
			if (x_pos_ > animation_b_) {//nếu đi quá về phía bên phải
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				LoadImg("img//threat_left.png", screen);
			}
			else if (x_pos_ < animation_a_) {//nếu đi quá về phía bên trái
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				LoadImg("img//threat_right.png", screen);
			}
		}
		else {
			if (input_type_.left_ == 1) {
				LoadImg("img//threat_left.png", screen);
			}
		}
	}
}

void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen) {
	if (p_bullet != NULL) {
		p_bullet->set_bullet_type(BulletObject::TYPE_0);
		bool ret = p_bullet->LoadImageBullet(screen);
		if (ret == true) {
			p_bullet->set_is_move(true);
			p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
			p_bullet->SetRect(rect_.x + 5, y_pos_ + 30);
			p_bullet->set_x_val(15);
			bullet_list_.push_back(p_bullet);
		}
	}
}

void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit) {
	for (int i = 0; i < bullet_list_.size(); i++) {
		BulletObject* p_bullet = bullet_list_.at(i);
		if (p_bullet != NULL) {
			if (p_bullet->get_is_move()) {// có hiển thị trên màn hình
				int bullet_distance = rect_.x +width_frame_- p_bullet->GetRect().x;
				if (bullet_distance < 400 && bullet_distance>0) {
					p_bullet->HandleMove(x_limit, y_limit);
					p_bullet->Render(screen);
				}
				else {
					p_bullet->set_is_move(false);
				}
			}
			else {// đi quá màn hình
				p_bullet->set_is_move(true);// bắn nhiều lần
				p_bullet->SetRect(rect_.x + 5, y_pos_ + 20);// vị trí đạn hiển thị đầu tiên
			}
		}
	}
}

void ThreatsObject::RemoveBullet(const int& idx) {
	int size = bullet_list_.size();
	if (size > 0 && idx < size) {//nếu băng đạn có đạn và vị trí viên đạn nhỏ hơn số đạn trong băng thì ta xóa
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);
		if (p_bullet) {
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

