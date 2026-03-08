#pragma once



class Player : public vnCharacter
{
public:
	//コンスト&&デストラ
	Player(const WCHAR* folder, const WCHAR* file);
	~Player();

	//何処でもPlayerの情報を持ってこれるようにする関数
	static Player* getInstance();

	//システム関数
	virtual void execute();

	vnMotionData* motion_idle;
	vnMotionData* motion_run;
	vnMotionData* motion_jump;

private:
	Collider::Cube collide;	//物理判定用コライダー



	static Player* sInstance;

	float			currentSpeed;	//現在のスピード
	const float		MaxSpeed = 0.1f;//最大のスピード
	bool isInputKey;
	//極座標の情報
	float radius;	//半径
	float theta;	//角度(平面角 / 経度)
	float phi;		//角度(仰角 / 緯度)
	bool air;

	bool isDead = false;

	//重力
	const XMVECTOR	Gravity = XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f);

	//速度
	XMVECTOR MoveVector = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR Velocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//ジャンプ力
	const XMVECTOR	JumpForce = XMVectorSet(0.0f, 0.25f, 0.0f, 0.0f);

public:
 	Collider::Cube* getCubeCollider();//渡す用

	//加速値の設定
	void setVelocity(float x, float y, float z);
	void setVelocity(const XMVECTOR& v);
	void setVelocityX(float value);
	void setVelocityY(float value);
	void setVelocityZ(float value);
	void addVelocity(float x, float y, float z);
	void addVelocity(const XMVECTOR& v);
	void addVelocityX(float value);
	void addVelocityY(float value);
	void addVelocityZ(float value);

	//加速値の取得
	XMVECTOR* getVelocity();
	XMVECTOR getVelocityConst() const;
	float getVelocityX(void);
	float getVelocityY(void);
	float getVelocityZ(void);
	
	bool getIsDead();
	void setAir(bool isAir);
};