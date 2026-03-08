//vn_character.h
#pragma once

#include "../framework/vn_motionData .h"

vnMotionData* loadMotionFile(const WCHAR* path);
void deleteMotionFile(vnMotionData* p);



class vnCharacter : public vnObject
{
private:
	//パーツ数
	int PartsNum;

	//パーツごとのオブジェクトデータ
	vnObject** pParts;

	//ボーンデータ(パーツ名、バインドポーズ参照用)
	vnModel_BoneData* pBoneData;


	//再生中のモーションデータ
	vnMotionData* pMotion;

	//モーションの再生位置
	float Time;

	bool isLoop;
	float RateVel;
	bool isMotionEnd;

public:
	vnCharacter(const WCHAR* folder, const WCHAR* file);
	~vnCharacter();

	void execute();

	//バインドポーズに戻す
	void bindPose();

	//モーションの設定
	void setMotion(vnMotionData* p);

	int getPartsNum();
	vnObject* getParts(int id);
	vnObject* getParts(const char *name);

	void setMotionLoop(bool loop);
	void setMotionRate(float Rate = 1.0f);
	bool getMotionEnd();
	void TimeReset();
};
