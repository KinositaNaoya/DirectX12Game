//--------------------------------------------------------------//
//	"vn_object.h"												//
//		オブジェクト基底クラス									//
//													2025/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#include "../framework/vn_modelData.h"
class vnModel;

//ポリゴン頂点構造体
struct vnVertex3D
{
	float x,y,z;		//座標
	float nx,ny,nz;		//法線
	float r,g,b,a;		//カラー
	float u,v;			//UV
};

class vnObject
{
private:


public:
	//レンダーフラグ(パイプラインのパターン)
	enum eRenderFlag {
		Transparent = 0x00000001,	//半透明		(二進数::0001)
		Lighting = 0x00000002,	//ライティング		(二進数::0010)
		ZWrite = 0x00000004,	//深度書き込み		(二進数::0100)
		DoubleSided = 0x00000008,	//両面表示		(二進数::1000)
		MaxNum = 0x00000010,	//組み合わせの最大数(二進数::1001)
	};

protected:
	//移動値
	XMVECTOR	Position;
	
	//回転値
	XMVECTOR	Rotation;
	
	//拡大値
	XMVECTOR	Scale;

	//変換行列
	XMMATRIX	Local;	//自身で完結している座標
	XMMATRIX	World;	//ワールド単位の最終座標

	//階層構造の親
	vnObject* pParent;

	//実行状態の管理
	bool executeEnable;

	//描画状態の管理
	bool renderEnable;

	//ライティング有り
	bool lighting;

	//半透明
	bool transparent;

	//深度書き込みの有効/無効
	bool zWrite;

	UINT32 renderFlag;	//オブジェクト固有の描画パターン

	//静的共通データ
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[];			//頂点要素
	static ID3D12PipelineState		*pPipelineState[eRenderFlag::MaxNum];				//パイプラインステート
	
	//コンスタントバッファ構造体
	struct stConstantBuffer
	{
		XMFLOAT4X4	WVP;		//World*View*Proj
		XMFLOAT4X4	World;		//World
		XMFLOAT4	LightDir;	//平行光源の(逆)方向
		XMFLOAT4	LightCol;	//平行光源の色
		XMFLOAT4	LightAmb;	//環境光の色
		XMFLOAT4	CameraPos;	//カメラのワールド座標
		XMFLOAT4	Diffuse;	//マテリアルの拡散色
		XMFLOAT4	Ambient;	//マテリアルの環境色
		XMFLOAT4	Specular;	//マテリアルの鏡面反射光
	};

	//ローカルマトリクスを計算
	void calculateLocalMatrix();
	//ワールドマトリクスを計算
	void calculateWorldMatrix();

	

public:
	//静的共通データ初期化
	static bool initializeCommon();

	//静的共通データ削除
	static void terminateCommon();

	//コンストラクタ
	vnObject();

	//デストラクタ
	virtual ~vnObject();
	
	//システム関数
	virtual void execute();
	virtual void render();

	//移動値の設定
	void setPosition(float x, float y, float z);
	void setPosition(const XMVECTOR *v);
	void setPositionX(float value);
	void setPositionY(float value);
	void setPositionZ(float value);
	void addPosition(float x, float y, float z);
	void addPosition(const XMVECTOR *v);
	void addPositionX(float value);
	void addPositionY(float value);
	void addPositionZ(float value);

	//移動値の取得
	XMVECTOR *getPosition();
	XMVECTOR getPositionConst() const;
	float getPositionX(void);
	float getPositionY(void);
	float getPositionZ(void);

	

	//回転値の設定
	void setRotation(float x, float y, float z);
	void setRotation(const XMVECTOR *v);
	void setRotationX(float radian);
	void setRotationY(float radian);
	void setRotationZ(float radian);
	void addRotation(float x, float y, float z);
	void addRotation(const XMVECTOR *v);
	void addRotationX(float radian);
	void addRotationY(float radian);
	void addRotationZ(float radian);

	//回転値の取得
	XMVECTOR *getRotation();
	float getRotationX(void);
	float getRotationY(void);
	float getRotationZ(void);
	
	
	//拡大値の設定
	void setScale(float x, float y, float z);
	void setScale(const XMVECTOR *v);
	void setScaleX(float value);
	void setScaleY(float value);
	void setScaleZ(float value);
	void addScale(float x, float y, float z);
	void addScale(const XMVECTOR *v);
	void addScaleX(float value);
	void addScaleY(float value);
	void addScaleZ(float value);

	//拡大値の取得
	XMVECTOR *getScale();
	float getScaleX(void);
	float getScaleY(void);
	float getScaleZ(void);


	//ワールド行列の取得
	const XMMATRIX *getWorld()const;

	//ワールド座標の取得
	void getWorldPosition(XMVECTOR *v);
	float getWorldPositionX();
	float getWorldPositionY();
	float getWorldPositionZ();

	//ワールド行列の設定
	void setWorldMatrix(XMMATRIX *m);
	XMMATRIX* getWorldMatrix();


	//実行状態の設定
	void setExecuteEnable(bool flag);

	//描画状態の設定
	void setRenderEnable(bool flag);

	//実行状態の取得
	bool isExecuteEnable();

	//描画状態の取得
	bool isRenderEnable();

	//ライティング有りの設定
	void setLighting(bool flag);

	//ライティング有りの取得
	bool getLighting(void);

	//半透明の設定
	void setTransparent(bool flag);

	//半透明の取得
	bool getTransparent(void);

	//深度書き込みの有効/無効の設定
	void setZWrite(bool flag);

	//深度書き込みの有効/無効の取得
	bool getZWrite();

	//両面表示の有効/無効の設定
	void setDoubleSided(bool flag);

	//両面表示の有効/無効の取得
	bool getDoubleSided();

	//階層構造の設定
	void setParent(vnObject* p);

	//階層構造の取得
	vnObject* getParent(void);

	//レンダーフラグの設定
	void setRenderFlag(eRenderFlag flag,bool value);

	bool getRenderFlag(eRenderFlag flag);
};
