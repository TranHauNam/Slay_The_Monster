#include"stdafx.h"
#include"MainObject.h"
using namespace std;
MainObject::MainObject() {
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = -1;
    input_tyle_.left_ = 0;
    input_tyle_.right_ = 0;
    input_tyle_.up_ = 0;
    input_tyle_.down_ = 0;
    input_tyle_.jump_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
    money_count = 0;
	heart_count=0;
	rod_=false;
	encounter_obstacles_=false;
	item_win=false;
}

MainObject::~MainObject() {
    Free();
}

SDL_Rect MainObject::GetRectFrame() {
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = rect_.w / 8;
    rect.h = rect_.h;
    return rect;
}

bool MainObject::LoadImg(string path, SDL_Renderer* screen) {
    bool ret = BaseObject::LoadImg(path, screen);//kế thừa hàm bool LoadImg từ class BaseObject
    if (ret == true) {
        width_frame_ = rect_.w / 8;
        height_frame_ = rect_.h;
    }
    return ret;
}

void MainObject::set_clips() {
    //gán các giá trị của frame_clip_[i](0<=i<8)
    if (width_frame_ > 0 && height_frame_ > 0) {

        for (int i = 0; i < 8; i++) {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }

}

void MainObject::Show(SDL_Renderer* des) {
    UpdateImagePlayer(des);

    if (input_tyle_.left_ == 1 || input_tyle_.right_ == 1) {//nếu nhân vật đang chuyển động thì chuyển tới frame tiếp theo

        frame_++;
    }
    else {// nếu không chuyển động thì frame 0
        frame_ = 0;
    }

    if (frame_ >= 8) {
        frame_ = 0;
    }

    if (come_back_time_ == 0) {
        rect_.x = x_pos_ - map_x_;//khi bản đồ cuốn theo nhân vật thì ta phải trừ đi lượng bị cuốn theo
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* current_clip = &frame_clip_[frame_];//hiện tại đang ở frame thứ mấy

        SDL_Rect renderQuad = { rect_.x,rect_.y,width_frame_,height_frame_ };//thể hiện đang ở tọa độ nào chiều dài chiều rộng là bao nhiêu

        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);// dùng để đẩy all thông số từ p_object_ lên des với thông số là renderQuad và curren_clip

    }


}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen,Mix_Chunk* sound_bullet,Mix_Chunk* sound_jump/*Mix_Chunk* sound_footsteps*/) 
{

  //If a key was pressed
  if( events.type == SDL_KEYDOWN )
  {
    //Set the velocity
    switch( events.key.keysym.sym )
    {
    case SDLK_RIGHT: 
      {
        status_  = WALK_RIGHT;
        input_tyle_.right_ = 1;
		UpdateImagePlayer(screen);
	//	Mix_PlayChannel(-1,sound_footsteps,0);
      }
	  break;
    case SDLK_LEFT: 
      {
        status_ = WALK_LEFT;
        input_tyle_.left_ = 1;
        UpdateImagePlayer(screen);
	//	Mix_PlayChannel(-1,sound_footsteps,0);
      }
	  break;
    }
  }
  //If a key was released
  else if( events.type == SDL_KEYUP )
  {
    ////Set the velocity
      switch( events.key.keysym.sym )
      {
      case SDLK_RIGHT:{
		  input_tyle_.right_ = 0;
					  }
		  break;
      case SDLK_LEFT:{
		  input_tyle_.left_ = 0;
					 }
		  break;

      }
  }
  else if (events.type == SDL_MOUSEBUTTONDOWN) 
  {
     if (events.button.button == SDL_BUTTON_LEFT) 
     {
      BulletObject* p_bullet = new BulletObject();
      p_bullet->LoadImg("img//player_bullet2.png", screen);
	  Mix_PlayChannel(-1,sound_bullet,0);
      //p_bullet->set_type(BulletObject::SPHERE);

      if (status_ == WALK_LEFT)
      {
		  p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        p_bullet->SetRect(this->rect_.x, this->rect_.y + height_frame_*0.22);
      }
      else
      {
		  p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
        p_bullet->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_*0.22);
      }

      p_bullet->set_x_val(20);
      p_bullet->set_y_val(20);
      //p_bullet->set_move_type(BulletObject::SIN_TYPE);

      p_bullet->set_is_move(true);
      p_bullet_list_.push_back(p_bullet);
  }
    else if (events.button.button == SDL_BUTTON_RIGHT)
    {
        input_tyle_.jump_ = 1;
		Mix_PlayChannel(-1,sound_jump,0);
    }
  }
  else if (events.type == SDL_MOUSEBUTTONUP)
  {
    if (events.button.button == SDL_BUTTON_LEFT) 
    {
      ;//
    }
    else
    {
      ;//
    }
  }
}

