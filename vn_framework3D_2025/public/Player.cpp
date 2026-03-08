#include "../framework.h"
#include "../framework/vn_environment.h"

Player* Player::sInstance = nullptr;

//コンスト
Player::Player(const WCHAR* folder, const WCHAR* file) : vnCharacter(folder, file)
{
	
	

	sInstance = this;

	//三人称視点プレイヤーカメラの初期値
	radius = 15.0f;
	theta = 0.0f;
	phi = 0.0f;
	
	

	motion_idle = loadMotionFile(L"data/model/Player/motion/HumanF@Idle01.mot");
	motion_run = loadMotionFile(L"data/model/Player/motion/HumanF@Run01_Forward.mot");
	motion_jump = loadMotionFile(L"data/model/Player/motion/HumanF@Jump01 - begin.mot");

	collide.setOffset(0.0f, 1.0f, 0.0f);
	collide.setSize(1.0f, 2.0f, 1.0f, 0.0f);//物理判定のサイズ
	collide.setOwner(this);

	setPositionZ(-15.0f);

	isDead = false;
}
//デストラ
Player::~Player()
{


	deleteMotionFile(motion_idle);
	deleteMotionFile(motion_run);
	deleteMotionFile(motion_jump);
}

Player* Player::getInstance()
{
	return sInstance;
}

void Player::execute()
{
	

	setMotionLoop(true);
	//落ちたらもどる処理
	{
		if (Player::getPositionY() <= -20.0f) {
			isDead = true;
		}
	}

	//徐々にスピードを上げる処理
	{
		if (vnKeyboard::on(DIK_W) || vnKeyboard::on(DIK_A) ||
			vnKeyboard::on(DIK_S) || vnKeyboard::on(DIK_D)) {
			if (currentSpeed <= MaxSpeed)currentSpeed += 0.002f;
			isInputKey = true;
		}
		else {
			isInputKey = false;
			
			if (currentSpeed >= 0.01) {
				currentSpeed -= 0.05f;
			}
			else {
				//連打でバクステしてしまうバグ対策
				currentSpeed = 0.0f;
			}
		}
	}

	//移動処理
	{
		//プレイヤーの移動ベクトル
		MoveVector = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		if (vnKeyboard::on(DIK_W)) {
			MoveVector += XMVectorSet(0.0f, 0.0f, currentSpeed, 0.0f);
		}
		if (vnKeyboard::on(DIK_A)) {
			MoveVector += XMVectorSet(-currentSpeed, 0.0f, 0.0f, 0.0f);
		}
		if (vnKeyboard::on(DIK_S)) {
			MoveVector += XMVectorSet(0.0f, 0.0f, -currentSpeed, 0.0f);
		}
		if (vnKeyboard::on(DIK_D)) {
			MoveVector += XMVectorSet(currentSpeed, 0.0f, 0.0f, 0.0f);
		}

		//カメラの角度を回転マトリックスに変換
		XMMATRIX rotate = XMMatrixRotationY(theta);
		//移動ベクトルに回転マトリクスを適用
		MoveVector = XMVector3TransformNormal(MoveVector, rotate);
		
	}

	//方向制御処理
	{
		if (isInputKey == true) {
			float moveX = XMVectorGetX(MoveVector);
			float moveZ = XMVectorGetZ(MoveVector);
			float rotY = atan2f(moveX, moveZ);
			Player::setRotationY(rotY);
		}
	}

	//カメラの回転処理
	{
		
		theta += vnMouse::getDX() * 0.002f;
		phi += vnMouse::getDY() * 0.002f;

		//カメラの上下の限界設定( 90度 - 0.1ラジアン )
		float limit = XM_PIDIV2 - 0.1f;
		if (phi > limit)  phi = limit;
		if (phi < -limit) phi = -limit;
	}

	//カメラの座標の計算処理
	{
		XMVECTOR camPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	//camBias + PlayerPos
		XMVECTOR camTrg = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	//カメラの向く方向
		XMVECTOR camBias = XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f);	//カメラ衛星の中心点(仮)

		//カメラ衛星の位置
		XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
		//回転マトリクスを作成
		XMMATRIX camRotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);
		//カメラ衛星を移動
		eye = XMVector3TransformNormal(eye, camRotate);

		//カメラ衛星の中心をプレイヤーにする
		camPos = XMVectorAdd(eye, *Player::getPosition());
		camPos = XMVectorAdd(camPos, camBias);
		vnCamera::setPosition(&camPos);

		//カメラの注視点をプレイヤーに変更
		camTrg = XMVectorAdd(*Player::getPosition(), camBias);
		vnCamera::setTarget(&camTrg);
	}
	//重力
	Velocity += Gravity;
	
	//ジャンプの発動
	if(air == false && vnKeyboard::trg(DIK_SPACE))
	{
		air = true;	
		Velocity += JumpForce;
	}

	//自分の位置の更新
	addPosition(&MoveVector);
	addPosition(&Velocity);
	if (XMVectorGetY(Velocity) > 0.1f) {

	}

	if (air) {
		setMotionLoop(false);
		setMotion(motion_jump);
	}
	else if(XMVectorGetX(XMVector3Length(MoveVector)) != 0.0f){
		setMotion(motion_run);
	}
	else if (XMVectorGetX(XMVector3Length(MoveVector)) == 0.0f) {
		
		setMotion(motion_idle);

	}

	vnCharacter::execute();
}

Collider::Cube* Player::getCubeCollider()
{
	return &collide;
}

//加速値の設定
void Player::setVelocity(float x, float y, float z)
{
	Velocity = XMVectorSet(x, y, z, 0.0f);
}

void Player::setVelocity(const XMVECTOR& v)
{
	Velocity = v;
}

void Player::setVelocityX(float value)
{
	Velocity = XMVectorSetX(Position, value);
}

void Player::setVelocityY(float value)
{
	Velocity = XMVectorSetY(Position, value);
}

void Player::setVelocityZ(float value)
{
	Velocity = XMVectorSetZ(Position, value);
}

void Player::addVelocity(float x, float y, float z)
{
	Velocity = XMVectorAdd(Velocity, XMVectorSet(x, y, z, 0.0f));
}

void Player::addVelocity(const XMVECTOR& v)
{
	Velocity = XMVectorAdd(Velocity, v);
}

void Player::addVelocityX(float value)
{
	Velocity = XMVectorAdd(Velocity, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void Player::addVelocityY(float value)
{
	Velocity = XMVectorAdd(Velocity, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void Player::addVelocityZ(float value)
{
	Velocity = XMVectorAdd(Velocity, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

//加速値の取得
XMVECTOR* Player::getVelocity()
{
	return &Velocity;
}

XMVECTOR Player::getVelocityConst() const
{
	return Velocity;
}

float Player::getVelocityX(void)
{
	return XMVectorGetX(Velocity);
}

float Player::getVelocityY(void)
{
	return XMVectorGetY(Velocity);
}

float Player::getVelocityZ(void)
{
	return XMVectorGetZ(Velocity);
}

bool Player::getIsDead()
{
	return isDead;
}

//空中か否か
void Player::setAir(bool isAir)
{
	air = isAir;
}
