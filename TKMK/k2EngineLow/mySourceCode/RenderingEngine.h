#pragma once

#include "MyRenderer.h"
#include "PostEffect/PostEffect.h"
#include "Light/Light.h"
#include "Shadow/Shadow.h"

namespace nsK2EngineLow {
	static const int MAX_VIEWPORT = 4;
	static const int DUO_VIEWPORT = 2;

	class IRenderer;
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine : public Noncopyable
	{
	public:
		enum EnGameMode {
			enGameMode_SoloPlay = 1,
			enGameMode_DuoPlay,
			enGameMode_TrioPlay,
			enGameMode_QuartetPlay,
			enGameMode_Num
		};
		/// <summary>
		/// 現在のカメラ描画
		/// </summary>
		enum EnCameraDrawing
		{
			enCameraDrawing_Solo = 0,		//1画面だけのとき
			enCameraDrawing_Left = 0,		//左画面
			enCameraDrawing_Right = 1,		//右画面
			enCameraDrawing_LeftUp = 0,		//左上
			enCameraDrawing_RightUp = 1,	//右上
			enCameraDrawing_LeftDown = 2,	//左下
			enCameraDrawing_RightDown = 3	//右下
		};

		void Init();

		void InitRenderTargets();
		void InitCopyToFrameBufferSprite();

		/// <summary>
		/// 描画オブジェクトを追加
		/// </summary>
		/// <param name="renderObject">描画オブジェクト</param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.emplace_back(renderObject);
		}