void MainObject::HandelBullet(SDL_Renderer* des) {
    for (int i = 0; i < p_bullet_list_.size(); i++) {
        BulletObject* p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL) {// có tồn tại p_bullet
            if (p_bullet->get_is_move() == true) {//đạn đang được bắn
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);//hiển thị lên màn hình
            }
            else {//đạn đã ra ngoài màn hình
                if (p_bullet != NULL) {
                    p_bullet_list_.erase(p_bullet_list_.begin() + i);//xóa đạn
                    delete p_bullet;
                    p_bullet = NULL;
                }

            }
        }
    }
}

void MainObject::DoPlayer(Map& map_data,Mix_Chunk* sound_coins) {
    if (come_back_time_ == 0) {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;// tốc độ rơi

        if (y_val_ >= MAX_FALL_SPEED) {
            y_val_ = MAX_FALL_SPEED;// tốc độ rơi max
        }
        if (input_tyle_.left_ == 1) {//khi bấm trái
            x_val_ -= PLAYER_SPEED;// tốc độ di chuyển
        }
        else if (input_tyle_.right_ == 1) {//khi bấm phải
            x_val_ += PLAYER_SPEED;
        }
        if (input_tyle_.jump_ == 1) {
            if (on_ground_ == true) {//nhân vật đang ở trên mặt đất
                y_val_ = -PLAYER_JUMP_VAL;//nhảy
            }
            on_ground_ = false;//không cho nhảy nhiều lần
            input_tyle_.jump_ = 0;
        }
		CheckToMap(map_data,sound_coins);
        CenterEntityOnMap(map_data);
    }
    else {
        come_back_time_--;
        if (come_back_time_ == 0) {
            if (x_pos_ > 256) {// 256 = 4 TILE_MAP
                x_pos_ -= 256;
            }
            else {
                x_pos_ = 0;
            }
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
            on_ground_ = false;
        }
    }	
}

