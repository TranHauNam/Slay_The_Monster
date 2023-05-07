#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer {
public:
	ImpTimer();
	~ImpTimer();
	
	void start();//hàm bắt đầu chạy để đếm time chạy
	void stop();//hàm dừng lại
	void paused();//hàm bắt đầu dừng để đếm time dừng
	void unpaused();//bỏ tạm dừng

	int get_ticks();//hàm trả về thời gian đã trôi qua
	
	bool is_started();//hàm trả về trạng thái bắt đầu chạy
	bool is_paused();//hàm trả về trạng thái bắt đầu dừng





private:
	int start_tick_;//biến lưu trữ time chạy
	int paused_tick_;//biến lưu trữ time dừng
	
	bool is_started_;
	bool is_paused_;

};




#endif
