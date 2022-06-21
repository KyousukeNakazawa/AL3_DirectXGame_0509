#pragma once
#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall
{
public:
	//コンストラクタ
	TimedCall(std::function<int(int)> func, uint32_t time);
	//更新
	void Update();
	//完了ならtrueを返す
	bool IsFinished() { return isFinished; };

private:
	//コールバック
	std::function<int(int)> func_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinished = false;
};

