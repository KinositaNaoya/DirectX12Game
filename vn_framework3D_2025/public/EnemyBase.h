#pragma once

class EnemyBase : public vnCharacter
{
protected:
	Collider::Cube collide;	//物理判定用コライダー

private:
	
	int elapsedTime = 0.0f;
	float Time = 0.0f;
	float Second = 0.0f;
	float Minutes = 0.0f;
	float maxTime = 300.0f;
	bool isNormal = true;

	const float phaseLength = 60.0f;

	vnSound* SkyChangeSE;
	bool skyChangeFlag;
	vnSound* Phase1_BGM;
	vnSound* Phase2_BGM;

	void UpdatePhase();

public:
	enum PHASETABLE {
		STANDBY,
		PHASE_1,
		PHASE_2,
		PHASE_3,
		PHASE_4,
		FINAL,
		GAMESET
	};
	PHASETABLE phase = STANDBY;

	EnemyBase(const WCHAR* folder, const WCHAR* file);
	~EnemyBase();


	Collider::Cube* getCubeCollider();
	PHASETABLE getPhase() const { return this->phase; }
	float getScond();
	float getMinutes();
	int getEnemyTimer();
	//フェーズの時間を0～1で返す
	float getEnemyPhaseRate();


	void TimeUpdate();	//エネミーの動きの根幹

	void setSkipPhase(PHASETABLE Phase = STANDBY);

	void SaveLevel();
	void LoadLevel();
	
};
