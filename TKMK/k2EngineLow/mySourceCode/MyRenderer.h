#pragma once

namespace nsK2EngineLow
{
	//ビューポートの最大数
	const int MAX_VIEWPORT = 4;
	//2人プレイ時のビューポート数
	const int DUO_VIEWPORT = 2;

	// シーンのディレクションライトの最大数
	const int MAX_DIRECTIONAL_LIGHT = 4;

	// シャドウマップ
	enum {
		SHADOW_MAP_AREA_NEAR,		//近影範囲のシャドウマップ。
		SHADOW_MAP_AREA_MIDDLE,		//中影範囲のシャドウマップ。
		SHADOW_MAP_AREA_FAR,		//遠影範囲のシャドウマップ。
		NUM_SHADOW_MAP
	};

	// レンダリングターゲットのフォーマット。
	struct RenderTargetFormat {
		DXGI_FORMAT colorBufferFormat;	// カラーバッファのフォーマット。
		DXGI_FORMAT depthBufferFormat;	// 深度バッファのフォーマット。
	};
	// ハードシャドウ用のシャドウマップのフォーマット。
	const RenderTargetFormat g_hardShadowMapFormat = {
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	};
	//ソフトシャドウ用のシャドウマップのフォーマット
	const RenderTargetFormat g_softShadowMapFormat = {
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	};
	// メインレンダリングターゲットのフォーマット。
	const RenderTargetFormat g_mainRenderTargetFormat = {
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	};
	//ZPrepassレンダーターゲットのフォーマット
	const RenderTargetFormat g_zPrepassRenderTargetFormat = {
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	};
}
