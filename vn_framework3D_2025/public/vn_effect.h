//--------------------------------------------------------------//
//	"vn_effect.h"												//
//		エフェクト(パーティクル)クラス							//
//													2025/11/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#define DebugMode 0

//パーティクル
class vnParticle
{
public:
	float		Life;	//寿命(フレーム)
	float		StartLife;	//寿命(エミット時の初期値)
	XMVECTOR	Pos;	//位置
	XMVECTOR	Vel;	//速度
	XMVECTOR	Col;	//色
	float		Size;	//サイズ
};


//エミッター
class vnEmitter : public vnObject
{
public:

	//パーティクルを放出する際の設定
	struct stEmitterDesc
	{
		WCHAR Texture[64] = L"";

		float LifeMin = 30.0f;
		float LifeMax = 60.0f;

		XMVECTOR ColorMin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR ColorMax = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		float SizeMin = 0.5f;
		float SizeMax = 1.0f;

		XMVECTOR SpeedMin = XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f);
		XMVECTOR SpeedMax = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	};
private:
	//放出するかのフラグ
	bool emit;

	stEmitterDesc Desc;

	//パーティクル配列
	vnParticle* pParticle;

	//描画パーティクル数
	int renderParticleNum;

	//描画されるインデックス数
	int IndexNum;

	//1ﾌﾚｰﾑに生成できるパーティクルの上限数
	int OneFrameEmittedParticle = 1;


	//頂点データ
	vnVertex3D* vtx;

	//インデックスデータ(三角二つの四角形)
	WORD* idx;

	//頂点バッファ
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//インデックスバッファ
	ID3D12Resource* pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	//テクスチャ
	ID3D12Resource* texbuff;
	//定数バッファ
	ID3D12Resource* constBuff;
	ID3D12DescriptorHeap* basicDescHeap;
	stConstantBuffer* pConstBuffer;

	//マテリアル関連
	XMVECTOR	Diffuse;	//拡散色
	XMVECTOR	Ambient;	//環境色
	XMVECTOR	Specular;	//鏡面反射色

	//頂点データへの各種情報の設定
	virtual void setVertexPosition();

	//particle処理の乱数用
	float getRandNum();


public:
	vnEmitter(stEmitterDesc* desc);
	virtual ~vnEmitter();

	virtual void execute();

	virtual void render();

	void setDesc(stEmitterDesc* desc);

	void setEmit(bool flag);

	bool isEmit();

	//1フレームのパーティクルの放出数を設定
	void setOneFrameEmittedParticle(int OneFrameParticleNUM);

};