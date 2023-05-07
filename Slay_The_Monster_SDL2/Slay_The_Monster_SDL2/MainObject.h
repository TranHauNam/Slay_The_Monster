#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include"CommonFunc.h"
#include"BaseObject.h"
#include"BulletObject.h"
using namespace std;

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 7
#define PLAYER_JUMP_VAL 15

//int num_die = 0;// số lần bị chết

class MainObject : public BaseObject {
public:
    //hàm tạo và hàm hủy
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_RIGHT = 0,
        WALK_LEFT = 1,
    };

    bool LoadImg(string path, SDL_Renderer* screen);//hàm load ảnh nhân vật
    void Show(SDL_Renderer* des);
    void HandelInputAction(SDL_Event events, SDL_Renderer* screen,Mix_Chunk* sound_bullet,Mix_Chunk* sound_jump/*Mix_Chunk* sound_footsteps*/);//hàm xử lí sự kiện
    void set_clips();
    void DoPlayer(Map& map_data,Mix_Chunk* sound_coins);
    void CheckToMap(Map& map_data,Mix_Chunk* sound_coins);
    void SetMapXY(int map_x, int map_y) {
        map_x_ = map_x;
        map_y_ = map_y;
    };
    void CenterEntityOnMap(Map& map_data);
    void UpdateImagePlayer(SDL_Renderer* des);
    SDL_Rect GetRectFrame();

    void HandelBullet(SDL_Renderer* des);
    void RemoveBullet(const int& idx);//vị trí viên đạn thứ bao nhiêu được xóa

    void set_bullet_list(vector<BulletObject*> p_bullet_list) {
        p_bullet_list_ = p_bullet_list;
    }
    vector<BulletObject*> get_bullet_list() {
        return p_bullet_list_;
    }
    int get_frame_width() const { return width_frame_; }
    int get_frame_height() const { return height_frame_; }
	void set_come_back_time(const int& come_back_time) {
		come_back_time_=come_back_time;
	}
	int get_come_back_time() {
		return come_back_time_;
	}
	bool get_rod(){
		return rod_;
	}
	int get_money_count() {
		return money_count;
	}
	int get_heart_count() {
		return heart_count;
	}
	void set_heart_count(const int& heart) {
		heart_count=heart;
	}
	bool get_item_win() {
		return item_win;;
	}
	void set_item_win(const bool& item) {
		item_win=item;;
	}
	void set_rod(const bool& rod) {
		rod_=rod;
	}
	void set_encounter_obstacles(const bool& encounter_obstacles){
		encounter_obstacles_=encounter_obstacles;
	}
	bool get_encounter_obstacles(){
		return encounter_obstacles_;
	}

private:
    int money_count;//số tiền ăn được
	int heart_count;
    vector<BulletObject*> p_bullet_list_;//lưu trữ danh sách các viên đạn

    float x_val_;//biến để thể hiện khi ta sang trái sang phải 1 đoạn bao nhiêu
    float y_val_;//lên xuống

    float x_pos_;// biến để thể hiện vị trí hiện tại
    float y_pos_;

    // biến để lưu kích thước từng frame
    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];//mảng lưu 8 frame
    Input input_tyle_;//biến để lưu trạng thái
    int frame_;//lưu chỉ số của frame tức là đang ở frame nào(0,1,2,....,8)
    int status_;//biến để thể hiện đang di chuyển sang bên trái hay sang bên phải
    bool on_ground_;
    //biến để lưu vị trí hiện tại của bản đồ
    int map_x_;
    int map_y_;
    int come_back_time_;//thời gian quay trở lại khi bị rơi xuống vực thẳm or chết
	bool rod_;
	bool encounter_obstacles_;
	bool item_win;
};



#endif
