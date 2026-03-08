//vn_character.cpp
#include "../framework.h"
#include "../framework/vn_environment.h"

vnCharacter::vnCharacter(const WCHAR* folder, const WCHAR* file)
{
	//階層構造ファイル(*.bone)の読み込み
	FILE* fp = NULL;

	WCHAR path[256];
	swprintf_s(path, L"%s%s", folder, file);

	if (_wfopen_s(&fp, path, L"rb") != 0)
	{	//ファイルが開けなかった
		assert(false);
	}

	//ファイルのサイズ(Byte)を調べる
	long size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//パーツの数
	PartsNum = size / sizeof(vnModel_BoneData);
	//ファイルサイズ分のメモリを確保
	pBoneData = new vnModel_BoneData[PartsNum];

	//ファイルの中身を全て取得
	fread(pBoneData, size, 1, fp);

	fclose(fp);


	//パーツごとのオブジェクトデータ
	pParts = new vnObject * [PartsNum];

	//階層構造の構築
	for (int i = 0; i < PartsNum; i++)
	{
		//char文字列をWCHAR文字列に変換
		WCHAR name[256];
		size_t ret;
		mbstowcs_s(&ret, name, 256,
			pBoneData[i].Name, strlen(pBoneData[i].Name));
		//Multi Byte String to WChar String
		vnFont::output(L"pBoneData[%d].Name : %s\n", i, name);

		//vnmのファイル名を作る
		WCHAR partsfile[256];	//vnmファイル名
		WCHAR partspath[256];	//ファイルパス
		swprintf_s(partsfile, L"%s.vnm", name);
		swprintf_s(partspath, L"%s%s", folder, partsfile);

		//vnmファイルがあるか調べる
		if (_wfopen_s(&fp, partspath, L"rb") != 0)
		{	//vnmファイルが無い

			pParts[i] = new vnObject();
		}
		else
		{
			pParts[i] = new vnModel(folder, partsfile);
		}
		if (fp != NULL)fclose(fp);

		if (pBoneData[i].ParentID == -1)
		{	//親はvnCharacter自体
			pParts[i]->setParent(this);
		}
		else
		{
			pParts[i]->setParent(pParts[pBoneData[i].ParentID]);
		}
		pParts[i]->setPosition(pBoneData[i].pos[0], pBoneData[i].pos[1], pBoneData[i].pos[2]);
		pParts[i]->setRotation(pBoneData[i].rot[0], pBoneData[i].rot[1], pBoneData[i].rot[2]);
		pParts[i]->setScale(pBoneData[i].scl[0], pBoneData[i].scl[1], pBoneData[i].scl[2]);
	}
	pMotion = NULL;
	Time = 0.0f;
	isLoop = true;
	RateVel = 1.0f;
	isMotionEnd = false;
}

vnCharacter::~vnCharacter()
{
	delete[] pParts;
	delete[] pBoneData;
}

