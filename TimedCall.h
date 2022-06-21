#pragma once
#include <functional>

/// <summary>
/// ��������
/// </summary>
class TimedCall
{
public:
	//�R���X�g���N�^
	TimedCall(std::function<int(int)> func, uint32_t time);
	//�X�V
	void Update();
	//�����Ȃ�true��Ԃ�
	bool IsFinished() { return isFinished; };

private:
	//�R�[���o�b�N
	std::function<int(int)> func_;
	//�c�莞��
	uint32_t time_;
	//�����t���O
	bool isFinished = false;
};

