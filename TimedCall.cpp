#include "TimedCall.h"

TimedCall::TimedCall(std::function<int(int)> func, uint32_t time) :func_(func), time_(time) {

};

void TimedCall::Update() {
	if (isFinished) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished = true;
		//�R�[���o�b�N�֐��̌Ăяo��
		func_;
	}
}