void vnCharacter::execute()
{
	//if (pMotion == NULL)return;
	//isMotionEnd = false;
	////時間の進行
	//Time += RateVel;
	//if (Time >= pMotion->Length)
	//{
	//	if (!isLoop) {
	//		isMotionEnd = true;
	//		return;
	//	}
	//	
	//	Time = 0.0f;
	//}

	if (pMotion == NULL) return;

	if (isMotionEnd) return;

	Time += RateVel;

	if (Time >= pMotion->Length)
	{
		if (!isLoop)
		{
			Time = pMotion->Length;  // 固定
			isMotionEnd = true;
		}
		else
		{
			Time = 0.0f;
		}
	}

	//モーションの再生

	//データからチャンネルとキーフレームの先頭アドレスを取得
	vnMotionData_Channel* channel = reinterpret_cast<vnMotionData_Channel*>(reinterpret_cast<__int64>(pMotion) + pMotion->ChannelAccess);
	vnMotionData_KeyFrame* key = reinterpret_cast<vnMotionData_KeyFrame*>(reinterpret_cast<__int64>(pMotion) + pMotion->KeyFrameAccess);

	vnObject* pObj = this;	//再生対象オブジェクト

	for (int i = 0; i < pMotion->ChannelNum; i++)
	{
		if (channel[i].Name[0] == '\0')	//名前の設定がないチャンネルはオブジェクト自体のモーションとして扱う
		{
			pObj = this;
		}
		else
		{
			pObj = getParts(channel[i].Name);
		}
		if (!pObj)continue;

		//キーフレームの先頭アドレスと数を取得
		vnMotionData_KeyFrame* keys = key + channel[i].StartIndex;
		int n = channel[i].KeyFrameNum;

		//値を評価
		float v = 0.0f;
		bool hit = false;
		for (int k = 0; k < n; k++)
		{
			if (Time == keys[k].Time)
			{
				v = keys[k].Value;
				hit = true;
				break;
			}
			else if (Time < keys[k].Time)
			{
				float rate = (Time - keys[k - 1].Time) / (keys[k].Time - keys[k - 1].Time);
				float d = keys[k].Value - keys[k - 1].Value;
				v = d * rate + keys[k - 1].Value;
				hit = true;
				break;
			}
		}
		if (!hit)continue;	//キーフレームがTimeまで設定されていなかった場合の対策
		//評価された値を適用
		switch (channel[i].ChannelID)
		{
		case eMotionChannel::PosX:	pObj->setPositionX(v);	break;
		case eMotionChannel::PosY:	pObj->setPositionY(v);	break;
		case eMotionChannel::PosZ:	pObj->setPositionZ(v);	break;
		case eMotionChannel::RotX:	pObj->setRotationX(v);	break;
		case eMotionChannel::RotY:	pObj->setRotationY(v);	break;
		case eMotionChannel::RotZ:	pObj->setRotationZ(v);	break;
		case eMotionChannel::SclX:	pObj->setScaleX(v);		break;
		case eMotionChannel::SclY:	pObj->setScaleY(v);		break;
		case eMotionChannel::SclZ:	pObj->setScaleZ(v);		break;
		}
	}
}

void vnCharacter::setMotion(vnMotionData* p)
{
	if (pMotion == p)return;

	Time = 0.0f;
	isMotionEnd = false;
	pMotion = p;
}

void vnCharacter::bindPose()
{
	for (int i = 0; i < PartsNum; i++)
	{
		pParts[i]->setPosition(
			pBoneData[i].pos[0],
			pBoneData[i].pos[1],
			pBoneData[i].pos[2]);
		pParts[i]->setRotation(
			pBoneData[i].rot[0],
			pBoneData[i].rot[1],
			pBoneData[i].rot[2]);
		pParts[i]->setScale(
			pBoneData[i].scl[0],
			pBoneData[i].scl[1],
			pBoneData[i].scl[2]);
	}
}

int vnCharacter::getPartsNum()
{
	return PartsNum;
}

vnObject* vnCharacter::getParts(int id)
{
	if (id < 0 || id >= PartsNum)return NULL;
	return pParts[id];
}

vnObject* vnCharacter::getParts(const char* name)
{
	if (!name)return NULL;

	for (int i = 0; i < PartsNum; i++)
	{
		//if (name == pBoneData[i].Name)
		if (strcmp(name, pBoneData[i].Name) == 0)
		{
			return pParts[i];
		}
	}
	return NULL;
}

void vnCharacter::setMotionLoop(bool loop)
{
	isLoop = loop;
}

bool vnCharacter::getMotionEnd()
{
	return isMotionEnd;
}

void vnCharacter::TimeReset()
{
	Time = 0.0f;
}

void vnCharacter::setMotionRate(float Rate)
{
	RateVel = Rate;
}

vnMotionData* loadMotionFile(const WCHAR* path)
{
	vnMotionData* ret = NULL;

	FILE* fp = NULL;

	if (_wfopen_s(&fp, path, L"rb") != 0)
	{
		vnFont::output(L"cannot open file : %s\n", path);
		assert(false);
		return ret;
	}

	long size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	ret = (vnMotionData*)new BYTE[size];

	fread(ret, size, 1, fp);
	fclose(fp);

	return ret;
}

void deleteMotionFile(vnMotionData* p)
{
	if (!p)return;
	delete[] p;
}
