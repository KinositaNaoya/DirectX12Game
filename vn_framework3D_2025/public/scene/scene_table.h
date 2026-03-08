//--------------------------------------------------------------//
//	"scene_table.h"												//
//		シーンテーブル											//
//													2025/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//シーンの種類
enum eSceneTable
{
	Boot,
	Null,
	Null2,
	TITLE,
	SELECT,
	STAGE_1,
	CLEAR,
	SceneMax,
};

//シーンクラス定義ファイル
#include "vn_scene.h"
#include "scene_boot.h"
#include "scene_null.h"
#include "scene_null_second.h"
#include "scene_Title.h"
#include "scene_stage1.h"
#include "scene_GameClear.h"
#include "scene_Select.h"



//シーン切り替え関数(予約)
void switchScene(eSceneTable scene);

//シーン切り替え関数(予約 + 初期値指定)
void switchScene(eSceneTable scene,XMVECTOR initPos);

//現在のシーン取得(実処理)
void switchScene();

eSceneTable getSceneTable();