void MainObject::CenterEntityOnMap(Map& map_data) {
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);//tức là vị trí đầu tiên của map được thay đổi khi nhân vật di chuyển được nửa bản đồ, lúc này bản đồ sẽ cuốn theo ;
    if (map_data.start_x_ < 0) {//khi ta di chuyển sang bên trái
        map_data.start_x_ = 0;//không cho phép bản đồ di chuyển theo nhân vật sang bên trái nữa
    }
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) {//nếu nó	lớn hơn vị trí lớn nhất của bản đồ thì không cho bản đồ di chuyển thêm nữa
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);//tức là vị trí đầu tiên của map được thay đổi khi nhân vật di chuyển được nửa bản đồ, lúc này bản đồ sẽ cuốn theo ;
    if (map_data.start_y_ < 0) {
        map_data.start_y_ = 0;
    }
    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_) {//nếu nó	lớn hơn vị trí lớn nhất của bản đồ thì không cho bản đồ di chuyển thêm nữa
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

void MainObject::CheckToMap(Map& map_data,Mix_Chunk* sound_coins) {
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
			int res1=map_data.tile[y1][x2];
			int res2=map_data.tile[y2][x2];
			if (res1 == STATE_MONEY ) {//va chạm phải tiền
                map_data.tile[y1][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
			else if (res2 == STATE_MONEY ) {//va chạm phải tiền
                map_data.tile[y2][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
            else {
				if (res1 != BLANK_TILE || res2 != BLANK_TILE) {
                    x_pos_ = x2 * TILE_SIZE;
                    x_pos_ -= width_frame_ + 1;
                    x_val_ = 0;
                }
            }
			if(res1==8||res2==8||res1==10||res2==10||res1==11||res2==11||res1==27||res2==27||res1==30||res2==30||res1==31||res2==31){
				encounter_obstacles_=true;
			}
			if (res1 == 28 ) {//va chạm phải maus
                map_data.tile[y1][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			else if (res2 == 28 ) {//va chạm phải tiền
                map_data.tile[y2][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			if(res1==32){
				map_data.tile[y1][x2] = 0;
				item_win=true;
			}
			else if(res1==32){
				map_data.tile[y2][x2] = 0;
				item_win=true;
			}
        }
        else if (x_val_ < 0) {
			int res1=map_data.tile[y1][x1];
			int res2=map_data.tile[y2][x1];
			if (res1 == STATE_MONEY ) {//va chạm phải tiền
                map_data.tile[y1][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
			if (res2 == STATE_MONEY ) {//va chạm phải tiền
                map_data.tile[y2][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
            else {
				if (res1 != BLANK_TILE || res2 != BLANK_TILE) {
                    x_pos_ = (x1 + 1) * TILE_SIZE;
                    x_val_ = 0;
                }
            }
			if(res1==27||res2==27||res1==8||res2==8||res1==9||res2==9||res1==10||res2==10||res1==11||res2==11){
				encounter_obstacles_=true;
			}
			if (res1 == 28 ) {//va chạm phải tiền
                map_data.tile[y1][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			else if (res2 == 28 ) {//va chạm phải tiền
                map_data.tile[y2][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			if(res1==32){
				map_data.tile[y1][x1] = 0;
				item_win=true;
			}
			else if(res1==32){
				map_data.tile[y2][x1] = 0;
				item_win=true;
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
			int res1=map_data.tile[y2][x1];
			int res2=map_data.tile[y2][x2];
			if (res1 == STATE_MONEY ){//va chạm phải tiền
                map_data.tile[y2][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
			else if (res2 == STATE_MONEY ){//va chạm phải tiền
                map_data.tile[y2][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
            else {
				if (res1 != BLANK_TILE || res2 != BLANK_TILE) {
                    y_pos_ = y2 * TILE_SIZE;
                    y_pos_ -= height_frame_ + 1;
                    y_val_ = 0;
                    on_ground_ = true;
                }
            }
			if(res1==8||res2==8||res1==10||res2==10||res1==11||res2==11||res1==27||res2==27||res1==30||res2==30||res1==31||res2==31){
				encounter_obstacles_=true;
			}
			if (res1 == 28 ) {//va chạm phải tiền
                map_data.tile[y2][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			else if (res2 == 28 ) {//va chạm phải tiền
                map_data.tile[y2][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			if(res1==32){
				map_data.tile[y2][x1] = 0;
				item_win=true;
			}
			else if(res1==32){
				map_data.tile[y2][x2] = 0;
				item_win=true;
			}
        }
        else if (y_val_ < 0) {//nhân vật đang nhảy
			int res1=map_data.tile[y1][x1];
			int res2=map_data.tile[y1][x2];
            if (res1 == STATE_MONEY ) {//va chạm phải tiền
                map_data.tile[y1][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
			else if (res2 == STATE_MONEY ) {//va chạm phải tiền
                map_data.tile[y1][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
                money_count++;
				Mix_PlayChannel(-1,sound_coins,0);
            }
            else {
                if (res1 != BLANK_TILE || res2 != BLANK_TILE) {
                    y_pos_ = (y1 + 1) * TILE_SIZE;
                    y_val_ = 0;
                }
            }
			if(res1==8||res2==8||res1==10||res2==10||res1==11||res2==11||res1==27||res2==27||res1==30||res2==30||res1==31||res2==31){
				encounter_obstacles_=true;
			}
			if (res1 == 28 ) {//va chạm phải tiền
                map_data.tile[y1][x1] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			else if (res2 == 28 ) {//va chạm phải tiền
                map_data.tile[y1][x2] = 0;//gán cho ô tile map = 0 để ô đó mất đi xem như đã ăn được tiền
				heart_count=1;
            }
			if(res1==32){
				map_data.tile[y1][x1] = 0;
				item_win=true;
			}
			else if(res1==32){
				map_data.tile[y1][x2] = 0;
				item_win=true;
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
		rod_=true;
        come_back_time_ = 50;
    }
}

void MainObject::UpdateImagePlayer(SDL_Renderer* des) {
    if (on_ground_ == true) {//trên mặt đất
        if (status_ == WALK_LEFT) {//bên trái
            LoadImg("img//player_left.png", des);
        }
        else {
            LoadImg("img//player_right.png", des);
        }
    }
    else {
        if (status_ == WALK_LEFT) {//bên trái
            LoadImg("img//jump_left.png", des);
        }
        else {
            LoadImg("img//jump_right.png", des);
        }
    }
}
void MainObject::RemoveBullet(const int& idx) {
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size) {//nếu băng đạn có đạn và vị trí viên đạn nhỏ hơn số đạn trong băng thì ta xóa
        BulletObject* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);
        if (p_bullet) {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}