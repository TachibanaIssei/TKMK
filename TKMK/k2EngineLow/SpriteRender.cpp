#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

void nsK2EngineLow::SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
{
	SpriteInitData initData;
	//DDSファイル(画像データ)のファイルパスを指定する
	initData.m_ddsFilePath[0] = filePath;
	//Sprite表示用のシェーダーのファイルパスを指定する
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	//スプライトの幅と高さを指定する
	initData.m_width = static_cast<UINT>(w);
	initData.m_height = static_cast<UINT>(h);
	//スプライトのブレンドモードを設定
	initData.m_alphaBlendMode = alphaBlendMode;

	//Sprite初期化オブジェクトを使用して、Spriteを初期化する
	m_sprite.Init(initData);

}

void nsK2EngineLow::SpriteRender::Draw(RenderContext& rc)
{
	g_renderingEngine->AddSpriteList(this);
}