		/// <summary>
		/// スプライトレンダークラスをリストに追加する
		/// </summary>
		/// <param name="spriteRender">スプライトレンダー</param>
		void AddSpriteList(SpriteRender* spriteRender, const bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterSpriteList.push_back(spriteRender);
			}
			else {
				m_spriteList.push_back(spriteRender);
			}
		}
		void AddFrontSpriteList(SpriteRender* spriteRender)
		{
			m_spriteFrontDrawList.push_back(spriteRender);
		}

		/// <summary>
		/// 画面分割中のビューポートに描画するスプライトをリストに追加
		/// </summary>
		/// <param name="spriteRender"></param>
		/// <param name="viewportNo"></param>
		void AddSpriteDrawViewportList(SpriteRender* spriteRender, const int viewportNo)
		{
			m_spriteDrawViewportList[viewportNo].push_back(spriteRender);
		}

		/// <summary>
		/// フォントレンダークラスをリストに追加する
		/// </summary>
		/// <param name="fontRender">フォントレンダー</param>
		void AddFontList(FontRender* fontRender, const bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterFontList.push_back(fontRender);
			}
			else
			{
				m_fontList.push_back(fontRender);
			}
		}
		/// <summary>
		/// 画面分割中のビューポートに描画するフォントレンダークラスをリストに追加する
		/// </summary>
		/// <param name="spriteRender"></param>
		/// <param name="viewportNo"></param>
		void AddFontDrawViewportList(FontRender* fontRender, const int viewportNo)
		{
			m_fontDrawViewportList[viewportNo].push_back(fontRender);
		}

		/// <summary>
		/// シャドウモデルを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ShadowModelRendering(RenderContext& rc, Camera& camera, const int number);

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// シーンライトを取得
		/// </summary>
		/// <returns>シーンライト</returns>
		SceneLight& GetSceneLight(const int lightNumber)
		{
			return m_sceneLight[lightNumber];
		}

		/// <summary>
		/// ライトビュープロジェクション行列を設定する
		/// </summary>
		/// <param name="LVP">ライトビュープロジェクション行列</param>
		void SetmLVP(const int cameraDrawing, const Matrix LVP)
		{
			m_sceneLight[cameraDrawing].SetmLVP(LVP);
		}

		////////////////////////////////////////////////////////////////////////////////
		///シャドウマップの関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// シャドウマップのテクスチャを取得
			/// </summary>
			/// <returns>シャドウマップのテクスチャ</returns>
		Texture& GetShadowMapTexture(const int number)
		{
			return m_shadow.GetShadowMapTexture(number);
		}

		/// <summary>
		/// ライトカメラを取得
		/// </summary>
		/// <returns>ライトカメラ</returns>
		Camera& GetLightCamera(const int number)
		{
			return m_shadow.GetLightCamera(number);
		}

		////////////////////////////////////////////////////////////////////////////////
		///ディレクションライトの関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// ディレクションライトを設定
			/// </summary>
			/// <param name="lightNo">ライト番号</param>
			/// <param name="direction">ライト方向</param>
			/// <param name="color">ライト色</param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_Right].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_RightDown].SetDirectionLight(lightNo, direction, color);
			m_sceneLight[enCameraDrawing_LeftDown].SetDirectionLight(lightNo, direction, color);
		}
		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		void SetDirLightDirection(const Vector3 direction)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_Right].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_RightDown].SetDirLightDirection(direction);
			m_sceneLight[enCameraDrawing_LeftDown].SetDirLightDirection(direction);
		}
		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetDirLightColor(const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_RightDown].SetDirLightColor(color);
			m_sceneLight[enCameraDrawing_LeftDown].SetDirLightColor(color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetDirLigDirection();
		}
		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetDirLigColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetDirLigColor();
		}

		////////////////////////////////////////////////////////////////////////////////
		///環境光の関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 環境光を設定
			/// </summary>
			/// <param name="ambient">環境光</param>
		void SetAmbient(const Vector3 ambient)
		{
			m_sceneLight[enCameraDrawing_Left].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_Right].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_RightDown].SetAmbient(ambient);
			m_sceneLight[enCameraDrawing_LeftDown].SetAmbient(ambient);
		}

		////////////////////////////////////////////////////////////////////////////////////////
		///カメラの位置の関数
		////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// カメラの位置を設定する
			/// </summary>
			/// <param name="eyePos"></param>
		void SetEyePos(const Vector3& eyePosLeft, const Vector3& eyePosRight, const Vector3& eyePosLeftDown, const Vector3& eyePosRightDown)
		{
			m_sceneLight[enCameraDrawing_Left].SetEyePos(eyePosLeft);
			m_sceneLight[enCameraDrawing_Right].SetEyePos(eyePosRight);
			m_sceneLight[enCameraDrawing_LeftDown].SetEyePos(eyePosLeftDown);
			m_sceneLight[enCameraDrawing_RightDown].SetEyePos(eyePosRightDown);

		}

		////////////////////////////////////////////////////////////////////////////////
		///ポイントライトの関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// ポイントライトを設定する
			/// </summary>
			/// <param name="pos">ライトの位置</param>
			/// <param name="color">ライトの色</param>
			/// <param name="range">xにライトの影響範囲,yに影響範囲に累乗するパラメータ</param>
		void SetPointLight(const Vector3 pos, const Vector3 color, const Vector3 range)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_Right].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLight(pos, color, range);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLight(pos, color, range);
		}
		/// <summary>
		/// ポイントライトの座標を設定する
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(const Vector3 pos)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_Right].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLightPosition(pos);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLightPosition(pos);
		}
		/// <summary>
		/// ポイントライトの色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetPointLightColor(const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLightColor(color);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLightColor(color);
		}
		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetPointLightAttn(const Vector3 attn)
		{
			m_sceneLight[enCameraDrawing_Left].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_Right].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_LeftDown].SetPointLightAttn(attn);
			m_sceneLight[enCameraDrawing_RightDown].SetPointLightAttn(attn);
		}
		/// <summary>
		/// ポイントライトを使用する
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight[enCameraDrawing_Left].UsePointLight();
			m_sceneLight[enCameraDrawing_Right].UsePointLight();
			m_sceneLight[enCameraDrawing_RightDown].UsePointLight();
			m_sceneLight[enCameraDrawing_LeftDown].UsePointLight();
		}
		/// <summary>
		/// ポイントライトを使用しない
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUsePointLight();
			m_sceneLight[enCameraDrawing_Right].UnUsePointLight();
			m_sceneLight[enCameraDrawing_RightDown].UnUsePointLight();
			m_sceneLight[enCameraDrawing_LeftDown].UnUsePointLight();
		}

		/// <summary>
		/// ポイントライトの位置を取得する
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPointLightPosition(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightPosition();
		}
		/// <summary>
		/// ポイントライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetPointLightColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightColor();
		}
		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetPointLightAttn(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetPointLightAttn();
		}
		/// <summary>
		/// ポイントライトは使用中?
		/// </summary>
		/// <returns>使用中ならtrue</returns>
		const int PointLightIsUse(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].PointLightIsUse();
		}

		////////////////////////////////////////////////////////////////////////////////
		///スポットライトの関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// スポットライトを設定する
			/// </summary>
			/// <param name="pos">位置</param>
			/// <param name="color">色</param>
			/// <param name="range">xに影響範囲,yに影響範囲に累乗するパラメータ</param>
			/// <param name="direction">照射方向</param>
			/// <param name="angle">xは照射角度,ｙは影響に累乗するパラメータ</param>
		void SetSpotLight(const Vector3 pos, const Vector3 color, const Vector3 attn, const Vector3 direction, const Vector3 angle)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_Right].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLight(pos, color, attn, direction, angle);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLight(pos, color, attn, direction, angle);
		}
		/// <summary>
		/// スポットライトの位置を設定する
		/// </summary>
		/// <param name="pos">座標</param>
		void SetSpotLightPosition(const Vector3 pos)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightPosition(pos);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightPosition(pos);
		}
		/// <summary>
		/// スポットライトのライト色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetSpotLightColor(const Vector3 color)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightColor(color);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightColor(color);
		}
		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetSpotLightAttn(const Vector3 attn)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightAttn(attn);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightAttn(attn);
		}
		/// <summary>
		/// スポットライトのライトの方向を設定
		/// </summary>
		/// <param name="direction">方向</param>
		void SetSpotLightDirection(const Vector3 direction)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightDirection(direction);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightDirection(direction);
		}
		/// <summary>
		/// スポットライトのライトの角度を設定
		/// </summary>
		/// <param name="angle">角度</param>
		void SetSpotLightAngle(const Vector3 angle)
		{
			m_sceneLight[enCameraDrawing_Left].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_Right].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_LeftDown].SetSpotLightAngle(angle);
			m_sceneLight[enCameraDrawing_RightDown].SetSpotLightAngle(angle);
		}
		/// <summary>
		/// スポットライトを使用する
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight[enCameraDrawing_Left].UseSpotLight();
			m_sceneLight[enCameraDrawing_Right].UseSpotLight();
			m_sceneLight[enCameraDrawing_RightDown].UseSpotLight();
			m_sceneLight[enCameraDrawing_LeftDown].UseSpotLight();
		}
		/// <summary>
		/// スポットライトを使用しない
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_Right].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_LeftDown].UnUseSpotLight();
			m_sceneLight[enCameraDrawing_RightDown].UnUseSpotLight();
		}

		/// <summary>
		/// スポットライトの位置を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetSpotLightPosition(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightPosition();
		}
		/// <summary>
		/// スポットライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetSpotLightColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightColor();
		}
		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn(const EnCameraDrawing cameraDrawing)const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightAttn();
		}
		/// <summary>
		/// スポットライトの光の方向を取得する
		/// </summary>
		/// <returns>方向</returns>
		const Vector3& GetSpotLightDirection(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightDirection();
		}
		/// <summary>
		/// スポットライトの角度を取得する
		/// </summary>
		/// <returns>角度</returns>
		const Vector3& GetSpotLightAngle(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetSpotLightAngle();
		}
		/// <summary>
		/// スポットライトは使用中？
		/// </summary>
		/// <returns>使用中の場合true</returns>
		const int SpotLightIsUse(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].SpotLightIsUse();
		}

		////////////////////////////////////////////////////////////////////////////////
		///半球ライトの関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 半球ライトを設定する
			/// </summary>
			/// <param name="groundColor">地面色</param>
			/// <param name="skyColor">天球色</param>
			/// <param name="groundNormal">地面の法線</param>
		void SetHemiLight(const Vector3 groundColor, const Vector3 skyColor, const Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}
		/// <summary>
		/// 半球ライトの地面色を設定
		/// </summary>
		/// <param name="groundColor">地面色</param>
		void SetHemiLightGroundColor(const Vector3 groundColor)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_RightDown].SetHemiLightGroundColor(groundColor);
			m_sceneLight[enCameraDrawing_LeftDown].SetHemiLightGroundColor(groundColor);
		}
		/// <summary>
		/// 半球ライトの天球色を設定
		/// </summary>
		/// <param name="skyColor">天球色</param>
		void SetHemiLightSkyColor(const Vector3 skyColor)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_RightDown].SetHemiLightSkyColor(skyColor);
			m_sceneLight[enCameraDrawing_LeftDown].SetHemiLightSkyColor(skyColor);
		}
		/// <summary>
		/// 半球ライトの地面の法線
		/// </summary>
		/// <param name="normal">地面の法線</param>
		void SetHemiLightGroundNormal(const Vector3 normal)
		{
			m_sceneLight[enCameraDrawing_Left].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_Right].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_RightDown].SetHemiLightGroundNormal(normal);
			m_sceneLight[enCameraDrawing_LeftDown].SetHemiLightGroundNormal(normal);
		}
		/// <summary>
		/// 半球ライトを使用する
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight[enCameraDrawing_Left].UseHemiLight();
			m_sceneLight[enCameraDrawing_Right].UseHemiLight();
			m_sceneLight[enCameraDrawing_RightDown].UseHemiLight();
			m_sceneLight[enCameraDrawing_LeftDown].UseHemiLight();
		}
		/// <summary>
		/// 半球ライトを使用しない
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight[enCameraDrawing_Left].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_Right].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_RightDown].UnUseHemiLight();
			m_sceneLight[enCameraDrawing_LeftDown].UnUseHemiLight();
		}

		/// <summary>
		/// 半球ライトの地面色を取得する
		/// </summary>
		/// <returns>地面色</returns>
		const Vector3& GetHemiLightGroundColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightGroundColor();
		}
		/// <summary>
		/// 半球ライトの天球色を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightSkyColor();
		}
		/// <summary>
		/// 半球ライトの地面の法線を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].GetHemiLightGroundNormal();
		}
		/// <summary>
		/// 半球ライトは使用中?
		/// </summary>
		/// <returns>使用中のときtrue</returns>
		const int HemiLightIsUse(const EnCameraDrawing cameraDrawing) const
		{
			return m_sceneLight[cameraDrawing].HemiLightIsUse();
		}

		////////////////////////////////////////////////////////////////////////////////
		///画面分割の関数
		////////////////////////////////////////////////////////////////////////////////

			/// <summary>
			/// ゲームモードをレンダリングエンジンに設定する
			/// </summary>
		void SetGameModeToRenderingEngine(const EnGameMode gameMode)
		{
			m_gameMode = gameMode;
		}
		/// <summary>
		/// ゲームモードを取得
		/// </summary>
		/// <returns></returns>
		const EnGameMode GetGameMode() const
		{
			return m_gameMode;
		}
		/// <summary>
		/// どちらのカメラを描画中か
		/// </summary>
		/// <returns></returns>
		const EnCameraDrawing GetCameraDrawing() const
		{
			return m_cameraDrawing;
		}

		/// <summary>
		/// フレームの最初に行われるエフェクトの処理
		/// </summary>
		void EffectBeginRender();

	private:
		/// <summary>
		/// ビューポートをInitする
		/// </summary>
		void InitViewPorts();

		/// <summary>
		/// モデル描画をビューポートの数実行する関数
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void DrawModelInViewPorts(RenderContext& rc);

		/// <summary>
		/// フォワードレンダリングでのモデル描画
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);
		/// <summary>
		/// スプライトを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void SpriteRendering(RenderContext& rc, const bool drawTiming = false);
		/// <summary>
		/// スプライトを画面分割中のビューポートに描画
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void SpriteViewportRendering(RenderContext& rc, const int viewportNo);
		void SpriteFrontRendering(RenderContext& rc);
		/// <summary>
		/// フォントを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void FontRendering(RenderContext& rc, const bool drawTiming = false);
		/// <summary>
		/// フォントを画面分割中のビューポートに描画
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void FontViewportRendering(RenderContext& rc, const int viewportNo);
		/// <summary>
		/// エフェクト描画を実行する
		/// </summary>
		/// <param name="rc"></param>
		void ExcuteEffectRender(RenderContext& rc);
		/// <summary>
		/// 2D描画を行う
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// 描画リストをclearする
		/// </summary>
		void ClearRenderList();

	private:
		std::vector<IRenderer*>		m_renderObjects;
		std::vector<SpriteRender*>	m_spriteList;							//スプライトクラスのリスト
		std::vector<SpriteRender*>	m_laterSpriteList;						//描画順が遅いスプライトクラスのリスト
		std::vector<SpriteRender*>	m_spriteDrawViewportList[MAX_VIEWPORT];	//画面分割中のビューポートに描画するスプライトのリスト
		std::vector<SpriteRender*>	m_spriteFrontDrawList;					//全てにおいて前面に描画される
		std::vector<FontRender*>	m_fontList;								//フォントクラスのリスト
		std::vector<FontRender*>	m_laterFontList;						//laterSpriteの上から文字を描画
		std::vector<FontRender*>	m_fontDrawViewportList[MAX_VIEWPORT];	//画面分割中のビューポートに描画するフォントクラスのリスト

		SceneLight					m_sceneLight[MAX_VIEWPORT];				//シーンライト

		RenderTarget				m_mainRenderTarget;						//メインレンダーターゲット
		RenderTarget				m_2DRenderTarget;						//2Dレンダーターゲット
		Sprite						m_2DSprite;
		Sprite						m_mainSprite;
		Sprite						m_copyToFrameBufferSprite;				//テクスチャを貼り付けるためのスプライトを初期化

		Shadow						m_shadow;								//シャドウマップ
		PostEffect					m_postEffect;							//ポストエフェクト

		EnCameraDrawing m_cameraDrawing = enCameraDrawing_Left;

		D3D12_VIEWPORT m_soloViewPort;										//1画面用のビューポート
		D3D12_VIEWPORT m_duoViewPorts[DUO_VIEWPORT];						//2画面分割用のビューポート
		D3D12_VIEWPORT m_quarteViewPorts[MAX_VIEWPORT];						//4画面分割用のビューポート
		EnGameMode m_gameMode = enGameMode_SoloPlay;
	};
}